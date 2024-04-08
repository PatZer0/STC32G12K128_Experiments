/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		fifo
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32F12K
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/



#ifndef __ZF_FIFO_H
#define __ZF_FIFO_H


typedef enum
{
    FIFO_SUCCESS,                                                               // FIFO �����ɹ�

    FIFO_RESET_UNDO,                                                            // FIFO ���ò���δִ��
    FIFO_CLEAR_UNDO,                                                            // FIFO ��ղ���δִ��
    FIFO_BUFFER_NULL,                                                           // FIFO �û��������쳣
    FIFO_WRITE_UNDO,                                                            // FIFO д�����δִ��
    FIFO_SPACE_NO_ENOUGH,                                                       // FIFO д����� �������ռ䲻��
    FIFO_READ_UNDO,                                                             // FIFO ��ȡ����δִ��
    FIFO_DATA_NO_ENOUGH,                                                        // FIFO ��ȡ���� ���ݳ��Ȳ���
}fifo_state_enum;                                                               // FIFO �������

// �����߼�
// �������ò���   ����ǿ����� FIFO ����ʹ��
// ����д�����   �����������Լ�д�����ʱ����
// ˳���ȡ����   ��������պ����ò���ʱ����
// β����ȡ����   ��������պ������Լ�д�����ʱ����
// ��ȡ��ղ���   ��������պ������Լ���ȡ����ʱ����
// ����Ϊ�˷�ֹ�ж�Ƕ�׵������ݻ���
typedef enum
{
    FIFO_IDLE       = 0x00,                                                     // ����״̬

    FIFO_RESET      = 0x01,                                                     // ����ִ�����û�����
    FIFO_CLEAR      = 0x02,                                                     // ����ִ����ջ�����
    FIFO_WRITE      = 0x04,                                                     // ����ִ��д�뻺����
    FIFO_READ       = 0x08,                                                     // ����ִ�ж�ȡ������
}fifo_execution_enum;                                                           // FIFO ����״̬ ΪǶ��ʹ��Ԥ�� �޷���ȫ���������

typedef enum
{
    FIFO_READ_AND_CLEAN,                                                        // FIFO ������ģʽ ��ȡ������ͷŶ�Ӧ������
    FIFO_READ_ONLY,                                                             // FIFO ������ģʽ ����ȡ
}fifo_operation_enum;

typedef enum
{
    FIFO_DATA_8BIT,                                                             // FIFO ����λ�� 8bit
    FIFO_DATA_16BIT,                                                            // FIFO ����λ�� 16bit
    FIFO_DATA_32BIT,                                                            // FIFO ����λ�� 32bit
}fifo_data_type_enum;

typedef struct 
{
    uint8               execution;                                              // ִ�в���
    fifo_data_type_enum type;                                                   // ��������
    void                *buffer;                                                // ����ָ��
    uint32              head;                                                   // ����ͷָ�� ����ָ��յĻ���
    uint32              end;                                                    // ����βָ�� ����ָ��ǿջ��棨����ȫ�ճ��⣩
    uint32              reamin_size;                                            // ����ʣ���С
    uint32              max;                                                    // �����ܴ�С
}fifo_struct;

fifo_state_enum fifo_clear              (fifo_struct *fifo);
uint32          fifo_used               (fifo_struct *fifo);

fifo_state_enum fifo_write_element      (fifo_struct *fifo, uint32 dat);
fifo_state_enum fifo_write_buffer       (fifo_struct *fifo, void *dat, uint32 length);
fifo_state_enum fifo_read_element       (fifo_struct *fifo, void *dat, fifo_operation_enum flag);
fifo_state_enum fifo_read_buffer        (fifo_struct *fifo, void *dat, uint32 *length, fifo_operation_enum flag);
fifo_state_enum fifo_read_tail_buffer   (fifo_struct *fifo, void *dat, uint32 *length, fifo_operation_enum flag);

fifo_state_enum fifo_init               (fifo_struct *fifo, fifo_data_type_enum type, void *buffer_addr, uint32 len);


#endif


