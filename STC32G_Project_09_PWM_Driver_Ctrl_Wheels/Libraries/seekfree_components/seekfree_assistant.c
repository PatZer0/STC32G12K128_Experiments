/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		seekfree_assistant
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/
 

#include "seekfree_assistant.h"

static seekfree_assistant_camera_struct         seekfree_assistant_camera_data;   
seekfree_assistant_oscilloscope_struct          seekfree_assistant_oscilloscope_data;                         // 虚拟示波器数据
static seekfree_assistant_camera_dot_struct      seekfree_assistant_camera_dot_data;                          // 图像上位机打点协议数据
static seekfree_assistant_camera_buffer_struct   seekfree_assistant_camera_buffer;                            // 图像以及边界缓冲区信息

static fifo_struct      xdata seekfree_assistant_fifo;
static uint8            xdata seekfree_assistant_buffer[SEEKFREE_ASSISTANT_BUFFER_SIZE];              		// 数据存放数组
float                   xdata seekfree_assistant_parameter[SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT] = {0};    	// 保存接收到的参数
vuint8					xdata seekfree_assistant_parameter_update_flag[SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT] = {0};
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手求和函数
// 参数说明     *buffer         需要校验的数据地址
// 参数说明     length          校验长度
// 返回参数     uint8           和值
// 使用示例
//-------------------------------------------------------------------------------------------------------------------
static uint8 seekfree_assistant_sum (uint8 *buffer, uint32 length)
{
    uint8 temp_sum = 0;

    while(length--)
    {
        temp_sum += *buffer++;
    }

    return temp_sum;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手 图像发送函数
// 参数说明     camera_type     摄像头类型
// 参数说明     *image_addr     图像首地址
// 参数说明     boundary_num    图像中包含边界数量
// 参数说明     width           图像宽度
// 参数说明     height          图像高度
// 返回参数     void
// 使用示例
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_camera_data_send (seekfree_assistant_image_type_enum camera_type, void *image_addr, uint8 boundary_num, uint16 width, uint16 height)
{
    uint32 image_size = 0;

    seekfree_assistant_camera_data.head           = SEEKFREE_ASSISTANT_SEND_HEAD;
    seekfree_assistant_camera_data.function       = SEEKFREE_ASSISTANT_CAMERA_FUNCTION;
    seekfree_assistant_camera_data.camera_type    = (camera_type << 5) | ((image_addr != NULL ? 0 : 1) << 4) | boundary_num;
    // 写入包长度信息，仅包含协议部分
    seekfree_assistant_camera_data.length         = sizeof(seekfree_assistant_camera_struct);
    seekfree_assistant_camera_data.image_width    = ((((uint16)width << 8)&0xFF00) | ((uint16)width >> 8));
    seekfree_assistant_camera_data.image_height   = ((((uint16)height << 8)&0xFF00) | ((uint16)height >> 8));

    // 首先发送帧头、功能、摄像头类型、以及宽度高度等信息
    seekfree_assistant_transfer((const uint8 *)&seekfree_assistant_camera_data, sizeof(seekfree_assistant_camera_struct));

    // 根据摄像头类型计算图像大小
    switch(camera_type)
    {
        case SEEKFREE_ASSISTANT_OV7725_BIN:
        {
            image_size = width * height / 8;
        }break;

        case SEEKFREE_ASSISTANT_MT9V03X:
        {
            image_size = width * height;
        }break;

        case SEEKFREE_ASSISTANT_SCC8660:
        {
            image_size = width * height * 2;
        }break;
    }

    // 发送图像数据
    if(NULL != image_addr)
    {
        seekfree_assistant_transfer(image_addr, image_size);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手图像信息配置函数
// 参数说明     camera_type     图像类型
// 参数说明     image_addr      图像地址    如果传递NULL参数则表示只发送边线信息到上位机
// 参数说明     width           图像宽度
// 参数说明     height          图像高度
// 返回参数     void
// 使用示例                     seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_camera_information_config (seekfree_assistant_image_type_enum camera_type, void *image_addr, uint16 width, uint16 height)
{
    seekfree_assistant_camera_dot_data.head       = SEEKFREE_ASSISTANT_SEND_HEAD;
    seekfree_assistant_camera_dot_data.function   = SEEKFREE_ASSISTANT_CAMERA_DOT_FUNCTION;
    // 写入包长度信息
    seekfree_assistant_camera_dot_data.length     = sizeof(seekfree_assistant_camera_dot_struct);

    seekfree_assistant_camera_buffer.camera_type  = camera_type;
    seekfree_assistant_camera_buffer.image_addr   = image_addr;
    seekfree_assistant_camera_buffer.width        = width;
    seekfree_assistant_camera_buffer.height       = height;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手 虚拟示波器发送函数
// 参数说明     *seekfree_assistant_oscilloscope  示波器数据结构体
// 返回参数     void
// 使用示例     seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_oscilloscope_send (seekfree_assistant_oscilloscope_struct *seekfree_assistant_oscilloscope)
{
    uint8 packet_size;
	uint8 i = 0;
	seekfree_assistant_oscilloscope_struct temp_oscilloscope;
	
	temp_oscilloscope.channel_num = seekfree_assistant_oscilloscope->channel_num & 0x0f;
	
	// 由于大小端不匹配，所以需要交换。
	for(i = 0; i < temp_oscilloscope.channel_num; i++)
	{
		((uint8 *)&temp_oscilloscope.dat[i])[0] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[3];
		((uint8 *)&temp_oscilloscope.dat[i])[1] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[2];
		((uint8 *)&temp_oscilloscope.dat[i])[2] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[1];
		((uint8 *)&temp_oscilloscope.dat[i])[3] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[0];
	}

    // 帧头
    temp_oscilloscope.head         = SEEKFREE_ASSISTANT_SEND_HEAD;

    // 写入包长度信息
    packet_size                     = sizeof(temp_oscilloscope) - (SEEKFREE_ASSISTANT_SET_OSCILLOSCOPE_COUNT - temp_oscilloscope.channel_num) * 4;
    temp_oscilloscope.length       = packet_size;

    // 写入功能字与通道数量
    temp_oscilloscope.channel_num |= SEEKFREE_ASSISTANT_CAMERA_OSCILLOSCOPE;

    // 和校验计算
    temp_oscilloscope.check_sum    = 0;
    temp_oscilloscope.check_sum    = seekfree_assistant_sum((uint8 *)&temp_oscilloscope, packet_size);

    // 数据在调用本函数之前，由用户将需要发送的数据写入temp_oscilloscope.data[]

    seekfree_assistant_transfer((const uint8 *)&temp_oscilloscope, packet_size);
}

void seekfree_assistant_camera_dot_send (seekfree_assistant_camera_buffer_struct *buffer)
{
    uint8  i;
    uint16 dot_bytes = 0;   // 点字节数量

    dot_bytes = seekfree_assistant_camera_dot_data.dot_num;
	dot_bytes = ((((uint16)dot_bytes << 8)&0xFF00) | ((uint16)dot_bytes >> 8));
    if(seekfree_assistant_camera_dot_data.dot_type & (1 << 5))
    {
        dot_bytes *= 2;
    }

    // 首先发送帧头、功能、边界编号、坐标长度、点个数
    seekfree_assistant_transfer((const uint8 *)&seekfree_assistant_camera_dot_data, sizeof(seekfree_assistant_camera_dot_struct));

    for(i=0; i < SEEKFREE_ASSISTANT_CAMERA_MAX_BOUNDARY; i++)
    {
        // 判断是否发送横坐标数据
        if(NULL != buffer->boundary_x[i])
        {
            seekfree_assistant_transfer((const uint8 *)buffer->boundary_x[i], dot_bytes);
        }

        // 判断是否发送纵坐标数据
        if(NULL != buffer->boundary_y[i])
        {
            // 如果没有纵坐标数据，则表示每一行只有一个边界
            // 指定了横纵坐标数据，这种方式可以实现同一行多个边界的情况，例如搜线算法能够搜索出回弯。
            seekfree_assistant_transfer((const uint8 *)buffer->boundary_y[i], dot_bytes);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手图像边线发送配置函数
// 参数说明     boundary_type   边界类型
// 参数说明     dot_num         一条边界有多少个点
// 参数说明     dot_x1          存放边线1横坐标的地址  如果传递NULL参数则表示不发送边线1
// 参数说明     dot_x2          存放边线2横坐标的地址  如果传递NULL参数则表示不发送边线2
// 参数说明     dot_x3          存放边线3横坐标的地址  如果传递NULL参数则表示不发送边线3
// 参数说明     dot_y1          存放边线1纵坐标的地址  如果传递NULL参数则表示不发送边线1
// 参数说明     dot_y2          存放边线2纵坐标的地址  如果传递NULL参数则表示不发送边线2
// 参数说明     dot_y3          存放边线3纵坐标的地址  如果传递NULL参数则表示不发送边线3
// 返回参数     void
// 使用示例                     seekfree_assistant_camera_config(X_BOUNDARY, MT9V03X_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL, NULL);     // 图像发送时包含三条边线，边线只有横坐标
// 使用示例                     seekfree_assistant_camera_config(Y_BOUNDARY, MT9V03X_W, NULL, NULL, NULL, y1_boundary, y2_boundary, y3_boundary);     // 图像发送时包含三条边线，边线只有纵坐标
// 使用示例                     seekfree_assistant_camera_config(XY_BOUNDARY, 160, xy_x1_boundary, xy_x2_boundary, xy_x3_boundary, xy_y1_boundary, xy_y2_boundary, xy_y3_boundary);   // 图像发送时包含三条边线，边线包含横纵坐标
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_camera_boundary_config (seekfree_assistant_boundary_type_enum boundary_type, uint16 dot_num, void *dot_x1, void *dot_x2, void *dot_x3, void *dot_y1, void *dot_y2, void *dot_y3)
{
    uint8 i = 0;
    uint8 boundary_num = 0;
    uint8 boundary_data_type = 0;

    seekfree_assistant_camera_dot_data.dot_num    = ((((uint16)dot_num << 8)&0xFF00) | ((uint16)dot_num >> 8));;
    seekfree_assistant_camera_dot_data.valid_flag = 0;
    for(i = 0; i < 3; i++)
    {
        seekfree_assistant_camera_buffer.boundary_x[i] = NULL;
        seekfree_assistant_camera_buffer.boundary_y[i] = NULL;
    }

    switch(boundary_type)
    {
        case X_BOUNDARY:
        {
            if(NULL != dot_x1)
            {
                boundary_num++;
                seekfree_assistant_camera_dot_data.valid_flag |= 1 << 0;
                seekfree_assistant_camera_buffer.boundary_x[i++] = dot_x1;
            }
            if(NULL != dot_x2)
            {
                boundary_num++;
                seekfree_assistant_camera_dot_data.valid_flag |= 1 << 1;
                seekfree_assistant_camera_buffer.boundary_x[i++] = dot_x2;
            }
            if(NULL != dot_x3)
            {
                boundary_num++;
                seekfree_assistant_camera_dot_data.valid_flag |= 1 << 2;
                seekfree_assistant_camera_buffer.boundary_x[i++] = dot_x3;
            }

            if(255 < seekfree_assistant_camera_buffer.height)
            {
                boundary_data_type = 1;
            }
        }break;

        case Y_BOUNDARY:
        {
            if(NULL != dot_y1)
            {
                boundary_num++;
                seekfree_assistant_camera_dot_data.valid_flag |= 1 << 0;
                seekfree_assistant_camera_buffer.boundary_y[i++] = dot_y1;
            }
            if(NULL != dot_y2)
            {
                boundary_num++;
                seekfree_assistant_camera_dot_data.valid_flag |= 1 << 1;
                seekfree_assistant_camera_buffer.boundary_y[i++] = dot_y2;
            }
            if(NULL != dot_y3)
            {
                boundary_num++;
                seekfree_assistant_camera_dot_data.valid_flag |= 1 << 2;
                seekfree_assistant_camera_buffer.boundary_y[i++] = dot_y3;
            }

            if(255 < seekfree_assistant_camera_buffer.width)
            {
                boundary_data_type = 1;
            }
        }break;

        case XY_BOUNDARY:
        {
            if((NULL != dot_x1) && (NULL != dot_y1))
            {
                boundary_num++;
                seekfree_assistant_camera_dot_data.valid_flag |= 1 << 0;
                seekfree_assistant_camera_buffer.boundary_x[i]   = dot_x1;
                seekfree_assistant_camera_buffer.boundary_y[i++] = dot_y1;
            }
            if((NULL != dot_x2) && (NULL != dot_y2))
            {
                boundary_num++;
                seekfree_assistant_camera_dot_data.valid_flag |= 1 << 1;
                seekfree_assistant_camera_buffer.boundary_x[i]   = dot_x2;
                seekfree_assistant_camera_buffer.boundary_y[i++] = dot_y2;
            }
            if((NULL != dot_x3) && (NULL != dot_y3))
            {
                boundary_num++;
                seekfree_assistant_camera_dot_data.valid_flag |= 1 << 2;
                seekfree_assistant_camera_buffer.boundary_x[i]   = dot_x3;
                seekfree_assistant_camera_buffer.boundary_y[i++] = dot_y3;
            }

            if((255 < seekfree_assistant_camera_buffer.width) || (255 < seekfree_assistant_camera_buffer.height))
            {
                boundary_data_type = 1;
            }
        }break;

        case NO_BOUNDARY:break;
    }

    seekfree_assistant_camera_dot_data.dot_type   = (boundary_type << 6) | (boundary_data_type << 5) | boundary_num;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手解析接收到的数据
// 参数说明     void
// 返回参数     void
// 使用示例     函数只需要放到周期运行的PIT中断或者主循环即可
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_data_analysis (void)
{
    uint8  temp_sum;
    uint32 read_length;
    seekfree_assistant_parameter_struct *receive_packet;

    // 这里使用uint32进行定义，目的是为了保证数组四字节对齐
    uint32  temp_buffer[SEEKFREE_ASSISTANT_BUFFER_SIZE / 4];

	uint32 i = 0;
    // 尝试读取数据, 如果不是自定义的传输方式则从接收回调中读取数据
    read_length = seekfree_assistant_receive((uint8 *)temp_buffer, SEEKFREE_ASSISTANT_BUFFER_SIZE);
    if(read_length)
    {
        // 将读取到的数据写入FIFO
        fifo_write_buffer(&seekfree_assistant_fifo, (uint8 *)temp_buffer, read_length);
    }

    while(sizeof(seekfree_assistant_parameter_struct) <= fifo_used(&seekfree_assistant_fifo))
    {
        read_length = sizeof(seekfree_assistant_parameter_struct);
        fifo_read_buffer(&seekfree_assistant_fifo, (uint8 *)temp_buffer, &read_length, FIFO_READ_ONLY);
        if(SEEKFREE_ASSISTANT_RECEIVE_HEAD != ((uint8 *)temp_buffer)[0])
        {
            // 没有帧头则从FIFO中去掉第一个数据
            read_length = 1;
        }
        else
        {

            // 找到帧头
            receive_packet = (seekfree_assistant_parameter_struct *)temp_buffer;
            temp_sum = receive_packet->check_sum;
            receive_packet->check_sum = 0;

            if(temp_sum == seekfree_assistant_sum((uint8 *)temp_buffer, sizeof(seekfree_assistant_parameter_struct)))
            {
                // 和校验成功保存数据
			
				// 由于大小端不匹配，所以需要交换。
				((uint8 *)&seekfree_assistant_parameter[receive_packet->channel - 1])[3] = ((uint8 *)&receive_packet->dat)[0];
				((uint8 *)&seekfree_assistant_parameter[receive_packet->channel - 1])[2] = ((uint8 *)&receive_packet->dat)[1];
				((uint8 *)&seekfree_assistant_parameter[receive_packet->channel - 1])[1] = ((uint8 *)&receive_packet->dat)[2];
				((uint8 *)&seekfree_assistant_parameter[receive_packet->channel - 1])[0] = ((uint8 *)&receive_packet->dat)[3];
				seekfree_assistant_parameter_update_flag[receive_packet->channel - 1] = 1;
            }
            else
            {
                read_length = 1;
            }
        }

        // 丢弃无需使用的数据
        fifo_read_buffer(&seekfree_assistant_fifo, (uint8 *)temp_buffer, &read_length, FIFO_READ_AND_CLEAN);
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手发送摄像头图像
// 参数说明     void
// 返回参数     void
// 使用示例
// 备注信息     在调用图像发送函数之前，请务必调用一次seekfree_assistant_camera_config函数，将对应的参数设置好
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_camera_send (void)
{

    seekfree_assistant_camera_data_send(seekfree_assistant_camera_buffer.camera_type, seekfree_assistant_camera_buffer.image_addr, seekfree_assistant_camera_dot_data.dot_type & 0x0f, seekfree_assistant_camera_buffer.width, seekfree_assistant_camera_buffer.height);

    if(seekfree_assistant_camera_dot_data.dot_type & 0x0f)
    {
        seekfree_assistant_camera_dot_send(&seekfree_assistant_camera_buffer);
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手 初始化
// 参数说明
// 返回参数     void
// 使用示例
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_init ()
{
    fifo_init(&seekfree_assistant_fifo,  FIFO_DATA_8BIT, seekfree_assistant_buffer, SEEKFREE_ASSISTANT_BUFFER_SIZE);
}


