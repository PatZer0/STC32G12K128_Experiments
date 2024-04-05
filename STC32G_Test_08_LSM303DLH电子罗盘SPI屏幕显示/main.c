/*-----------------------------------------------------------------------------------------
* 请勿更改此文件编码！
* 必须保持为默认编码GB2312，否则汉字无法正常显示！

* 时钟选择 33.1776MHz
* 按键为 P1.0/1.1/1.3/1.4

* --------------------------I2C-LSM303DLH模块-电子罗盘加速度计-----------------------------
    - 默认使用P3.2/P3.3端口，修改请看 iic.c -> IIC_Init() 函数。

* -----------------------------------SPI-OLED显示屏----------------------------------------
    - 连接屏幕默认使用端口为：
      D0 - P2.5(SCLK)
      D1 - P2.3(MOSI)
      CS - P2.2(CS)
      DC - P2.1(DC)
      RES - P2.0(RES)
      修改请看 spi.h 和 spi.c

* 本实验仅实现SPI-OLED屏幕和LSM303DLH模块的功能，未结合激光测距模块
    
    由于硬件原因，按键KEY4-KEY1和屏幕的横向显示方向是反的，即屏幕左下角按键对应的是KEY4。
    如果需要修改，建议直接修改sbit key的定义。

* 注意：本程序仅供学习参考，请勿用于商业用途。
-----------------------------------------------------------------------------------------*/
#include <STRING.H>
#include <STC32G.H>
#include "oled.h"
#include "pic.h"
#include "iic.h"
#include "lsm303.h"

// 定义按键引脚
sbit key1 = P1^0;
sbit key2 = P1^1;
sbit key3 = P1^3;
sbit key4 = P1^4;
unsigned char key1_up_store = 1; // 按键1状态暂存
unsigned char key2_up_store = 1; // 按键2状态暂存
unsigned char key3_up_store = 1; // 按键3状态暂存
unsigned char key4_up_store = 1; // 按键4状态暂存

void OLED_LSM303DLH_UI_OnOff()
{
}

void OLED_LSM303DLH_UI_Range()
{
}

void OLED_LSM303DLH_UI_Refresh()
{
}

void OLED_LSM303DLH_UI_Init()
{
    OLED_BuffClear();                       //清除全部缓存
    OLED_LSM303DLH_UI_Refresh();            //刷新UI
    OLED_LSM303DLH_UI_OnOff();              //显示状态
    OLED_LSM303DLH_UI_Range();              //显示距离值
}

void OLED_Loading()
{
    OLED_BuffClear();                               //清除全部缓存
    OLED_BuffShowString(0,3,"   PLEASE WAIT  ",1);  //显示数据
    OLED_BuffShow();                                //将缓存写入显示屏显示
}

void key1_short_press()
{
    // 短按键1功能
}

void key2_short_press()
{
    // 短按键2功能
}

void key3_short_press()
{
    // 短按键3功能
}

void key4_short_press()
{
    // 短按键4功能
}

void key1_check()
{
    if(key1 == 0 && key1_up_store == 1)
    {
        key1_up_store = 0; // 标记按键被按下
        OLED_BuffShowString(13*8,6,"[-]",1);  //显示数据
        OLED_BuffShow();                    //将缓存写入显示屏显示
    }
    if(key1 == 1 && key1_up_store == 0)
    {
        key1_up_store = 1; // 标记按键被释放
        key1_short_press(); // 触发短按键1功能
        OLED_BuffShowString(13*8,6,"[-]",0);  //显示数据
        OLED_BuffShow();                    //将缓存写入显示屏显示
    }
}

void key2_check()
{
    if(key2 == 0 && key2_up_store == 1)
    {
        key2_up_store = 0; // 标记按键被按下
        OLED_BuffShowString(10*8,6,"[+]",1);  //显示数据
        OLED_BuffShow();                    //将缓存写入显示屏显示
    }
    if(key2 == 1 && key2_up_store == 0)
    {
        key2_up_store = 1; // 标记按键被释放
        key2_short_press(); // 触发短按键2功能
        OLED_BuffShowString(10*8,6,"[+]",0);  //显示数据
        OLED_BuffShow();                    //将缓存写入显示屏显示
    }
}

void key3_check()
{
    if(key3 == 0 && key3_up_store == 1)
    {
        key3_up_store = 0; // 标记按键被按下
    }
    if(key3 == 1 && key3_up_store == 0)
    {
        key3_up_store = 1; // 标记按键被释放
        key3_short_press(); // 触发短按键3功能
    }
}

void key4_check()
{
    if(key4 == 0 && key4_up_store == 1)
    {
        key4_up_store = 0; // 标记按键被按下
    }
    if(key4 == 1 && key4_up_store == 0)
    {
        key4_up_store = 1; // 标记按键被释放
        key4_short_press(); // 触发短按键4功能
    }
}

void Sys_Init()
{
    WTST = 0;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    EAXFR = 1; //扩展寄存器(XFR)访问使能
    CKCON = 0; //提高访问XRAM速度

    P0M1 = 0x00;   P0M0 = 0x00;   //设置为准双向口
    P1M1 = 0x00;   P1M0 = 0x00;   //设置为准双向口
    P1M0 = 0xff;   P1M1 = 0x00;   //设置为推挽输出
    P2M1 = 0x00;   P2M0 = 0x00;   //设置为准双向口
    P3M1 = 0x00;   P3M0 = 0x00;   //设置为准双向口
    P4M1 = 0x00;   P4M0 = 0x00;   //设置为准双向口
    P5M1 = 0x00;   P5M0 = 0x00;   //设置为准双向口
    P6M1 = 0x00;   P6M0 = 0x00;   //设置为准双向口
    P7M1 = 0x00;   P7M0 = 0x00;   //设置为准双向口
}

void main(void)
{
    EA = 1;                             //开总中断
    Sys_Init();                         //系统初始化
    SPI_init();                         //SPI初始化
    OLED_Init();                        //OLED初始化
    OLED_BuffClear();                   //清除全部缓存
    OLED_BuffShow();                    //将缓存写入显示屏显示
    IIC_Init();                         //IIC初始化
    LSM303_Init();                      //LSM303初始化
    while (1)
    {
        key1_check();                   //检查按键1
        key2_check();                   //检查按键2
        key3_check();                   //检查按键3
        key4_check();                   //检查按键4
        LSM303_Get_Acc_X();             //获取加速度X轴数据
        LSM303_Get_Acc_Y();             //获取加速度Y轴数据
        LSM303_Get_Acc_Z();             //获取加速度Z轴数据
        delay_ms(100);                  //延时100ms
    }
}
