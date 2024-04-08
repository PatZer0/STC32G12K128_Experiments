/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"


/*
 * 系统频率，可查看board.h中的 FOSC 宏定义修改。
 * board.h文件中FOSC的值设置为0,则程序自动设置系统频率为33.1776MHZ
 * 在board_init中,已经将P54引脚设置为复位
 * 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */

#include <STC32G.H>
// #include "STC32G_PWM.h"
#include "zf_pwm.h"

/*
    一共有2块电机驱动板，分别为 Driver_Board1 和 Driver_Board2
    Driver_Board1_A 为 X 方向前轮，Driver_Board1_B 为 X 方向后轮
    Driver_Board2_A 为 Y 方向前轮，Driver_Board2_B 为 Y 方向后轮

    电机驱动板1（X方向轮）接线：
    PA <--> P60     PB <--> P62
    A1 <--> P40     A2 <--> P41
    B1 <--> P42     B2 <--> P43

    电机驱动板2（Y方向轮）接线：
    PA <--> P64     PB <--> P66
    A1 <--> P44     A2 <--> P45
    B1 <--> P46     B2 <--> P47
*/

// Driver_Board1
sbit x_front_wheel_clockwise = P4^0;        // X 方向前轮顺时针转动信号
sbit x_front_wheel_anticlockwise = P4^1;    // X 方向前轮逆时针转动信号
sbit x_rear_wheel_clockwise = P4^3;         // X 方向后轮顺时针转动信号
sbit x_rear_wheel_anticlockwise = P4^2;     // X 方向后轮逆时针转动信号

// Driver_Board2
sbit y_front_wheel_clockwise = P4^4;        // Y 方向前轮顺时针转动信号
sbit y_front_wheel_anticlockwise = P4^5;    // Y 方向前轮逆时针转动信号
sbit y_rear_wheel_clockwise = P4^7;         // Y 方向后轮顺时针转动信号
sbit y_rear_wheel_anticlockwise = P4^6;     // Y 方向后轮逆时针转动信号

// PWN 初始化配置
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
//     PWM1_Config.PWM_Period = 10000;              // 周期为1000
//     PWM1_Config.PWM_Duty = 5000;                 // 占空比为50%
//     PWM1_Config.PWM_DeadTime = 122;               // 没有死区时间
//     PWM1_Config.PWM_EnoSelect = ENO1P;          // 选择输出通道ENO1P
//     PWM1_Config.PWM_CEN_Enable = ENABLE;        // 使能计数器
//     PWM1_Config.PWM_MainOutEnable = ENABLE;     // 使能主输出

//     PWM2_Config.PWM_Mode = CCMRn_PWM_MODE1;
//     PWM2_Config.PWM_Period = 10000;              // 周期为1000
//     PWM2_Config.PWM_Duty = 5000;                 // 占空比为50%
//     PWM2_Config.PWM_DeadTime = 122;               // 没有死区时间
//     PWM2_Config.PWM_EnoSelect = ENO2P;          // 选择输出通道ENO1P
//     PWM2_Config.PWM_CEN_Enable = ENABLE;        // 使能计数器
//     PWM2_Config.PWM_MainOutEnable = ENABLE;     // 使能主输出

//     PWM3_Config.PWM_Mode = CCMRn_PWM_MODE1;
//     PWM3_Config.PWM_Period = 10000;              // 周期为1000
//     PWM3_Config.PWM_Duty = 5000;                 // 占空比为50%
//     PWM3_Config.PWM_DeadTime = 122;               // 没有死区时间
//     PWM3_Config.PWM_EnoSelect = ENO3P;          // 选择输出通道ENO1P
//     PWM3_Config.PWM_CEN_Enable = ENABLE;        // 使能计数器
//     PWM3_Config.PWM_MainOutEnable = ENABLE;     // 使能主输出

//     PWM4_Config.PWM_Mode = CCMRn_PWM_MODE1;
//     PWM4_Config.PWM_Period = 10000;              // 周期为1000
//     PWM4_Config.PWM_Duty = 5000;                 // 占空比为50%
//     PWM4_Config.PWM_DeadTime = 122;               // 没有死区时间
//     PWM4_Config.PWM_EnoSelect = ENO4P;          // 选择输出通道ENO1P
//     PWM4_Config.PWM_CEN_Enable = ENABLE;        // 使能计数器
//     PWM4_Config.PWM_MainOutEnable = ENABLE;     // 使能主输出

//     PWM_Configuration(PWM1, &PWM1_Config); // 配置第一个PWM通道
//     PWM_Configuration(PWM2, &PWM2_Config); // 配置第二个PWM通道
//     PWM_Configuration(PWM3, &PWM3_Config); // 配置第三个PWM通道
//     PWM_Configuration(PWM4, &PWM4_Config); // 配置第四个PWM通道
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
//     // 延时函数，单位为 ms，主频为24MHz
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