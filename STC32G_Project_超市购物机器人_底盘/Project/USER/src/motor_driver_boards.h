#ifndef MOTOR_DRIVER_BOARDS_H
#define MOTOR_DRIVER_BOARDS_H

#include "headfile.h"

// ���������1��X����ǰ�֡�����
sbit gpio_wheel_x_front_clockwise           = P4^0;     // X ����ǰ��˳ʱ��ת���ź�
sbit gpio_wheel_x_front_counterclockwise    = P4^1;     // X ����ǰ����ʱ��ת���ź�
sbit gpio_wheel_x_rear_clockwise            = P4^3;     // X �������˳ʱ��ת���ź�
sbit gpio_wheel_x_rear_counterclockwise     = P4^2;     // X ���������ʱ��ת���ź�

// ���������2��Y����ǰ�֡�����
sbit gpio_wheel_y_front_clockwise           = P4^4;     // Y ����ǰ��˳ʱ��ת���ź�
sbit gpio_wheel_y_front_counterclockwise    = P4^5;     // Y ����ǰ����ʱ��ת���ź�
sbit gpio_wheel_y_rear_clockwise            = P4^7;     // Y �������˳ʱ��ת���ź�
sbit gpio_wheel_y_rear_counterclockwise     = P4^6;     // Y ���������ʱ��ת���ź�

// ��ʼ�����������
// void motor_drivers_pwm_init();      // ��ʼ�����������
void wheel_x_front_forward();       // X ����ǰ����ת
void wheel_x_front_backward();      // X ����ǰ�ַ�ת
void wheel_x_front_stop();          // X ����ǰ��ֹͣ
void wheel_x_rear_forward();        // X ���������ת
void wheel_x_rear_backward();       // X ������ַ�ת
void wheel_x_rear_stop();           // X �������ֹͣ
void wheels_x_forward();            // X ��������������ת
void wheels_x_backward();           // X �����������ӷ�ת
void wheels_x_stop();               // X ������������ֹͣ
void wheel_y_front_forward();       // Y ����ǰ����ת
void wheel_y_front_backward();      // Y ����ǰ�ַ�ת
void wheel_y_front_stop();          // Y ����ǰ��ֹͣ
void wheel_y_rear_forward();        // Y ���������ת
void wheel_y_rear_backward();       // Y ������ַ�ת
void wheel_y_rear_stop();           // Y �������ֹͣ
void wheels_y_forward();            // Y ��������������ת
void wheels_y_backward();           // Y �����������ӷ�ת
void wheels_y_stop();               // Y ������������ֹͣ
void wheels_all_forward();          // ����������ת
void wheels_all_backward();         // �������ӷ�ת
void wheels_all_stop();             // ��������ֹͣ

#endif // MOTOR_DRIVER_BOARDS_H
