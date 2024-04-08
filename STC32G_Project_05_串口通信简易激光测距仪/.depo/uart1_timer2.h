#ifndef __UART1_TIMER2_H__
#define __UART1_TIMER2_H__

void Uart1_Init(void);
void Uart1_SendChar(char dat);
void Uart1_SendString(char *str);
void Uart1_Running(void);

extern char rcv_data;
extern char rcv_buffer[16];  // 存储接收到的数据
extern bit data_ready;   // 数据是否接收完毕的标志

#endif