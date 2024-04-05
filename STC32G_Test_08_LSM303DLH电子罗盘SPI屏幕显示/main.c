/*-----------------------------------------------------------------------------------------
* ������Ĵ��ļ����룡
* ���뱣��ΪĬ�ϱ���GB2312���������޷�������ʾ��

* ʱ��ѡ�� 33.1776MHz
* ����Ϊ P1.0/1.1/1.3/1.4

* --------------------------I2C-LSM303DLHģ��-�������̼��ٶȼ�-----------------------------
    - Ĭ��ʹ��P3.2/P3.3�˿ڣ��޸��뿴 iic.c -> IIC_Init() ������

* -----------------------------------SPI-OLED��ʾ��----------------------------------------
    - ������ĻĬ��ʹ�ö˿�Ϊ��
      D0 - P2.5(SCLK)
      D1 - P2.3(MOSI)
      CS - P2.2(CS)
      DC - P2.1(DC)
      RES - P2.0(RES)
      �޸��뿴 spi.h �� spi.c

* ��ʵ���ʵ��SPI-OLED��Ļ��LSM303DLHģ��Ĺ��ܣ�δ��ϼ�����ģ��
    
    ����Ӳ��ԭ�򣬰���KEY4-KEY1����Ļ�ĺ�����ʾ�����Ƿ��ģ�����Ļ���½ǰ�����Ӧ����KEY4��
    �����Ҫ�޸ģ�����ֱ���޸�sbit key�Ķ��塣

* ע�⣺���������ѧϰ�ο�������������ҵ��;��
-----------------------------------------------------------------------------------------*/
#include <STRING.H>
#include <STC32G.H>
#include "oled.h"
#include "pic.h"
#include "iic.h"
#include "lsm303.h"

// ���尴������
sbit key1 = P1^0;
sbit key2 = P1^1;
sbit key3 = P1^3;
sbit key4 = P1^4;
unsigned char key1_up_store = 1; // ����1״̬�ݴ�
unsigned char key2_up_store = 1; // ����2״̬�ݴ�
unsigned char key3_up_store = 1; // ����3״̬�ݴ�
unsigned char key4_up_store = 1; // ����4״̬�ݴ�

void OLED_LSM303DLH_UI_OnOff()
{
}

void OLED_LSM303DLH_UI_Range()
{
}

void OLED_LSM303DLH_UI_Refresh()
{
}

void OLED_LSM303DLH_UI_Init()
{
    OLED_BuffClear();                       //���ȫ������
    OLED_LSM303DLH_UI_Refresh();            //ˢ��UI
    OLED_LSM303DLH_UI_OnOff();              //��ʾ״̬
    OLED_LSM303DLH_UI_Range();              //��ʾ����ֵ
}

void OLED_Loading()
{
    OLED_BuffClear();                               //���ȫ������
    OLED_BuffShowString(0,3,"   PLEASE WAIT  ",1);  //��ʾ����
    OLED_BuffShow();                                //������д����ʾ����ʾ
}

void key1_short_press()
{
    // �̰���1����
}

void key2_short_press()
{
    // �̰���2����
}

void key3_short_press()
{
    // �̰���3����
}

void key4_short_press()
{
    // �̰���4����
}

void key1_check()
{
    if(key1 == 0 && key1_up_store == 1)
    {
        key1_up_store = 0; // ��ǰ���������
        OLED_BuffShowString(13*8,6,"[-]",1);  //��ʾ����
        OLED_BuffShow();                    //������д����ʾ����ʾ
    }
    if(key1 == 1 && key1_up_store == 0)
    {
        key1_up_store = 1; // ��ǰ������ͷ�
        key1_short_press(); // �����̰���1����
        OLED_BuffShowString(13*8,6,"[-]",0);  //��ʾ����
        OLED_BuffShow();                    //������д����ʾ����ʾ
    }
}

void key2_check()
{
    if(key2 == 0 && key2_up_store == 1)
    {
        key2_up_store = 0; // ��ǰ���������
        OLED_BuffShowString(10*8,6,"[+]",1);  //��ʾ����
        OLED_BuffShow();                    //������д����ʾ����ʾ
    }
    if(key2 == 1 && key2_up_store == 0)
    {
        key2_up_store = 1; // ��ǰ������ͷ�
        key2_short_press(); // �����̰���2����
        OLED_BuffShowString(10*8,6,"[+]",0);  //��ʾ����
        OLED_BuffShow();                    //������д����ʾ����ʾ
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
    }
    if(key4 == 1 && key4_up_store == 0)
    {
        key4_up_store = 1; // ��ǰ������ͷ�
        key4_short_press(); // �����̰���4����
    }
}

void Sys_Init()
{
    WTST = 0;  //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
    EAXFR = 1; //��չ�Ĵ���(XFR)����ʹ��
    CKCON = 0; //��߷���XRAM�ٶ�

    P0M1 = 0x00;   P0M0 = 0x00;   //����Ϊ׼˫���
    P1M1 = 0x00;   P1M0 = 0x00;   //����Ϊ׼˫���
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
    SPI_init();                         //SPI��ʼ��
    OLED_Init();                        //OLED��ʼ��
    OLED_BuffClear();                   //���ȫ������
    OLED_BuffShow();                    //������д����ʾ����ʾ
    IIC_Init();                         //IIC��ʼ��
    LSM303_Init();                      //LSM303��ʼ��
    while (1)
    {
        key1_check();                   //��鰴��1
        key2_check();                   //��鰴��2
        key3_check();                   //��鰴��3
        key4_check();                   //��鰴��4
        LSM303_Get_Acc_X();             //��ȡ���ٶ�X������
        LSM303_Get_Acc_Y();             //��ȡ���ٶ�Y������
        LSM303_Get_Acc_Z();             //��ȡ���ٶ�Z������
        delay_ms(100);                  //��ʱ100ms
    }
}
