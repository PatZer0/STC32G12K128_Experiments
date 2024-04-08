#include "motor_driver_boards.h"

// 使用时建议将此函数复制到main.c中，并在初始化函数中调用
// 这样修改引脚只需要修改main.c即可
// 如果需要使用，记得要去.h中取消注释
// void motor_drivers_pwm_init()
// {
//     pwm_init(PWMA_CH1P_P60, 30000, 1000);
//     pwm_init(PWMA_CH2P_P62, 30000, 1000);
//     pwm_init(PWMA_CH3P_P64, 30000, 1000);
//     pwm_init(PWMA_CH4P_P66, 30000, 1000);
// }

void wheel_x_front_forward()
{
    // X 方向前轮正转
    gpio_wheel_x_front_clockwise = 1;
    gpio_wheel_x_front_counterclockwise = 0;
}
void wheel_x_front_backward()
{
    // X 方向前轮反转
    gpio_wheel_x_front_clockwise = 0;
    gpio_wheel_x_front_counterclockwise = 1;
}

void wheel_x_front_stop()
{
    // X 方向前轮停止
    gpio_wheel_x_front_clockwise = 0;
    gpio_wheel_x_front_counterclockwise = 0;
}

void wheel_x_rear_forward()
{
    // X 方向后轮正转
    gpio_wheel_x_rear_clockwise = 1;
    gpio_wheel_x_rear_counterclockwise = 0;
}

void wheel_x_rear_backward()
{
    // X 方向后轮反转
    gpio_wheel_x_rear_clockwise = 0;
    gpio_wheel_x_rear_counterclockwise = 1;
}

void wheel_x_rear_stop()
{
    // X 方向后轮停止
    gpio_wheel_x_rear_clockwise = 0;
    gpio_wheel_x_rear_counterclockwise = 0;
}

void wheels_x_forward()
{
    // X 方向所有轮子正转
    wheel_x_front_forward();
    wheel_x_rear_forward();
}

void wheels_x_backward()
{
    // X 方向所有轮子反转
    wheel_x_front_backward();
    wheel_x_rear_backward();
}

void wheels_x_stop()
{
    // X 方向所有轮子停止
    wheel_x_front_stop();
    wheel_x_rear_stop();
}

void wheel_y_front_forward()
{
    // Y 方向前轮正转
    gpio_wheel_y_front_clockwise = 1;
    gpio_wheel_y_front_counterclockwise = 0;
}

void wheel_y_front_backward()
{
    // Y 方向前轮反转
    gpio_wheel_y_front_clockwise = 0;
    gpio_wheel_y_front_counterclockwise = 1;
}

void wheel_y_front_stop()
{
    // Y 方向前轮停止
    gpio_wheel_y_front_clockwise = 0;
    gpio_wheel_y_front_counterclockwise = 0;
}

void wheel_y_rear_forward()
{
    // Y 方向后轮正转
    gpio_wheel_y_rear_clockwise = 1;
    gpio_wheel_y_rear_counterclockwise = 0;
}

void wheel_y_rear_backward()
{
    // Y 方向后轮反转
    gpio_wheel_y_rear_clockwise = 0;
    gpio_wheel_y_rear_counterclockwise = 1;
}

void wheel_y_rear_stop()
{
    // Y 方向后轮停止
    gpio_wheel_y_rear_clockwise = 0;
    gpio_wheel_y_rear_counterclockwise = 0;
}

void wheels_y_forward()
{
    // Y 方向所有轮子正转
    wheel_y_front_forward();
    wheel_y_rear_forward();
}

void wheels_y_backward()
{
    // Y 方向所有轮子反转
    wheel_y_front_backward();
    wheel_y_rear_backward();
}

void wheels_y_stop()
{
    // Y 方向所有轮子停止
    wheel_y_front_stop();
    wheel_y_rear_stop();
}

void wheels_all_stop()
{
    // 所有轮子停止
    gpio_wheel_x_front_clockwise = 0;
    gpio_wheel_x_front_counterclockwise = 0;
    gpio_wheel_x_rear_clockwise = 0;
    gpio_wheel_x_rear_counterclockwise = 0;
    gpio_wheel_y_front_clockwise = 0;
    gpio_wheel_y_front_counterclockwise = 0;
    gpio_wheel_y_rear_clockwise = 0;
    gpio_wheel_y_rear_counterclockwise = 0;
}