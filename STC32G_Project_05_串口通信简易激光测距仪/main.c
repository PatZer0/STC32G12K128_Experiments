/*-----------------------------------------------------------------------------------------
/ ������Ĵ��ļ����룡
/ ���뱣��ΪĬ�ϱ���GB2312���������޷�������ʾ��

* ʱ��ѡ�� 33.1776MHz
* ����Ϊ P2.0~P2.3
* ������ģ��ʹ��P3.6(RX)/P3.7(TX)
* OLED��ʾ��ʹ��P3.2/P3.3(I2C)

* --------------------------------------OLED��ʾ��-----------------------------------------
    - Ĭ��ʹ��P3.2/P3.3�˿ڣ��޸��뿴 iic.c -> IIC_Init() ������
* ------------------------------------UART1����ͨ��----------------------------------------
    - Ĭ��ʹ��P3.6/P3.7ΪRX/TX���޸��뿴 uart.c -> Uart1_Config() ������
    - ���ʵ��P3.0/P3.1�����ܵ����޷����س���
    - Ĭ��ʹ��9600bps�����ʣ�ƥ�伤����ģ�飩���޸��뿴 uart.h ���岿�֡�

* ����ע��
    ����ʱ�����ƣ�������ֻʵ�������������Ĺ����Լ���ʾ����������Ҳֻʵ����KEY4�����β�����
    ����������Ԥ��ָ����룬��û�о���ʵ�֣�����Ȥ�������м��ϡ�
    
    ����Ӳ��ԭ�򣬰���KEY4-KEY1����Ļ�ĺ�����ʾ�����Ƿ��ģ�����Ļ���½ǰ�����Ӧ����KEY4��
    �����Ҫ�޸ģ�����ֱ���޸�sbit key�Ķ��塣

* ע�⣺���������ѧϰ�ο�������������ҵ��;��
-----------------------------------------------------------------------------------------*/
#include <STRING.H>
#include <STC32G.H>
#include "oled.h"
#include "pic.h"
#include "uart.h"
#include "laser_ranging.h"

// ���尴������
sbit key1 = P2^0;
sbit key2 = P2^1;
sbit key3 = P2^2;
sbit key4 = P2^3;
unsigned char key1_up_store = 1; // ����1״̬�ݴ�
unsigned char key2_up_store = 1; // ����2״̬�ݴ�
unsigned char key3_up_store = 1; // ����3״̬�ݴ�
unsigned char key4_up_store = 1; // ����4״̬�ݴ�
unsigned int Timer1_Counter = 0; // ��ʱ��0������
unsigned char OLED_Refesh_Flag = 0; // OLEDˢ�±�־λ
unsigned char LaserRanging_State = 0; // ����״̬


void OLED_LaserRanging_UI_OnOff()
{
    if(!LaserRanging_State)
    {
        OLED_BuffShowString(0,6,"OFF",0);  //��ʾ����
        OLED_BuffShow_NoInterrupt();                    //������д����ʾ����ʾ
    }
    else
    {
        OLED_BuffShowString(0,6,"ON ",0);  //��ʾ����
        OLED_BuffShow_NoInterrupt();                    //������д����ʾ����ʾ
    }
}

void OLED_LaserRanging_UI_Range()
{
    if(LaserRanging_Range == 5)
    {
        OLED_BuffShowString(8*(16-4),0," ",1);
        OLED_BuffShowNum(8*(16-3),0,LaserRanging_Range,1);
        OLED_BuffShowString(8*(16-2),0,"��",1);
        OLED_BuffShow_NoInterrupt();                      //������д����ʾ����ʾ
    }
    else
    {
        OLED_BuffShowNum(8*(16-4),0,LaserRanging_Range,1);
        OLED_BuffShowString(8*(16-2),0,"��",1);
        OLED_BuffShow_NoInterrupt();                      //������д����ʾ����ʾ
    }
}

void OLED_LaserRanging_UI_Refresh()
{
    OLED_BuffShowString(0,0,"������        ",1);
    OLED_BuffShowString(0,2,"   ��KEY4��ʼ   ", 0);
    OLED_BuffShowString(0,6,"          [+][-]",0);
    OLED_BuffShow_NoInterrupt();                                //������д����ʾ����ʾ
}

void OLED_LaserRanging_UI_Init()
{
    OLED_BuffClear();                           //���ȫ������
    OLED_LaserRanging_UI_Refresh();             //ˢ�¼�����UI
    OLED_LaserRanging_UI_OnOff();               //��ʾ������״̬
    OLED_LaserRanging_UI_Range();           //��ʾ����ֵ
}

void OLED_Loading()
{
    OLED_BuffClear();                               //���ȫ������
    OLED_BuffShowString(0,3,"   PLEASE WAIT  ",1);  //��ʾ����
    OLED_BuffShow_NoInterrupt();                                //������д����ʾ����ʾ
}

void key1_short_press()
{
    // �̰���1����
    LaserRanging_State = 0;
    LaserRanging_Range_Decrease();
    OLED_LaserRanging_UI_Init();
}

void key2_short_press()
{
    // �̰���2����
    LaserRanging_State = 0;
    LaserRanging_Range_Increase();
    OLED_LaserRanging_UI_Init();
}

void key3_short_press()
{
    // �̰���3����
}

void key4_short_press()
{
    // �̰���4����
    LaserRanging_State = !LaserRanging_State;
    OLED_LaserRanging_UI_OnOff();
    if(LaserRanging_State)  // �򿪼�����
    {
        LaserRanging_Continous();
        OLED_BuffShowString(0, 2, "  ���ڵȴ�����  ", 0);
        OLED_BuffShowString(0, 4, "  ���ڵȴ�����  ", 0);
        OLED_BuffShow_NoInterrupt();
    }
    else                    // �رռ�����
    {
        LaserRanging_LaserOff();
        OLED_BuffShowString(0, 4, "    �����ͣ    ", 0);
        OLED_BuffShow_NoInterrupt();
    }
}

void key1_check() 
{
    if(key1 == 0 && key1_up_store == 1)
    {
        key1_up_store = 0; // ��ǰ���������
        OLED_BuffShowString(13*8,6,"[-]",1);  //��ʾ����
        OLED_BuffShow_NoInterrupt();                    //������д����ʾ����ʾ
    }
    if(key1 == 1 && key1_up_store == 0)
    {
        key1_up_store = 1; // ��ǰ������ͷ�
        key1_short_press(); // �����̰���1����
        OLED_BuffShowString(13*8,6,"[-]",0);  //��ʾ����
        OLED_BuffShow_NoInterrupt();                    //������д����ʾ����ʾ
    }
}

void key2_check()
{
    if(key2 == 0 && key2_up_store == 1)
    {
        key2_up_store = 0; // ��ǰ���������
        OLED_BuffShowString(10*8,6,"[+]",1);  //��ʾ����
        OLED_BuffShow_NoInterrupt();                    //������д����ʾ����ʾ
    }
    if(key2 == 1 && key2_up_store == 0)
    {
        key2_up_store = 1; // ��ǰ������ͷ�
        key2_short_press(); // �����̰���2����
        OLED_BuffShowString(10*8,6,"[+]",0);  //��ʾ����
        OLED_BuffShow_NoInterrupt();                    //������д����ʾ����ʾ
    }
}

void key3_check()
{
    if(key3 == 0 && key3_up_store == 1)
    {
        key3_up_store = 0; // ��ǰ���������
    }
    if(key3 == 1 && key3_up_store == 0)
    {
        key3_up_store = 1; // ��ǰ������ͷ�
        key3_short_press(); // �����̰���3����
    }
}

void key4_check()
{
    if(key4 == 0 && key4_up_store == 1)
    {
        key4_up_store = 0; // ��ǰ���������
        if(!LaserRanging_State)
        {
            OLED_BuffShowString(0,6,"OFF",1);  //��ʾ����
            OLED_BuffShow_NoInterrupt();                    //������д����ʾ����ʾ
        }
        else
        {
            OLED_BuffShowString(0,6,"ON ",1);  //��ʾ����
            OLED_BuffShow_NoInterrupt();                    //������д����ʾ����ʾ
        }

    }
    if(key4 == 1 && key4_up_store == 0)
    {
        key4_up_store = 1; // ��ǰ������ͷ�
        key4_short_press(); // �����̰���4����
    }
}

void Timer1_Isr(void) interrupt 3
{
    unsigned char Freq_Hz = 1; // ˢ��Ƶ��
    Timer1_Counter++;
    if (Timer1_Counter >= (1000/Freq_Hz)) // 1000ms/Freq_Hz
    {
        OLED_Refesh_Flag = 1;
        Timer1_Counter = 0;
    }
}

void Timer1_Init(void)		//1����@33.1776MHz
{
	TM1PS = 0x00;			//���ö�ʱ��ʱ��Ԥ��Ƶ ( ע��:��������ϵ�ж��д˼Ĵ���,������鿴�����ֲ� )
	AUXR |= 0x40;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0x66;				//���ö�ʱ��ʼֵ
	TH1 = 0x7E;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	ET1 = 1;				//ʹ�ܶ�ʱ��1�ж�
}


void Sys_Init()
{
    WTST = 0;  //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
    EAXFR = 1; //��չ�Ĵ���(XFR)����ʹ��
    CKCON = 0; //��߷���XRAM�ٶ�

    P0M1 = 0x00;   P0M0 = 0x00;   //����Ϊ׼˫���
    // P1M1 = 0x00;   P1M0 = 0x00;   //����Ϊ׼˫���
    P1M0 = 0xff;   P1M1 = 0x00;   //����Ϊ�������
    P2M1 = 0x00;   P2M0 = 0x00;   //����Ϊ׼˫���
    P3M1 = 0x00;   P3M0 = 0x00;   //����Ϊ׼˫���
    P4M1 = 0x00;   P4M0 = 0x00;   //����Ϊ׼˫���
    P5M1 = 0x00;   P5M0 = 0x00;   //����Ϊ׼˫���
    P6M1 = 0x00;   P6M0 = 0x00;   //����Ϊ׼˫���
    P7M1 = 0x00;   P7M0 = 0x00;   //����Ϊ׼˫���
}

void main(void)
{
    EA = 1;                             //�����ж�
    Sys_Init();                         //ϵͳ��ʼ��
    Uart1_Init();                       //����1��ʼ��
    Uart_Port_Init();                   //���ڶ˿ڳ�ʼ��
    Timer1_Init();                      //��ʱ��0��ʼ��
    OLED_Init();                        //OLED��ʼ��
    OLED_LaserRanging_UI_Init();        //��ʼ��������UI
    LaserRanging_Init();
    while (1)
    {
        key1_check();                   //��鰴��1
        key2_check();                   //��鰴��2
        key3_check();                   //��鰴��3
        key4_check();                   //��鰴��4
        if(OLED_Refesh_Flag)            //ˢ��OLED
        {
            OLED_Refesh_Flag = 0;
            EA = 0;
            OLED_BuffShow_NoInterrupt();
            EA = 1;
        }
    }
}
