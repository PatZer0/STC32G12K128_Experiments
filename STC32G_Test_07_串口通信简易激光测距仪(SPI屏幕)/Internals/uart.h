#ifndef __UART_H__
#define __UART_H__

#define Uart1_BUF_LENGTH 16                        // 定义接收缓冲区长度
#define Uart2_BUF_LENGTH 16                        // 定义接收缓冲区长度
#define Uart3_BUF_LENGTH 16                        // 定义接收缓冲区长度
#define Uart4_BUF_LENGTH 16                        // 定义接收缓冲区长度

extern unsigned char Uart1_TX_Counter, Uart2_TX_Counter, Uart3_TX_Counter, Uart4_TX_Counter;   // 发送计数
extern unsigned char Uart1_RX_Counter, Uart2_RX_Counter, Uart3_RX_Counter, Uart4_RX_Counter;   // 接收计数
extern bit           Uart1_TX_Busy, Uart1_TX_Busy, Uart1_TX_Busy, Uart4_TX_Busy;               // 发送忙标志
extern unsigned char Uart1_RX_Buffer[Uart1_BUF_LENGTH];                                        // 接收缓冲
extern unsigned char Uart2_RX_Buffer[Uart2_BUF_LENGTH];                                        // 接收缓冲
extern unsigned char Uart3_RX_Buffer[Uart3_BUF_LENGTH];                                        // 接收缓冲
extern unsigned char Uart4_RX_Buffer[Uart4_BUF_LENGTH];                                        // 接收缓冲

void Uart_Port_Init(void);
void Uart1_Init();
void Uart2_Init();
void Uart3_Init();
void Uart4_Init();

void Uart1_SendString(unsigned char *puts);
void Uart2_SendString(unsigned char *puts);
void Uart3_SendString(unsigned char *puts);
void Uart4_SendString(unsigned char *puts);
void Uart1_SendCmd(unsigned char *bytes, unsigned char length);
void Uart2_SendCmd(unsigned char *bytes, unsigned char length);
void Uart3_SendCmd(unsigned char *bytes, unsigned char length);
void Uart4_SendCmd(unsigned char *bytes, unsigned char length);

// 根据需要的串口，在主函数循环中添加以下运行函数
void Uart1_Running();
void Uart2_Running();
void Uart3_Running();
void Uart4_Running();

#endif