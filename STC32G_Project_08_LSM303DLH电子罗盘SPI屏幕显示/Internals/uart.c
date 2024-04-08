#include <STC32G.h>
#include <STDIO.H>
#include <INTRINS.H>
#include "uart.h"
#include "laser_ranging.h"
#include "oled.h"

unsigned char Uart1_TX_Counter, Uart2_TX_Counter, Uart3_TX_Counter, Uart4_TX_Counter;   // 发送计数
unsigned char Uart1_RX_Counter, Uart2_RX_Counter, Uart3_RX_Counter, Uart4_RX_Counter;   // 接收计数
bit           Uart1_TX_Busy, Uart2_TX_Busy, Uart3_TX_Busy, Uart4_TX_Busy;               // 发送忙标志
unsigned char Uart1_RX_Buffer[Uart1_BUF_LENGTH];                                        // 接收缓冲
unsigned char Uart2_RX_Buffer[Uart2_BUF_LENGTH];                                        // 接收缓冲
unsigned char Uart3_RX_Buffer[Uart3_BUF_LENGTH];                                        // 接收缓冲
unsigned char Uart4_RX_Buffer[Uart4_BUF_LENGTH];                                        // 接收缓冲

void Uart_Port_Init(void)
{
	P_SW1 |= 0xc0;						//UART1/USART1: RxD(P4.3), TxD(P4.4)
	P_SW2 &= ~0x01;						//UART2/USART2: RxD2(P1.0), TxD2(P1.1)
}

// 串口1的初始化函数，需要使用STC-ISP进行配置
void Uart1_Init(void)	// 9600bps@33.1776MHz
{
	SCON = 0x50;		// 8位数据,可变波特率
	AUXR |= 0x01;		// 串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		// 定时器时钟1T模式
	T2L = 0xA0;			// 设置定时初始值
	T2H = 0xFC;			// 设置定时初始值
	AUXR |= 0x10;		// 定时器2开始计时
	ES = 1;				// 使能串口1中断
}

// 串口2的初始化函数，需要使用STC-ISP进行配置
void Uart2_Init(void)	// 9600bps@33.1776MHz
{
	S2CON = 0x50;		// 8位数据,可变波特率
	AUXR |= 0x04;		// 定时器时钟1T模式
	T2L = 0xA0;			// 设置定时初始值
	T2H = 0xFC;			// 设置定时初始值
	AUXR |= 0x10;		// 定时器2开始计时
	IE2 |= 0x01;		// 使能串口2中断
}

void Uart1_SendString(unsigned char *puts)
{
    for (; *puts != 0;  puts++)                     // 遇到停止符0结束
    {
        SBUF = *puts;
        Uart1_TX_Busy = 1;
        while(Uart1_TX_Busy);
    }
}

void Uart2_SendString(unsigned char *puts)
{
    for (; *puts != 0;  puts++)                     // 遇到停止符0结束
    {
        S2BUF = *puts;
        Uart2_TX_Busy = 1;
        while(Uart2_TX_Busy);
    }
}

void Uart3_SendString(unsigned char *puts)
{
    for (; *puts != 0;  puts++)                     // 遇到停止符0结束
    {
        S3BUF = *puts;
        Uart3_TX_Busy = 1;
        while(Uart3_TX_Busy);
    }
}

void Uart4_SendString(unsigned char *puts)
{
    for (; *puts != 0;  puts++)                     // 遇到停止符0结束
    {
        S4BUF = *puts;
        Uart4_TX_Busy = 1;
        while(Uart4_TX_Busy);
    }
}

void Uart1_SendCmd(unsigned char *bytes, unsigned char length)
{
    unsigned char i;
    for (i = 0; i < length; i++)
    {
        SBUF = bytes[i];
        Uart1_TX_Busy = 1;
        while(Uart1_TX_Busy);
    }
}

void Uart2_SendCmd(unsigned char *bytes, unsigned char length)
{
    unsigned char i;
    for (i = 0; i < length; i++)
    {
        S2BUF = bytes[i];
        Uart2_TX_Busy = 1;
        while(Uart2_TX_Busy);
    }
}

void Uart3_SendCmd(unsigned char *bytes, unsigned char length)
{
    unsigned char i;
    for (i = 0; i < length; i++)
    {
        S3BUF = bytes[i];
        Uart3_TX_Busy = 1;
        while(Uart3_TX_Busy);
    }
}

void Uart4_SendCmd(unsigned char *bytes, unsigned char length)
{
    unsigned char i;
    for (i = 0; i < length; i++)
    {
        S4BUF = bytes[i];
        Uart4_TX_Busy = 1;
        while(Uart4_TX_Busy);
    }
}

void Uart1_Isr(void) interrupt 4
{
	if (TI)
	{
		TI = 0;
        Uart1_TX_Busy = 0;
	}
	if (RI)
	{
		RI = 0;
        Uart1_RX_Buffer[Uart1_RX_Counter++] = SBUF;
        if(Uart1_RX_Counter >= Uart1_BUF_LENGTH) Uart1_RX_Counter = 0;    // 防止越界

        // 以上为普通串口中断代码，以下是为激光测距仪设计的处理代码
        LaserRanging_IRQHandler(); 
    }
}


void Uart2_Isr(void) interrupt 8                                            // 串口2的中断函数
{
	if (S2CON & 0x02)	                                                    // 检测串口2发送中断
	{
		S2CON &= ~0x02; 	                                                // 清除串口2发送中断请求位
        Uart2_TX_Busy = 0;                                                  // 发送空闲标志位
	}
	if (S2CON & 0x01)	                                                    // 检测串口2接收中断
	{
		S2CON &= ~0x01;	                                                    // 清除串口2接收中断请求位
        Uart2_RX_Buffer[Uart2_RX_Counter] = S2BUF;                          // 接收数据存入缓冲区
        if(++Uart2_RX_Counter >= Uart2_BUF_LENGTH) Uart2_RX_Counter = 0;    // 缓冲区满, 循环
	}
}

void Uart3_Isr(void) interrupt 17                                           // 串口3的中断函数
{
	if (S3CON & 0x02)	                                                    // 检测串口3发送中断
	{
		S3CON &= ~0x02;	                                                    // 清除串口3发送中断请求位
        Uart3_TX_Busy = 0;                                                  // 发送空闲标志位
	}
	if (S3CON & 0x01)	                                                    // 检测串口3接收中断
	{
		S3CON &= ~0x01;	                                                    // 清除串口3接收中断请求位
        Uart3_RX_Buffer[Uart3_RX_Counter] = S3BUF;                          // 接收数据存入缓冲区
        if(++Uart3_RX_Counter >= Uart3_BUF_LENGTH) Uart3_RX_Counter = 0;    // 缓冲区满, 循环
	}
}

void Uart4_Isr(void) interrupt 18                                           // 串口4的中断函数
{
	if (S4CON & 0x02)	                                                    // 检测串口4发送中断
	{
		S4CON &= ~0x02;	                                                    // 清除串口4发送中断请求位
        Uart4_TX_Busy = 0;                                                  // 发送空闲标志位
	}
	if (S4CON & 0x01)	                                                    // 检测串口4接收中断
	{
		S4CON &= ~0x01;	                                                    // 清除串口4接收中断请求位
        Uart4_RX_Buffer[Uart4_RX_Counter] = S4BUF;                          // 接收数据存入缓冲区
        if(++Uart4_RX_Counter >= Uart4_BUF_LENGTH) Uart4_RX_Counter = 0;    // 缓冲区满, 循环
	}
}

void Uart1_Running()
{
    if((Uart1_TX_Counter != Uart1_RX_Counter) && (!Uart1_TX_Busy))          // 收到数据, 发送空闲
    {
        SBUF = Uart1_RX_Buffer[Uart1_TX_Counter];                           // 发送数据
        Uart1_TX_Busy = 1;                                                  // 标记忙
        if(++Uart1_TX_Counter >= Uart1_BUF_LENGTH) Uart1_TX_Counter = 0;    // 循环
    }
}

void Uart2_Running()
{
    if((Uart2_TX_Counter != Uart2_RX_Counter) && (!Uart2_TX_Busy))          // 收到数据, 发送空闲
    {
        S2BUF = Uart2_RX_Buffer[Uart2_TX_Counter];                          // 发送数据
        Uart2_TX_Busy = 1;                                                  // 标记忙
        if(++Uart2_TX_Counter >= Uart2_BUF_LENGTH) Uart2_TX_Counter = 0;    // 循环
    }
}

void Uart3_Running()
{
    if((Uart3_TX_Counter != Uart3_RX_Counter) && (!Uart3_TX_Busy))          // 收到数据, 发送空闲
    {
        S3BUF = Uart3_RX_Buffer[Uart3_TX_Counter];                          // 发送数据
        Uart3_TX_Busy = 1;                                                  // 标记忙
        if(++Uart3_TX_Counter >= Uart3_BUF_LENGTH) Uart3_TX_Counter = 0;    // 循环
    }
}

void Uart4_Running()
{
    if((Uart4_TX_Counter != Uart4_RX_Counter) && (!Uart4_TX_Busy))          // 收到数据, 发送空闲
    {
        S4BUF = Uart4_RX_Buffer[Uart4_TX_Counter];                          // 发送数据
        Uart4_TX_Busy = 1;                                                  // 标记忙
        if(++Uart4_TX_Counter >= Uart4_BUF_LENGTH) Uart4_TX_Counter = 0;    // 循环
    }
}