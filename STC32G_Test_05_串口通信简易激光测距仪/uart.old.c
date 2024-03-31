/*
������uart.c���Ƴ��Ĵ��롣
*/

#define SYSCLK_UART 33177600L                       //������ʱ�ӣ���ȷ���㲨���ʣ�
#define Baudrate1 (65536 - SYSCLK_UART / 9600 / 4)  //���岨����
#define Baudrate2 (65536 - SYSCLK_UART / 9600 / 4)  //���岨����
#define Baudrate3 (65536 - SYSCLK_UART / 9600 / 4)  //���岨����
#define Baudrate4 (65536 - SYSCLK_UART / 9600 / 4)  //���岨����

void Uart1_Init(void)
{
    Uart1_Config(2);    // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
    EA = 1;             //����ȫ���ж�
}

void Uart1_SetTimer2Baudraye(unsigned long dat)
{
    T2R = 0;		            //ֹͣ��ʱ��2
    T2_CT = 0;	                //Timer2 set As Timer
    T2x12 = 1;	                //Timer2 set as 1T mode
    T2H = (unsigned char)(dat / 256);
    T2L = (unsigned char)(dat % 256);
    ET2 = 0;                    //��ֹ�ж�
    T2R = 1;		            //Timer run enable
}

void Uart1_Config(unsigned char brt)
{
    /*********** ������ʹ�ö�ʱ��2 *****************/
    if(brt == 2)
    {
        S1BRT = 1;	//S1 BRT Use Timer2;
        Uart1_SetTimer2Baudraye(Baudrate1);
    }

    /*********** ������ʹ�ö�ʱ��1 *****************/
    else
    {
        TR1 = 0;
        S1BRT = 0;		//S1 BRT Use Timer1;
        T1_CT = 0;		//Timer1 set As Timer
        T1x12 = 1;		//Timer1 set as 1T mode
        TMOD &= ~0x30;//Timer1_16bitAutoReload;
        TH1 = (unsigned char)(Baudrate1 / 256);
        TL1 = (unsigned char)(Baudrate1 % 256);
        ET1 = 0;    //��ֹ�ж�
        TR1 = 1;
    }
    /*************************************************/

    SCON = (SCON & 0x3f) | 0x40;    //Uart1ģʽ, 0x00: ͬ����λ���, 0x40: 8λ����,�ɱ䲨����, 0x80: 9λ����,�̶�������, 0xc0: 9λ����,�ɱ䲨����
//  PS  = 1;    //�����ȼ��ж�
    ES  = 1;    //�����ж�
    REN = 1;    //�������
    P_SW1 &= 0x3f;
    P_SW1 |= 0x40;      //Uart1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4

    Uart1_TX_Busy = 0;
    Uart1_TX_Counter = 0;
    Uart1_RX_Counter = 0;
}