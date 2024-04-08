#ifndef MOTOR_DRIVER_BOARDS_H
#define MOTOR_DRIVER_BOARDS_H

#include "headfile.h"

// 电机驱动板1：X方向前轮、后轮
sbit gpio_wheel_x_front_clockwise           = P4^0;     // X 方向前轮顺时针转动信号
sbit gpio_wheel_x_front_counterclockwise    = P4^1;     // X 方向前轮逆时针转动信号
sbit gpio_wheel_x_rear_clockwise            = P4^3;     // X 方向后轮顺时针转动信号
sbit gpio_wheel_x_rear_counterclockwise     = P4^2;     // X 方向后轮逆时针转动信号

// 电机驱动板2：Y方向前轮、后轮
sbit gpio_wheel_y_front_clockwise           = P4^4;     // Y 方向前轮顺时针转动信号
sbit gpio_wheel_y_front_counterclockwise    = P4^5;     // Y 方向前轮逆时针转动信号
sbit gpio_wheel_y_rear_clockwise            = P4^7;     // Y 方向后轮顺时针转动信号
sbit gpio_wheel_y_rear_counterclockwise     = P4^6;     // Y 方向后轮逆时针转动信号

// 初始化电机驱动板
// void motor_drivers_pwm_init();      // 初始化电机驱动板
void wheel_x_front_forward();       // X 方向前轮正转
void wheel_x_front_backward();      // X 方向前轮反转
void wheel_x_front_stop();          // X 方向前轮停止
void wheel_x_rear_forward();        // X 方向后轮正转
void wheel_x_rear_backward();       // X 方向后轮反转
void wheel_x_rear_stop();           // X 方向后轮停止
void wheels_x_forward();            // X 方向所有轮子正转
void wheels_x_backward();           // X 方向所有轮子反转
void wheels_x_stop();               // X 方向所有轮子停止
void wheel_y_front_forward();       // Y 方向前轮正转
void wheel_y_front_backward();      // Y 方向前轮反转
void wheel_y_front_stop();          // Y 方向前轮停止
void wheel_y_rear_forward();        // Y 方向后轮正转
void wheel_y_rear_backward();       // Y 方向后轮反转
void wheel_y_rear_stop();           // Y 方向后轮停止
void wheels_y_forward();            // Y 方向所有轮子正转
void wheels_y_backward();           // Y 方向所有轮子反转
void wheels_y_stop();               // Y 方向所有轮子停止
void wheels_all_forward();          // 所有轮子正转
void wheels_all_backward();         // 所有轮子反转
void wheels_all_stop();             // 所有轮子停止

#endif // MOTOR_DRIVER_BOARDS_H
