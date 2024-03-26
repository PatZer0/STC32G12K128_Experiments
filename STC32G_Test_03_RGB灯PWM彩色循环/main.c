#include <STC32G.h>

#define PWM1_0      0x00	//P:P1.0  N:P1.1
#define PWM1_1      0x01	//P:P2.0  N:P2.1
#define PWM1_2      0x02	//P:P6.0  N:P6.1

#define PWM2_0      0x00	//P:P1.2/P5.4  N:P1.3
#define PWM2_1      0x04	//P:P2.2  N:P2.3
#define PWM2_2      0x08	//P:P6.2  N:P6.3

#define PWM3_0      0x00	//P:P1.4  N:P1.5
#define PWM3_1      0x10	//P:P2.4  N:P2.5
#define PWM3_2      0x20	//P:P6.4  N:P6.5

#define PWM4_0      0x00	//P:P1.6  N:P1.7
#define PWM4_1      0x40	//P:P2.6  N:P2.7
#define PWM4_2      0x80	//P:P6.6  N:P6.7
#define PWM4_3      0xC0	//P:P3.4  N:P3.3

#define ENO1P       0x01
#define ENO1N       0x02
#define ENO2P       0x04
#define ENO2N       0x08
#define ENO3P       0x10
#define ENO3N       0x20
#define ENO4P       0x40
#define ENO4N       0x80

// #define MAIN_Fosc 24000000UL    // 主频，本实例中不需要

// 定义按键引脚
sbit key1 = P2^0;
sbit key2 = P2^1;
sbit key3 = P2^2;
sbit key4 = P2^3;
// 定义LED引脚
sbit led_vcc_green = P5^0;
sbit led_vcc_red = P5^1;
sbit led_vcc_blue = P5^2;

unsigned int pwma_1_duty;
unsigned int pwma_2_duty;
unsigned int pwma_3_duty;
bit pwma_1_flag;
bit pwma_2_flag;
bit pwma_3_flag;

unsigned char led_power = 0; // LED 电源状态，0为关闭，1为开启

unsigned int led_rgb_flash_interval = 750; // LED 三原色循环切换间隔，单位为毫秒
unsigned int led_rgb_flash_interval_index = 0; // LED 三原色循环切换间隔索引，用于选择数组中的某个值
unsigned int led_rgb_flash_intervals[6] = {100, 200, 500, 750, 1000, 2000}; // LED 三原色循环切换间隔，单位为毫秒
unsigned char led_rgb_flash_array[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}; // LED 三原色循环切换数组，0为关闭，1为开启
unsigned char led_rgb_flash_index = 0; // LED 三原色循环切换主索引，用于选择数组中的某个值

unsigned int led_color_gradient_interval = 1000; // LED 彩色渐变周期长度，单位为毫秒
unsigned int led_color_gradient_interval_index = 0; // LED 彩色渐变周期长度索引，用于选择数组中的某个值
unsigned int led_color_gradient_intervals[7] = {100, 200, 500, 750, 1000, 2000, 3000}; // LED 彩色渐变周期长度，单位为毫秒

unsigned char key1_up_store = 1; // 按键1状态暂存
unsigned char key2_up_store = 1; // 按键2状态暂存
unsigned char key3_up_store = 1; // 按键3状态暂存
unsigned char key4_up_store = 1; // 按键4状态暂存

unsigned char led_rgb_mode = 0; // RGB LED灯光模式，0为白光常亮，1为三原色循环，2为彩色渐变循环

// 计数器要使用int类型，如果使用char(最大值255)会溢出，导致功能异常
unsigned int timer0_counter = 0; // LED流水计数器

void key1_short_press()
{
    // 短按键1功能：关闭RGB灯
    led_power = 0;
}

void key2_short_press()
{
    // 短按键2功能：打开RGB灯
    led_power = 1;
}

void key3_short_press()
{
    // 短按键3功能：切换LED灯模式【白光呼吸/彩色循环/七彩闪烁】
    led_rgb_mode++;
    if(led_rgb_mode > 2)
    {
        led_rgb_mode = 0;
    }
}

void key4_short_press()
{
    // 短按键4功能：更改LED变化的速度
    if(led_rgb_mode == 1)
    {
        led_rgb_flash_interval_index++;
        led_rgb_flash_interval = led_rgb_flash_intervals[led_rgb_flash_interval_index % 6];
        if(led_rgb_flash_interval_index >= 6)
        {
            led_rgb_flash_interval_index = 0;
        }
    }
    else if(led_rgb_mode == 2)
    {
        led_color_gradient_interval_index++;
        led_color_gradient_interval = led_color_gradient_intervals[led_color_gradient_interval_index % 7];
        if(led_color_gradient_interval_index >= 7)
        {
            led_color_gradient_interval_index = 0;
        }
    }
}

void key1_check() 
{
    if(key1 == 0 && key1_up_store == 1)
    {
        key1_up_store = 0; // 标记按键被按下
    }
    if(key1 == 1 && key1_up_store == 0)
    {
        key1_up_store = 1; // 标记按键被释放
        key1_short_press(); // 触发短按键1功能
    }
}

void key2_check()
{
    if(key2 == 0 && key2_up_store == 1)
    {
        key2_up_store = 0; // 标记按键被按下
    }
    if(key2 == 1 && key2_up_store == 0)
    {
        key2_up_store = 1; // 标记按键被释放
        key2_short_press(); // 触发短按键2功能
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

void led_disabled()
{
    led_vcc_green = 0;
    led_vcc_red = 0;
    led_vcc_blue = 0;
}

void led_mode_0_white_light()
{
    led_vcc_green = 1;
    led_vcc_red = 1;
    led_vcc_blue = 1;
}

void led_mode_1_rgb_flash()
{
    led_rgb_flash_index++;
    if(led_rgb_flash_index >= 3)
    {
        led_rgb_flash_index = 0;
    }
    led_vcc_green = led_rgb_flash_array[led_rgb_flash_index][0];
    led_vcc_red = led_rgb_flash_array[led_rgb_flash_index][1];
    led_vcc_blue = led_rgb_flash_array[led_rgb_flash_index][2];
}

void led_mode_2_color_gradient()
{
    
}

void timer0_isr(void) interrupt 1
{
    timer0_counter++;
    if(led_power == 0)
    {
        led_disabled();
    }
    // RGB LED灯光模式，0为白光常亮
    else if(led_rgb_mode == 0 && led_power == 1)
    {
        led_mode_0_white_light();
    }
    // 1为三原色循环
    else if(led_rgb_mode == 1 && led_power == 1)
    {
        if(timer0_counter >= led_rgb_flash_interval)
        {
            timer0_counter = 0;
            led_mode_1_rgb_flash();
        }
    }
    // 2为彩色渐变循环
    else if(led_rgb_mode == 2 && led_power == 1)
    {
        if(timer0_counter >= led_color_gradient_interval)
        {
            timer0_counter = 0;
            led_mode_2_color_gradient();
        }
    }
    pwm_duty_update();
}

void timer0_init(void)		//0.1毫秒@24.000M 对应 PWM频率5000Hz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0xA0;				//设置定时初始值
	TH0 = 0xF6;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开
	ET0 = 1;				//使能定时器0中断
    EA = 1;			    	//使能总中断
}

// 延时函数，本实例中未使用
// void delay_ms(unsigned char ms)
// {
//      unsigned int i;
//      do{
//           i = MAIN_Fosc / 6000;
//           while(--i);
//      }while(--ms);
// }

void pwma_init(void)
{
    // 输出频率计算：(SYS_CLOCK) / (PSCR+1) / (ARR+1) = 24.0KHz
    // 输出占空比计算：(CCR) / (ARR+1) * 100% = 50%

    // 1.选择时钟源（默认24.0MHz）
    // 2.时基单元
    PWMA_PSCRH = 0x00;  // 高低位都是0，1分频（不改变）
    PWMA_PSCRL = 0x00;

    // 3.输入输出引脚配置
    PWMA_ENO = 0x00;        // 清零
    PWMA_ENO |= ENO1P;      //使能输出
    PWMA_ENO |= ENO1N;      //使能输出
    PWMA_ENO |= ENO2P;      //使能输出
    PWMA_ENO |= ENO2N;      //使能输出
    PWMA_ENO |= ENO3P;      //使能输出
    PWMA_ENO |= ENO3N;      //使能输出

    PWMA_PS = 0x00;         //高级 PWM 通道输出脚选择位
    PWMA_PS |= PWM1_2;      //选择 PWM1_2 通道
    PWMA_PS |= PWM2_2;      //选择 PWM2_2 通道
    PWMA_PS |= PWM3_2;      //选择 PWM3_2 通道
    PWMA_PS |= PWM4_2;      //选择 PWM4_2 通道

    // 4.输入输出模式设置
    PWMA_CCER1 = 0x00;      // 清零
    PWMA_CCMR1 = 0x68;      // 设置PWM模式为输出
    PWMA_CCER1 = 0x01;      // 开启输出比较通道

    PWMA_BKR = 0x80;        // 使能主输出

    PWMA_CR1 = 1;           // 使能ARR预装载, 开始计时
}

void pwm_duty_update(void)
{
    PWMA_CCR1H = (unsigned char)(pwma_1_duty >> 8); //设置占空比时间
    PWMA_CCR1L = (unsigned char)(pwma_1_duty);
    PWMA_CCR2H = (unsigned char)(pwma_2_duty >> 8); //设置占空比时间
    PWMA_CCR2L = (unsigned char)(pwma_2_duty);
    PWMA_CCR3H = (unsigned char)(pwma_3_duty >> 8); //设置占空比时间
    PWMA_CCR3L = (unsigned char)(pwma_3_duty);
}

void main(void)
{
    WTST = 0;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    EAXFR = 1; //扩展寄存器(XFR)访问使能
    CKCON = 0; //提高访问XRAM速度

    P5M0 = 0xff; P5M1 = 0x00;           //配置P5为输出模式
    P2M0 = 0x00; P2M1 = 0x00;           //配置P2为准双向模式

    pwma_init();                        //初始化PWM
    timer0_init();                      //初始化定时器0
    while(1)
    {
        // 检测按键
        key1_check();
        key2_check();
        key3_check();
        key4_check();
    }
}