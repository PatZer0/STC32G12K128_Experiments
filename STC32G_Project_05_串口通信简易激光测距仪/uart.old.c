/*
这里是uart.c中移除的代码。
*/

#define SYSCLK_UART 33177600L                       //定义主时钟（精确计算波特率）
#define Baudrate1 (65536 - SYSCLK_UART / 9600 / 4)  //定义波特率
#define Baudrate2 (65536 - SYSCLK_UART / 9600 / 4)  //定义波特率
#define Baudrate3 (65536 - SYSCLK_UART / 9600 / 4)  //定义波特率
#define Baudrate4 (65536 - SYSCLK_UART / 9600 / 4)  //定义波特率

void Uart1_Init(void)
{
    Uart1_Config(2);    // 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
    EA = 1;             //允许全局中断
}

void Uart1_SetTimer2Baudraye(unsigned long dat)
{
    T2R = 0;		            //停止定时器2
    T2_CT = 0;	                //Timer2 set As Timer
    T2x12 = 1;	                //Timer2 set as 1T mode
    T2H = (unsigned char)(dat / 256);
    T2L = (unsigned char)(dat % 256);
    ET2 = 0;                    //禁止中断
    T2R = 1;		            //Timer run enable
}

void Uart1_Config(unsigned char brt)
{
    /*********** 波特率使用定时器2 *****************/
    if(brt == 2)
    {
        S1BRT = 1;	//S1 BRT Use Timer2;
        Uart1_SetTimer2Baudraye(Baudrate1);
    }

    /*********** 波特率使用定时器1 *****************/
    else
    {
        TR1 = 0;
        S1BRT = 0;		//S1 BRT Use Timer1;
        T1_CT = 0;		//Timer1 set As Timer
        T1x12 = 1;		//Timer1 set as 1T mode
        TMOD &= ~0x30;//Timer1_16bitAutoReload;
        TH1 = (unsigned char)(Baudrate1 / 256);
        TL1 = (unsigned char)(Baudrate1 % 256);
        ET1 = 0;    //禁止中断
        TR1 = 1;
    }
    /*************************************************/

    SCON = (SCON & 0x3f) | 0x40;    //Uart1模式, 0x00: 同步移位输出, 0x40: 8位数据,可变波特率, 0x80: 9位数据,固定波特率, 0xc0: 9位数据,可变波特率
//  PS  = 1;    //高优先级中断
    ES  = 1;    //允许中断
    REN = 1;    //允许接收
    P_SW1 &= 0x3f;
    P_SW1 |= 0x40;      //Uart1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4

    Uart1_TX_Busy = 0;
    Uart1_TX_Counter = 0;
    Uart1_RX_Counter = 0;
}