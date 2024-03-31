#include <STC32G.H>
#include "uart1_timer2.h"

bit busy;
char wptr, rptr, buffer[16];
char rcv_data;
char rcv_buffer[16];  // 存储接收到的数据
bit data_ready = 0;   // 数据是否接收完毕的标志

void Uart1_Isr() interrupt 4
{
	if(TI)
    {
        TI = 0;     //清除中断标志
        busy = 0;   //串口空闲
    }
    if(RI)
    {
        RI = 0;                     //清除接收中断标志
        buffer[wptr++] = SBUF;      //接收数据存入缓冲区
        wptr &= 0x0f;               //环形缓冲区
    }
}

void Uart1_Init(void)	//9600bps@33.1776MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xA0;			//设置定时初始值
	TH1 = 0xFC;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时

	P_SW1 &= ~0xc0;						//UART1/USART1: RxD(P3.0), TxD(P3.1)
}

void Uart1_SendChar(char dat)
{
    while(busy);     //等待串口空闲
    busy = 1;        //设置串口忙标志
    SBUF = dat;      //发送数据
}

void Uart1_SendString(char *str)
{
    while(*str)                     //发送字符串
    {
        Uart1_SendChar(*str++);     //发送字符
    }
}

void Uart1_Running(void)
{
    static char rcv_index = 0;  // 接收数据的索引
    
    if(rptr != wptr)  // 缓冲区有数据
    {
        rcv_buffer[rcv_index++] = buffer[rptr++];  // 接收数据存入 rcv_buffer
        rptr &= 0x0f;  // 环形缓冲区
        
        if(rcv_index >= 16)  // 如果接收到足够的数据
        {
            rcv_index = 0;  // 重置索引
            data_ready = 1;  // 设置数据接收完毕标志
        }
    }
}
