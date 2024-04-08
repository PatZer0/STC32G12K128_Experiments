/*********************************************************************************************************************
* MM32F527X-E9P Opensourec Library 即（MM32F527X-E9P 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 MM32F527X-E9P 开源库的一部分
*
* MM32F527X-E9P 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          zf_device_dl1b
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.37
* 适用平台          MM32F527X_E9P
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-08-10        Teternal            first version
********************************************************************************************************************/
/*********************************************************************************************************************
* 接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   SCL                 查看 zf_device_dl1b.h 中 DL1B_SCL_PIN  宏定义
*                   SDA                 查看 zf_device_dl1b.h 中 DL1B_SDA_PIN  宏定义
*                   XS                  查看 zf_device_dl1b.h 中 DL1B_XS_PIN  宏定义
*                   VCC                 5V 电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/

#include "zf_delay.h"
#include "SEEKFREE_DL1B.h"
#include "SEEKFREE_CONFIG.h"

uint8 dl1b_init_flag = 0;
uint8 dl1b_finsh_flag = 0;
uint16 dl1b_distance_mm = 8192;



#define dl1b_transfer_8bit_array(tdata, tlen, rdata, rlen)      (dl1b_iic_transfer_8bit_array((tdata), (tlen), (rdata), (rlen)))


#define GET_DL1B_SDA   		 		DL1B_SDA_PIN
#define DL1B_SDA_LOW()         		DL1B_SDA_PIN = 0		//IO口输出低电平
#define DL1B_SDA_HIGH()        		DL1B_SDA_PIN = 1		//IO口输出高电平

#define DL1B_SCL_LOW()          	DL1B_SCL_PIN = 0		//IO口输出低电平
#define DL1B_SCL_HIGH()         	DL1B_SCL_PIN = 1		//IO口输出高电平

#define ack 1      //主应答
#define no_ack 0   //从应答	

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC延时
//  @return     void
//  @since      v1.0
//  Sample usage:				如果IIC通讯失败可以尝试增加j的值
//-------------------------------------------------------------------------------------------------------------------
static void dl1b_simiic_delay(void)
{
    uint16 xdata j=DL1B_SOFT_IIC_DELAY;
    while(j--);
}

//内部使用，用户无需调用
static void dl1b_simiic_start(void)
{
    DL1B_SDA_HIGH();
    DL1B_SCL_HIGH();
    dl1b_simiic_delay();
    DL1B_SDA_LOW();
    dl1b_simiic_delay();
    DL1B_SCL_LOW();
}

//内部使用，用户无需调用
static void dl1b_simiic_stop(void)
{
    DL1B_SDA_LOW();
    DL1B_SCL_LOW();
    dl1b_simiic_delay();
    DL1B_SCL_HIGH();
    dl1b_simiic_delay();
    DL1B_SDA_HIGH();
    dl1b_simiic_delay();
}

//主应答(包含ack:SDA=0和no_ack:SDA=0)
//内部使用，用户无需调用
static void dl1b_simiic_sendack(unsigned char ack_dat)
{
    DL1B_SCL_LOW();
    dl1b_simiic_delay();
    if(ack_dat) DL1B_SDA_LOW();
    else    	DL1B_SDA_HIGH();
    DL1B_SCL_HIGH();
    dl1b_simiic_delay();
    DL1B_SCL_LOW();
    dl1b_simiic_delay();
}


static int dl1b_sccb_waitack(void)
{
    DL1B_SCL_LOW();
    dl1b_simiic_delay();
    DL1B_SCL_HIGH();
    dl1b_simiic_delay();
    if(GET_DL1B_SDA)           //应答为高电平，异常，通信失败
    {
        DL1B_SCL_LOW();
        return 0;
    }
    DL1B_SCL_LOW();
    dl1b_simiic_delay();
    return 1;
}

//字节发送程序
//发送c(可以是数据也可是地址)，送完后接收从应答
//不考虑从应答位
//内部使用，用户无需调用
static void dl1b_send_ch(uint8 c)
{
    uint8 xdata i = 8;
    while(i--)
    {
        if(c & 0x80)	DL1B_SDA_HIGH();//SDA 输出数据
        else			DL1B_SDA_LOW();
        c <<= 1;
        dl1b_simiic_delay();
        DL1B_SCL_HIGH();                //SCL 拉高，采集信号
        dl1b_simiic_delay();
        DL1B_SCL_LOW();                //SCL 时钟线拉低
    }
    dl1b_sccb_waitack();
}


//字节接收程序
//接收器件传来的数据，此程序应配合|主应答函数|使用
//内部使用，用户无需调用
static uint8 dl1b_read_ch(uint8 ack_x)
{
    uint8 xdata i;
    uint8 xdata c;
    c=0;
    DL1B_SCL_LOW();
    dl1b_simiic_delay();
    DL1B_SDA_HIGH();
    for(i=0; i<8; i++)
    {
        dl1b_simiic_delay();
        DL1B_SCL_LOW();         //置时钟线为低，准备接收数据位
        dl1b_simiic_delay();
        DL1B_SCL_HIGH();         //置时钟线为高，使数据线上数据有效
        dl1b_simiic_delay();
        c<<=1;
        if(GET_DL1B_SDA)
        {
            c+=1;   //读数据位，将接收的数据存c
        }
    }
    DL1B_SCL_LOW();
    dl1b_simiic_delay();
    dl1b_simiic_sendack(ack_x);
    return c;
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口传输 8bit 数组 先写后读取
// 参数说明     *write_data     发送数据存放缓冲区
// 参数说明     write_len       发送缓冲区长度
// 参数说明     *read_data      读取数据存放缓冲区
// 参数说明     read_len        读取缓冲区长度
// 返回参数     void
// 使用示例     iic_transfer_8bit_array(IIC_1, addr, data, 64, data, 64);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void dl1b_iic_transfer_8bit_array (const uint8 *write_data, uint32 write_len, uint8 *read_data, uint32 read_len)
{
    dl1b_simiic_start();
    dl1b_send_ch(DL1B_DEV_ADDR << 1);
    while(write_len --)
    {
        dl1b_send_ch(*write_data ++);
    }
    dl1b_simiic_start();
    dl1b_send_ch(DL1B_DEV_ADDR << 1 | 0x01);
    while(read_len --)
    {
        // 前面7位需要回复ack，最后1位不需要回复ack.
        *read_data ++ = dl1b_read_ch(read_len != 0);
    }
    dl1b_simiic_stop();
}





//-------------------------------------------------------------------------------------------------------------------
// 函数简介     返回以毫米为单位的范围读数
// 参数说明     void
// 返回参数     void
// 使用示例     dl1b_get_distance();
// 备注信息     在开始单次射程测量后也调用此函数
//-------------------------------------------------------------------------------------------------------------------
void dl1b_get_distance (void)
{
    if(dl1b_init_flag)
    {
        uint8 xdata data_buffer[3];
        int16 dl1b_distance_temp = 0;
        data_buffer[0] = DL1B_GPIO__TIO_HV_STATUS >> 8;
        data_buffer[1] = DL1B_GPIO__TIO_HV_STATUS & 0xFF;
        dl1b_transfer_8bit_array(data_buffer, 2, &data_buffer[2], 1);
        if(data_buffer[2])
        {
            data_buffer[0] = DL1B_SYSTEM__INTERRUPT_CLEAR >> 8;
            data_buffer[1] = DL1B_SYSTEM__INTERRUPT_CLEAR & 0xFF;
            data_buffer[2] = 0x01;
            dl1b_transfer_8bit_array(data_buffer, 3, data_buffer, 0);// clear Interrupt
            data_buffer[0] = DL1B_RESULT__RANGE_STATUS >> 8;
            data_buffer[1] = DL1B_RESULT__RANGE_STATUS & 0xFF;
            dl1b_transfer_8bit_array(data_buffer, 2, &data_buffer[2], 1);
            if(0x89 == data_buffer[2])
            {
                data_buffer[0] = DL1B_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0 >> 8;
                data_buffer[1] = DL1B_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0 & 0xFF;
                dl1b_transfer_8bit_array(data_buffer, 2, data_buffer, 2);
                dl1b_distance_temp = data_buffer[0];
                dl1b_distance_temp = (dl1b_distance_temp << 8) | data_buffer[1];
                if(dl1b_distance_temp > 4000 || dl1b_distance_temp < 0)
                {
                    dl1b_distance_mm = 8192;
                    dl1b_finsh_flag = 0;
                }
                else
                {
                    dl1b_distance_mm = dl1b_distance_temp;
                    dl1b_finsh_flag = 1;
                }
            }
            else
            {
                dl1b_distance_mm = 8192;
                dl1b_finsh_flag = 0;
            }
        }
        else
        {
            dl1b_distance_mm = 8192;
            dl1b_finsh_flag = 0;
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 DL1B
// 参数说明     void
// 返回参数     uint8 xdata           1-初始化失败 0-初始化成功
// 使用示例     dl1b_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 dl1b_init (void)
{
    uint8 xdata   return_state    = 0;
    uint8 xdata   data_buffer[2 + sizeof(dl1b_default_configuration)];
    uint16 xdata  time_out_count  = 0;
    do
    {
        delay_ms(50);
        DL1B_XS_PIN = 0;
        delay_ms(10);
        DL1B_XS_PIN = 1;
        delay_ms(50);
        data_buffer[0] = DL1B_FIRMWARE__SYSTEM_STATUS >> 8;
        data_buffer[1] = DL1B_FIRMWARE__SYSTEM_STATUS & 0xFF;
        dl1b_transfer_8bit_array(data_buffer, 2, &data_buffer[2], 1);
        return_state = (0x01 == (data_buffer[2] & 0x01)) ? (0) : (1);
        if(1 == return_state)
        {
            break;
        }
        data_buffer[0] = DL1B_I2C_SLAVE__DEVICE_ADDRESS >> 8;
        data_buffer[1] = DL1B_I2C_SLAVE__DEVICE_ADDRESS & 0xFF;
        memcpy(&data_buffer[2], (uint8 *)dl1b_default_configuration, sizeof(dl1b_default_configuration));
        dl1b_transfer_8bit_array(data_buffer, 2 + sizeof(dl1b_default_configuration), data_buffer, 0);
        while(1)
        {
            data_buffer[0] = DL1B_GPIO__TIO_HV_STATUS >> 8;
            data_buffer[1] = DL1B_GPIO__TIO_HV_STATUS & 0xFF;
            dl1b_transfer_8bit_array(data_buffer, 2, &data_buffer[2], 1);
            if(0x00 == (data_buffer[2] & 0x01))
            {
                time_out_count = 0;
                break;
            }
            if(DL1B_TIMEOUT_COUNT < time_out_count ++)
            {
                return_state = 1;
                break;
            }
            delay_ms(1);
        }
        dl1b_init_flag = 1;
    }
    while(0);
    return return_state;
}
