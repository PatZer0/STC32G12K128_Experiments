/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"


/*
 * ϵͳƵ�ʣ��ɲ鿴board.h�е� FOSC �궨���޸ġ�
 * board.h�ļ���FOSC��ֵ����Ϊ0,������Զ�����ϵͳƵ��Ϊ33.1776MHZ
 * ��board_init��,�Ѿ���P54��������Ϊ��λ
 * �����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
 */

#include <STC32G.H>
// #include "STC32G_PWM.h"
#include "zf_pwm.h"

/*
    һ����2���������壬�ֱ�Ϊ Driver_Board1 �� Driver_Board2
    Driver_Board1_A Ϊ X ����ǰ�֣�Driver_Board1_B Ϊ X �������
    Driver_Board2_A Ϊ Y ����ǰ�֣�Driver_Board2_B Ϊ Y �������

    ���������1��X�����֣����ߣ�
    PA <--> P60     PB <--> P62
    A1 <--> P40     A2 <--> P41
    B1 <--> P42     B2 <--> P43

    ���������2��Y�����֣����ߣ�
    PA <--> P64     PB <--> P66
    A1 <--> P44     A2 <--> P45
    B1 <--> P46     B2 <--> P47
*/

// Driver_Board1
sbit x_front_wheel_clockwise = P4^0;        // X ����ǰ��˳ʱ��ת���ź�
sbit x_front_wheel_anticlockwise = P4^1;    // X ����ǰ����ʱ��ת���ź�
sbit x_rear_wheel_clockwise = P4^3;         // X �������˳ʱ��ת���ź�
sbit x_rear_wheel_anticlockwise = P4^2;     // X ���������ʱ��ת���ź�

// Driver_Board2
sbit y_front_wheel_clockwise = P4^4;        // Y ����ǰ��˳ʱ��ת���ź�
sbit y_front_wheel_anticlockwise = P4^5;    // Y ����ǰ����ʱ��ת���ź�
sbit y_rear_wheel_clockwise = P4^7;         // Y �������˳ʱ��ת���ź�
sbit y_rear_wheel_anticlockwise = P4^6;     // Y ���������ʱ��ת���ź�

// PWN ��ʼ������
// void PWM_Init()
// {    
//     PWMx_InitDefine PWM1_Config;
//     PWMx_InitDefine PWM2_Config;
//     PWMx_InitDefine PWM3_Config;
//     PWMx_InitDefine PWM4_Config;

//     PWM1_USE_P60P61();
//     PWM2_USE_P62P63();
//     PWM3_USE_P64P65();
//     PWM4_USE_P66P67();

//     PWM1_Config.PWM_Mode = CCMRn_PWM_MODE1;
//     PWM1_Config.PWM_Period = 10000;              // ����Ϊ1000
//     PWM1_Config.PWM_Duty = 5000;                 // ռ�ձ�Ϊ50%
//     PWM1_Config.PWM_DeadTime = 122;               // û������ʱ��
//     PWM1_Config.PWM_EnoSelect = ENO1P;          // ѡ�����ͨ��ENO1P
//     PWM1_Config.PWM_CEN_Enable = ENABLE;        // ʹ�ܼ�����
//     PWM1_Config.PWM_MainOutEnable = ENABLE;     // ʹ�������

//     PWM2_Config.PWM_Mode = CCMRn_PWM_MODE1;
//     PWM2_Config.PWM_Period = 10000;              // ����Ϊ1000
//     PWM2_Config.PWM_Duty = 5000;                 // ռ�ձ�Ϊ50%
//     PWM2_Config.PWM_DeadTime = 122;               // û������ʱ��
//     PWM2_Config.PWM_EnoSelect = ENO2P;          // ѡ�����ͨ��ENO1P
//     PWM2_Config.PWM_CEN_Enable = ENABLE;        // ʹ�ܼ�����
//     PWM2_Config.PWM_MainOutEnable = ENABLE;     // ʹ�������

//     PWM3_Config.PWM_Mode = CCMRn_PWM_MODE1;
//     PWM3_Config.PWM_Period = 10000;              // ����Ϊ1000
//     PWM3_Config.PWM_Duty = 5000;                 // ռ�ձ�Ϊ50%
//     PWM3_Config.PWM_DeadTime = 122;               // û������ʱ��
//     PWM3_Config.PWM_EnoSelect = ENO3P;          // ѡ�����ͨ��ENO1P
//     PWM3_Config.PWM_CEN_Enable = ENABLE;        // ʹ�ܼ�����
//     PWM3_Config.PWM_MainOutEnable = ENABLE;     // ʹ�������

//     PWM4_Config.PWM_Mode = CCMRn_PWM_MODE1;
//     PWM4_Config.PWM_Period = 10000;              // ����Ϊ1000
//     PWM4_Config.PWM_Duty = 5000;                 // ռ�ձ�Ϊ50%
//     PWM4_Config.PWM_DeadTime = 122;               // û������ʱ��
//     PWM4_Config.PWM_EnoSelect = ENO4P;          // ѡ�����ͨ��ENO1P
//     PWM4_Config.PWM_CEN_Enable = ENABLE;        // ʹ�ܼ�����
//     PWM4_Config.PWM_MainOutEnable = ENABLE;     // ʹ�������

//     PWM_Configuration(PWM1, &PWM1_Config); // ���õ�һ��PWMͨ��
//     PWM_Configuration(PWM2, &PWM2_Config); // ���õڶ���PWMͨ��
//     PWM_Configuration(PWM3, &PWM3_Config); // ���õ�����PWMͨ��
//     PWM_Configuration(PWM4, &PWM4_Config); // ���õ��ĸ�PWMͨ��
// }

void PWM_Init_ZF()
{
    pwm_init(PWMA_CH1P_P60, 30000, 1000);
    pwm_init(PWMA_CH2P_P62, 30000, 1000);
    pwm_init(PWMA_CH3P_P64, 30000, 1000);
    pwm_init(PWMA_CH4P_P66, 30000, 1000);
}

void GPIO_Init()
{
    P6M0 = 0x00; P6M1 = 0x00; 
    P4M0 = 0xff; P4M1 = 0x00; 
}

// void delay_ms(unsigned int t)
// {
//     // ��ʱ��������λΪ ms����ƵΪ24MHz
//     unsigned int i, j;
//     for(i=0; i<t; i++)
//     {
//         for(j=0; j<12000; j++)
//         {
//             ;
//         }
//     }
// }

void X_Front_Wheel_Forward()
{
    x_front_wheel_clockwise = 1;
    x_front_wheel_anticlockwise = 0;
}

void X_Front_Wheel_Backward()
{
    x_front_wheel_clockwise = 0;
    x_front_wheel_anticlockwise = 1;
}

void X_Front_Wheel_Stop()
{
    x_front_wheel_clockwise = 0;
    x_front_wheel_anticlockwise = 0;
}

void X_Rear_Wheel_Forward()
{
    x_rear_wheel_clockwise = 1;
    x_rear_wheel_anticlockwise = 0;
}

void X_Rear_Wheel_Backward()
{
    x_rear_wheel_clockwise = 0;
    x_rear_wheel_anticlockwise = 1;
}

void X_Rear_Wheel_Stop()
{
    x_rear_wheel_clockwise = 0;
    x_rear_wheel_anticlockwise = 0;
}

void Y_Front_Wheel_Forward()
{
    y_front_wheel_clockwise = 1;
    y_front_wheel_anticlockwise = 0;
}

void Y_Front_Wheel_Backward()
{
    y_front_wheel_clockwise = 0;
    y_front_wheel_anticlockwise = 1;
}

void Y_Front_Wheel_Stop()
{
    y_front_wheel_clockwise = 0;
    y_front_wheel_anticlockwise = 0;
}

void Y_Rear_Wheel_Forward()
{
    y_rear_wheel_clockwise = 1;
    y_rear_wheel_anticlockwise = 0;
}

void Y_Rear_Wheel_Backward()
{
    y_rear_wheel_clockwise = 0;
    y_rear_wheel_anticlockwise = 1;
}

void Y_Rear_Wheel_Stop()
{
    y_rear_wheel_clockwise = 0;
    y_rear_wheel_anticlockwise = 0;
}

void X_Wheels_Forward()
{
    X_Front_Wheel_Forward();
    X_Rear_Wheel_Forward();
}

void X_Wheels_Backward()
{
    X_Front_Wheel_Backward();
    X_Rear_Wheel_Backward();
}

void X_Wheels_Stop()
{
    X_Front_Wheel_Stop();
    X_Rear_Wheel_Stop();
}

void Y_Wheels_Forward()
{
    Y_Front_Wheel_Forward();
    Y_Rear_Wheel_Forward();
}

void Y_Wheels_Backward()
{
    Y_Front_Wheel_Backward();
    Y_Rear_Wheel_Backward();
}

void Y_Wheels_Stop()
{
    Y_Front_Wheel_Stop();
    Y_Rear_Wheel_Stop();
}

void All_Wheels_Stop()
{
    X_Wheels_Stop();
    Y_Wheels_Stop();
}

void main()
{   
    GPIO_Init();
    // PWM_Init();
    PWM_Init_ZF();
    while(1)
    {
        X_Wheels_Forward();
        Y_Wheels_Forward();
        delay_ms(255);
        pwm_duty(PWMA_CH1P_P60, 2000);
        pwm_duty(PWMA_CH2P_P62, 2000);
        pwm_duty(PWMA_CH3P_P64, 2000);
        pwm_duty(PWMA_CH4P_P66, 2000);
        delay_ms(255);
        X_Wheels_Backward();
        Y_Wheels_Backward();
        delay_ms(255);
        pwm_duty(PWMA_CH1P_P60, 5000);
        pwm_duty(PWMA_CH2P_P62, 5000);
        pwm_duty(PWMA_CH3P_P64, 5000);
        pwm_duty(PWMA_CH4P_P66, 5000);
        delay_ms(255);
        X_Wheels_Stop();
        Y_Wheels_Stop();
        delay_ms(255);
        // All_Wheels_Stop();
    }
}