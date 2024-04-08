#include "headfile.h"
/*
 * 系统频率，可查看board.h中的 FOSC 宏定义修改。
 * board.h文件中FOSC的值设置为0,则程序自动设置系统频率为33.1776MHZ
 * 在board_init中,已经将P54引脚设置为复位
 * 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */

#include "motor_driver_boards.h"     // 电机驱动板头文件
/* -----------------------------------电机驱动板连接----------------------------------------
 * 电机驱动板1（X方向轮）接线：
 * PA <--> P60     PB <--> P62
 * A1 <--> P40     A2 <--> P41
 * B1 <--> P43     B2 <--> P42

 * 电机驱动板2（Y方向轮）接线：
 * PA <--> P64     PB <--> P66
 * A1 <--> P44     A2 <--> P45
 * B1 <--> P47     B2 <--> P46
*/

/* -----------------------------------SPI-OLED显示屏----------------------------------------
 *  - 连接屏幕默认使用端口为：
 *    D0 - P2.5(SCLK)
 *    D1 - P2.3(MOSI)
 *    CS - P2.2(CS)
 *    DC - P2.1(DC)
 *    RES - P2.0(RES)
*/

#include "lsm303_sw.h"
/* -----------------------------------LSM303传感器------------------------------------------
 *  - 连接传感器默认使用端口为：
 *    SCL - P32(SCL)
 *    SDA - P33(SDA)
*/

#include "laser_ranging.h"
/* -----------------------------------激光雷达测距------------------------------------------
 *  - X方向激光雷达接线：
 *    TX - P4.3(RX)
 *    RX - P4.4(TX)
 *  - Y方向激光雷达接线：
 *    TX - P4.6(RX)
 *    RX - P4.7(TX)

*/


void gpio_init()
{
    gpio_mode(P4, GPO_PP);
    gpio_mode(P6, GPO_PP);
}

void motor_drivers_pwm_init()
{
    pwm_init(PWMA_CH1P_P60, 30000, 1000);
    pwm_init(PWMA_CH2P_P62, 30000, 1000);
    pwm_init(PWMA_CH3P_P64, 30000, 1000);
    pwm_init(PWMA_CH4P_P66, 30000, 1000);
}

void lsm303_iic_init(void)
{
    iic_init(IIC_4, IIC4_SCL_P32, IIC4_SDA_P33, 19);
    // 需要注意SEEKFREE LIBRARY默认提供的项目文件没有导入iic相关c文件
    // 需要手动在Keil中添加
}

void main()
{   
    float acc_x, acc_y, acc_z;
    float mag_x, mag_y, mag_z;
	board_init();			        // 初始化寄存器,勿删除此句代码。
    gpio_init();                    // 初始化GPIO，使得P4和P6正确输出
    oled_init_spi();                // 初始化OLED显示屏
    // lsm303_iic_init();              // 初始化LSM303_IIC接口
    lsm303_init_all();              // 初始化LSM303传感器
    motor_drivers_pwm_init();	    // 初始化PWM
    while(1)
    {
        oled_p6x8str_spi(0, 0, "LSM303 DATA READ TEST");
        oled_p6x8str_spi(0, 1, "ACC X:");
        oled_p6x8str_spi(0, 2, "ACC Y:");
        oled_p6x8str_spi(0, 3, "ACC Z:");
        acc_x = lsm303_read_acc('x');
        acc_y = lsm303_read_acc('y');
        acc_z = lsm303_read_acc('z');
        oled_printf_float_spi(6*7, 1, acc_x, 4, 6);
        oled_printf_float_spi(6*7, 2, acc_y, 4, 6);
        oled_printf_float_spi(6*7, 3, acc_z, 4, 6);
        mag_x = lsm303_read_mag('x');
        mag_y = lsm303_read_mag('y');
        mag_z = lsm303_read_mag('z');
        oled_p6x8str_spi(0, 5, "MAG X:");
        oled_p6x8str_spi(0, 6, "MAG Y:");
        oled_p6x8str_spi(0, 7, "MAG Z:");
        oled_printf_float_spi(6*7, 5, mag_x, 4, 6);
        oled_printf_float_spi(6*7, 6, mag_y, 4, 6);
        oled_printf_float_spi(6*7, 7, mag_z, 4, 6);
        delay_ms(100);
        // wheels_x_forward();
        // wheels_y_forward();
        // delay_ms(255);
        // pwm_duty(PWMA_CH1P_P60, 2000);
        // pwm_duty(PWMA_CH2P_P62, 2000);
        // pwm_duty(PWMA_CH3P_P64, 2000);
        // pwm_duty(PWMA_CH4P_P66, 2000);
        // delay_ms(255);
        // wheels_x_backward();
        // wheels_y_backward();
        // delay_ms(255);
        // pwm_duty(PWMA_CH1P_P60, 5000);
        // pwm_duty(PWMA_CH2P_P62, 5000);
        // pwm_duty(PWMA_CH3P_P64, 5000);
        // pwm_duty(PWMA_CH4P_P66, 5000);
        // delay_ms(255);
        // wheels_all_stop();
        // delay_ms(255);
    }
}


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