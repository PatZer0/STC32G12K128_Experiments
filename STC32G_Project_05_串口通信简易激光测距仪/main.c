/*-----------------------------------------------------------------------------------------
/ 请勿更改此文件编码！
/ 必须保持为默认编码GB2312，否则汉字无法正常显示！

* 时钟选择 33.1776MHz
* 按键为 P2.0~P2.3
* 激光测距模块使用P3.6(RX)/P3.7(TX)
* OLED显示屏使用P3.2/P3.3(I2C)

* --------------------------------------OLED显示屏-----------------------------------------
    - 默认使用P3.2/P3.3端口，修改请看 iic.c -> IIC_Init() 函数。
* ------------------------------------UART1串口通信----------------------------------------
    - 默认使用P3.6/P3.7为RX/TX，修改请看 uart.c -> Uart1_Config() 函数。
    - 如果实验P3.0/P3.1，可能导致无法下载程序。
    - 默认使用9600bps波特率（匹配激光测距模块），修改请看 uart.h 定义部分。

* 作者注：
    受于时间限制，本程序只实现了连续测量的功能以及显示，按键功能也只实现了KEY4，单次测量等
    其他功能有预留指令代码，但没有具体实现，有兴趣可以自行加上。
    
    由于硬件原因，按键KEY4-KEY1和屏幕的横向显示方向是反的，即屏幕左下角按键对应的是KEY4。
    如果需要修改，建议直接修改sbit key的定义。

* 注意：本程序仅供学习参考，请勿用于商业用途。
-----------------------------------------------------------------------------------------*/
#include <STRING.H>
#include <STC32G.H>
#include "oled.h"
#include "pic.h"
#include "uart.h"
#include "laser_ranging.h"

// 定义按键引脚
sbit key1 = P2^0;
sbit key2 = P2^1;
sbit key3 = P2^2;
sbit key4 = P2^3;
unsigned char key1_up_store = 1; // 按键1状态暂存
unsigned char key2_up_store = 1; // 按键2状态暂存
unsigned char key3_up_store = 1; // 按键3状态暂存
unsigned char key4_up_store = 1; // 按键4状态暂存
unsigned int Timer1_Counter = 0; // 定时器0计数器
unsigned char OLED_Refesh_Flag = 0; // OLED刷新标志位
unsigned char LaserRanging_State = 0; // 激光状态


void OLED_LaserRanging_UI_OnOff()
{
    if(!LaserRanging_State)
    {
        OLED_BuffShowString(0,6,"OFF",0);  //显示数据
        OLED_BuffShow_NoInterrupt();                    //将缓存写入显示屏显示
    }
    else
    {
        OLED_BuffShowString(0,6,"ON ",0);  //显示数据
        OLED_BuffShow_NoInterrupt();                    //将缓存写入显示屏显示
    }
}

void OLED_LaserRanging_UI_Range()
{
    if(LaserRanging_Range == 5)
    {
        OLED_BuffShowString(8*(16-4),0," ",1);
        OLED_BuffShowNum(8*(16-3),0,LaserRanging_Range,1);
        OLED_BuffShowString(8*(16-2),0,"米",1);
        OLED_BuffShow_NoInterrupt();                      //将缓存写入显示屏显示
    }
    else
    {
        OLED_BuffShowNum(8*(16-4),0,LaserRanging_Range,1);
        OLED_BuffShowString(8*(16-2),0,"米",1);
        OLED_BuffShow_NoInterrupt();                      //将缓存写入显示屏显示
    }
}

void OLED_LaserRanging_UI_Refresh()
{
    OLED_BuffShowString(0,0,"激光测距        ",1);
    OLED_BuffShowString(0,2,"   按KEY4开始   ", 0);
    OLED_BuffShowString(0,6,"          [+][-]",0);
    OLED_BuffShow_NoInterrupt();                                //将缓存写入显示屏显示
}

void OLED_LaserRanging_UI_Init()
{
    OLED_BuffClear();                           //清除全部缓存
    OLED_LaserRanging_UI_Refresh();             //刷新激光测距UI
    OLED_LaserRanging_UI_OnOff();               //显示激光测距状态
    OLED_LaserRanging_UI_Range();           //显示距离值
}

void OLED_Loading()
{
    OLED_BuffClear();                               //清除全部缓存
    OLED_BuffShowString(0,3,"   PLEASE WAIT  ",1);  //显示数据
    OLED_BuffShow_NoInterrupt();                                //将缓存写入显示屏显示
}

void key1_short_press()
{
    // 短按键1功能
    LaserRanging_State = 0;
    LaserRanging_Range_Decrease();
    OLED_LaserRanging_UI_Init();
}

void key2_short_press()
{
    // 短按键2功能
    LaserRanging_State = 0;
    LaserRanging_Range_Increase();
    OLED_LaserRanging_UI_Init();
}

void key3_short_press()
{
    // 短按键3功能
}

void key4_short_press()
{
    // 短按键4功能
    LaserRanging_State = !LaserRanging_State;
    OLED_LaserRanging_UI_OnOff();
    if(LaserRanging_State)  // 打开激光测距
    {
        LaserRanging_Continous();
        OLED_BuffShowString(0, 2, "  正在等待数据  ", 0);
        OLED_BuffShowString(0, 4, "  正在等待数据  ", 0);
        OLED_BuffShow_NoInterrupt();
    }
    else                    // 关闭激光测距
    {
        LaserRanging_LaserOff();
        OLED_BuffShowString(0, 4, "    测距暂停    ", 0);
        OLED_BuffShow_NoInterrupt();
    }
}

void key1_check() 
{
    if(key1 == 0 && key1_up_store == 1)
    {
        key1_up_store = 0; // 标记按键被按下
        OLED_BuffShowString(13*8,6,"[-]",1);  //显示数据
        OLED_BuffShow_NoInterrupt();                    //将缓存写入显示屏显示
    }
    if(key1 == 1 && key1_up_store == 0)
    {
        key1_up_store = 1; // 标记按键被释放
        key1_short_press(); // 触发短按键1功能
        OLED_BuffShowString(13*8,6,"[-]",0);  //显示数据
        OLED_BuffShow_NoInterrupt();                    //将缓存写入显示屏显示
    }
}

void key2_check()
{
    if(key2 == 0 && key2_up_store == 1)
    {
        key2_up_store = 0; // 标记按键被按下
        OLED_BuffShowString(10*8,6,"[+]",1);  //显示数据
        OLED_BuffShow_NoInterrupt();                    //将缓存写入显示屏显示
    }
    if(key2 == 1 && key2_up_store == 0)
    {
        key2_up_store = 1; // 标记按键被释放
        key2_short_press(); // 触发短按键2功能
        OLED_BuffShowString(10*8,6,"[+]",0);  //显示数据
        OLED_BuffShow_NoInterrupt();                    //将缓存写入显示屏显示
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
        if(!LaserRanging_State)
        {
            OLED_BuffShowString(0,6,"OFF",1);  //显示数据
            OLED_BuffShow_NoInterrupt();                    //将缓存写入显示屏显示
        }
        else
        {
            OLED_BuffShowString(0,6,"ON ",1);  //显示数据
            OLED_BuffShow_NoInterrupt();                    //将缓存写入显示屏显示
        }

    }
    if(key4 == 1 && key4_up_store == 0)
    {
        key4_up_store = 1; // 标记按键被释放
        key4_short_press(); // 触发短按键4功能
    }
}

void Timer1_Isr(void) interrupt 3
{
    unsigned char Freq_Hz = 1; // 刷新频率
    Timer1_Counter++;
    if (Timer1_Counter >= (1000/Freq_Hz)) // 1000ms/Freq_Hz
    {
        OLED_Refesh_Flag = 1;
        Timer1_Counter = 0;
    }
}

void Timer1_Init(void)		//1毫秒@33.1776MHz
{
	TM1PS = 0x00;			//设置定时器时钟预分频 ( 注意:并非所有系列都有此寄存器,详情请查看数据手册 )
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x66;				//设置定时初始值
	TH1 = 0x7E;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;				//使能定时器1中断
}


void Sys_Init()
{
    WTST = 0;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    EAXFR = 1; //扩展寄存器(XFR)访问使能
    CKCON = 0; //提高访问XRAM速度

    P0M1 = 0x00;   P0M0 = 0x00;   //设置为准双向口
    // P1M1 = 0x00;   P1M0 = 0x00;   //设置为准双向口
    P1M0 = 0xff;   P1M1 = 0x00;   //设置为推挽输出
    P2M1 = 0x00;   P2M0 = 0x00;   //设置为准双向口
    P3M1 = 0x00;   P3M0 = 0x00;   //设置为准双向口
    P4M1 = 0x00;   P4M0 = 0x00;   //设置为准双向口
    P5M1 = 0x00;   P5M0 = 0x00;   //设置为准双向口
    P6M1 = 0x00;   P6M0 = 0x00;   //设置为准双向口
    P7M1 = 0x00;   P7M0 = 0x00;   //设置为准双向口
}

void main(void)
{
    EA = 1;                             //开总中断
    Sys_Init();                         //系统初始化
    Uart1_Init();                       //串口1初始化
    Uart_Port_Init();                   //串口端口初始化
    Timer1_Init();                      //定时器0初始化
    OLED_Init();                        //OLED初始化
    OLED_LaserRanging_UI_Init();        //初始化激光测距UI
    LaserRanging_Init();
    while (1)
    {
        key1_check();                   //检查按键1
        key2_check();                   //检查按键2
        key3_check();                   //检查按键3
        key4_check();                   //检查按键4
        if(OLED_Refesh_Flag)            //刷新OLED
        {
            OLED_Refesh_Flag = 0;
            EA = 0;
            OLED_BuffShow_NoInterrupt();
            EA = 1;
        }
    }
}
