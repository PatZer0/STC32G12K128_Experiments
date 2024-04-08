/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		seekfree_assistant
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/
 

#include "seekfree_assistant.h"

static seekfree_assistant_camera_struct         seekfree_assistant_camera_data;   
seekfree_assistant_oscilloscope_struct          seekfree_assistant_oscilloscope_data;                         // ����ʾ��������
static seekfree_assistant_camera_dot_struct      seekfree_assistant_camera_dot_data;                          // ͼ����λ�����Э������
static seekfree_assistant_camera_buffer_struct   seekfree_assistant_camera_buffer;                            // ͼ���Լ��߽绺������Ϣ

static fifo_struct      xdata seekfree_assistant_fifo;
static uint8            xdata seekfree_assistant_buffer[SEEKFREE_ASSISTANT_BUFFER_SIZE];              		// ���ݴ������
float                   xdata seekfree_assistant_parameter[SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT] = {0};    	// ������յ��Ĳ���
vuint8					xdata seekfree_assistant_parameter_update_flag[SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT] = {0};
//-------------------------------------------------------------------------------------------------------------------
// �������     ���������ͺ���
// ����˵��     *buffer         ��ҪУ������ݵ�ַ
// ����˵��     length          У�鳤��
// ���ز���     uint8           ��ֵ
// ʹ��ʾ��
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
// �������     ������� ͼ���ͺ���
// ����˵��     camera_type     ����ͷ����
// ����˵��     *image_addr     ͼ���׵�ַ
// ����˵��     boundary_num    ͼ���а����߽�����
// ����˵��     width           ͼ����
// ����˵��     height          ͼ��߶�
// ���ز���     void
// ʹ��ʾ��
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_camera_data_send (seekfree_assistant_image_type_enum camera_type, void *image_addr, uint8 boundary_num, uint16 width, uint16 height)
{
    uint32 image_size = 0;

    seekfree_assistant_camera_data.head           = SEEKFREE_ASSISTANT_SEND_HEAD;
    seekfree_assistant_camera_data.function       = SEEKFREE_ASSISTANT_CAMERA_FUNCTION;
    seekfree_assistant_camera_data.camera_type    = (camera_type << 5) | ((image_addr != NULL ? 0 : 1) << 4) | boundary_num;
    // д���������Ϣ��������Э�鲿��
    seekfree_assistant_camera_data.length         = sizeof(seekfree_assistant_camera_struct);
    seekfree_assistant_camera_data.image_width    = ((((uint16)width << 8)&0xFF00) | ((uint16)width >> 8));
    seekfree_assistant_camera_data.image_height   = ((((uint16)height << 8)&0xFF00) | ((uint16)height >> 8));

    // ���ȷ���֡ͷ�����ܡ�����ͷ���͡��Լ���ȸ߶ȵ���Ϣ
    seekfree_assistant_transfer((const uint8 *)&seekfree_assistant_camera_data, sizeof(seekfree_assistant_camera_struct));

    // ��������ͷ���ͼ���ͼ���С
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

    // ����ͼ������
    if(NULL != image_addr)
    {
        seekfree_assistant_transfer(image_addr, image_size);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �������ͼ����Ϣ���ú���
// ����˵��     camera_type     ͼ������
// ����˵��     image_addr      ͼ���ַ    �������NULL�������ʾֻ���ͱ�����Ϣ����λ��
// ����˵��     width           ͼ����
// ����˵��     height          ͼ��߶�
// ���ز���     void
// ʹ��ʾ��                     seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_camera_information_config (seekfree_assistant_image_type_enum camera_type, void *image_addr, uint16 width, uint16 height)
{
    seekfree_assistant_camera_dot_data.head       = SEEKFREE_ASSISTANT_SEND_HEAD;
    seekfree_assistant_camera_dot_data.function   = SEEKFREE_ASSISTANT_CAMERA_DOT_FUNCTION;
    // д���������Ϣ
    seekfree_assistant_camera_dot_data.length     = sizeof(seekfree_assistant_camera_dot_struct);

    seekfree_assistant_camera_buffer.camera_type  = camera_type;
    seekfree_assistant_camera_buffer.image_addr   = image_addr;
    seekfree_assistant_camera_buffer.width        = width;
    seekfree_assistant_camera_buffer.height       = height;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������� ����ʾ�������ͺ���
// ����˵��     *seekfree_assistant_oscilloscope  ʾ�������ݽṹ��
// ���ز���     void
// ʹ��ʾ��     seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_oscilloscope_send (seekfree_assistant_oscilloscope_struct *seekfree_assistant_oscilloscope)
{
    uint8 packet_size;
	uint8 i = 0;
	seekfree_assistant_oscilloscope_struct temp_oscilloscope;
	
	temp_oscilloscope.channel_num = seekfree_assistant_oscilloscope->channel_num & 0x0f;
	
	// ���ڴ�С�˲�ƥ�䣬������Ҫ������
	for(i = 0; i < temp_oscilloscope.channel_num; i++)
	{
		((uint8 *)&temp_oscilloscope.dat[i])[0] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[3];
		((uint8 *)&temp_oscilloscope.dat[i])[1] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[2];
		((uint8 *)&temp_oscilloscope.dat[i])[2] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[1];
		((uint8 *)&temp_oscilloscope.dat[i])[3] = ((uint8 *)&seekfree_assistant_oscilloscope->dat[i])[0];
	}

    // ֡ͷ
    temp_oscilloscope.head         = SEEKFREE_ASSISTANT_SEND_HEAD;

    // д���������Ϣ
    packet_size                     = sizeof(temp_oscilloscope) - (SEEKFREE_ASSISTANT_SET_OSCILLOSCOPE_COUNT - temp_oscilloscope.channel_num) * 4;
    temp_oscilloscope.length       = packet_size;

    // д�빦������ͨ������
    temp_oscilloscope.channel_num |= SEEKFREE_ASSISTANT_CAMERA_OSCILLOSCOPE;

    // ��У�����
    temp_oscilloscope.check_sum    = 0;
    temp_oscilloscope.check_sum    = seekfree_assistant_sum((uint8 *)&temp_oscilloscope, packet_size);

    // �����ڵ��ñ�����֮ǰ�����û�����Ҫ���͵�����д��temp_oscilloscope.data[]

    seekfree_assistant_transfer((const uint8 *)&temp_oscilloscope, packet_size);
}

void seekfree_assistant_camera_dot_send (seekfree_assistant_camera_buffer_struct *buffer)
{
    uint8  i;
    uint16 dot_bytes = 0;   // ���ֽ�����

    dot_bytes = seekfree_assistant_camera_dot_data.dot_num;
	dot_bytes = ((((uint16)dot_bytes << 8)&0xFF00) | ((uint16)dot_bytes >> 8));
    if(seekfree_assistant_camera_dot_data.dot_type & (1 << 5))
    {
        dot_bytes *= 2;
    }

    // ���ȷ���֡ͷ�����ܡ��߽��š����곤�ȡ������
    seekfree_assistant_transfer((const uint8 *)&seekfree_assistant_camera_dot_data, sizeof(seekfree_assistant_camera_dot_struct));

    for(i=0; i < SEEKFREE_ASSISTANT_CAMERA_MAX_BOUNDARY; i++)
    {
        // �ж��Ƿ��ͺ���������
        if(NULL != buffer->boundary_x[i])
        {
            seekfree_assistant_transfer((const uint8 *)buffer->boundary_x[i], dot_bytes);
        }

        // �ж��Ƿ�������������
        if(NULL != buffer->boundary_y[i])
        {
            // ���û�����������ݣ����ʾÿһ��ֻ��һ���߽�
            // ָ���˺����������ݣ����ַ�ʽ����ʵ��ͬһ�ж���߽����������������㷨�ܹ����������䡣
            seekfree_assistant_transfer((const uint8 *)buffer->boundary_y[i], dot_bytes);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �������ͼ����߷������ú���
// ����˵��     boundary_type   �߽�����
// ����˵��     dot_num         һ���߽��ж��ٸ���
// ����˵��     dot_x1          ��ű���1������ĵ�ַ  �������NULL�������ʾ�����ͱ���1
// ����˵��     dot_x2          ��ű���2������ĵ�ַ  �������NULL�������ʾ�����ͱ���2
// ����˵��     dot_x3          ��ű���3������ĵ�ַ  �������NULL�������ʾ�����ͱ���3
// ����˵��     dot_y1          ��ű���1������ĵ�ַ  �������NULL�������ʾ�����ͱ���1
// ����˵��     dot_y2          ��ű���2������ĵ�ַ  �������NULL�������ʾ�����ͱ���2
// ����˵��     dot_y3          ��ű���3������ĵ�ַ  �������NULL�������ʾ�����ͱ���3
// ���ز���     void
// ʹ��ʾ��                     seekfree_assistant_camera_config(X_BOUNDARY, MT9V03X_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL, NULL);     // ͼ����ʱ�����������ߣ�����ֻ�к�����
// ʹ��ʾ��                     seekfree_assistant_camera_config(Y_BOUNDARY, MT9V03X_W, NULL, NULL, NULL, y1_boundary, y2_boundary, y3_boundary);     // ͼ����ʱ�����������ߣ�����ֻ��������
// ʹ��ʾ��                     seekfree_assistant_camera_config(XY_BOUNDARY, 160, xy_x1_boundary, xy_x2_boundary, xy_x3_boundary, xy_y1_boundary, xy_y2_boundary, xy_y3_boundary);   // ͼ����ʱ�����������ߣ����߰�����������
// ��ע��Ϣ
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
// �������     ������ֽ������յ�������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     ����ֻ��Ҫ�ŵ��������е�PIT�жϻ�����ѭ������
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_data_analysis (void)
{
    uint8  temp_sum;
    uint32 read_length;
    seekfree_assistant_parameter_struct *receive_packet;

    // ����ʹ��uint32���ж��壬Ŀ����Ϊ�˱�֤�������ֽڶ���
    uint32  temp_buffer[SEEKFREE_ASSISTANT_BUFFER_SIZE / 4];

	uint32 i = 0;
    // ���Զ�ȡ����, ��������Զ���Ĵ��䷽ʽ��ӽ��ջص��ж�ȡ����
    read_length = seekfree_assistant_receive((uint8 *)temp_buffer, SEEKFREE_ASSISTANT_BUFFER_SIZE);
    if(read_length)
    {
        // ����ȡ��������д��FIFO
        fifo_write_buffer(&seekfree_assistant_fifo, (uint8 *)temp_buffer, read_length);
    }

    while(sizeof(seekfree_assistant_parameter_struct) <= fifo_used(&seekfree_assistant_fifo))
    {
        read_length = sizeof(seekfree_assistant_parameter_struct);
        fifo_read_buffer(&seekfree_assistant_fifo, (uint8 *)temp_buffer, &read_length, FIFO_READ_ONLY);
        if(SEEKFREE_ASSISTANT_RECEIVE_HEAD != ((uint8 *)temp_buffer)[0])
        {
            // û��֡ͷ���FIFO��ȥ����һ������
            read_length = 1;
        }
        else
        {

            // �ҵ�֡ͷ
            receive_packet = (seekfree_assistant_parameter_struct *)temp_buffer;
            temp_sum = receive_packet->check_sum;
            receive_packet->check_sum = 0;

            if(temp_sum == seekfree_assistant_sum((uint8 *)temp_buffer, sizeof(seekfree_assistant_parameter_struct)))
            {
                // ��У��ɹ���������
			
				// ���ڴ�С�˲�ƥ�䣬������Ҫ������
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

        // ��������ʹ�õ�����
        fifo_read_buffer(&seekfree_assistant_fifo, (uint8 *)temp_buffer, &read_length, FIFO_READ_AND_CLEAN);
    }
}
//-------------------------------------------------------------------------------------------------------------------
// �������     ������ַ�������ͷͼ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��
// ��ע��Ϣ     �ڵ���ͼ���ͺ���֮ǰ������ص���һ��seekfree_assistant_camera_config����������Ӧ�Ĳ������ú�
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
// �������     ������� ��ʼ��
// ����˵��
// ���ز���     void
// ʹ��ʾ��
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_init ()
{
    fifo_init(&seekfree_assistant_fifo,  FIFO_DATA_8BIT, seekfree_assistant_buffer, SEEKFREE_ASSISTANT_BUFFER_SIZE);
}


