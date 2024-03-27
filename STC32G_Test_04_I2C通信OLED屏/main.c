/*-----------------------------------------------------------------------------------------

* ʱ��ѡ�� 22.1184MHZ

* �ο���STC32G����OLED12864��SPI��I2C���ַ�ʽ
https://www.stcaimcu.com/forum.php?mod=viewthread&tid=2592
(����: ��о��̳-STCȫ��32λ8051�����߻�����������)

* �޸� By PatZer0 @ 2024.03.27
    - ͳһ�����Ͷ��壬  BYTE/u8/uint8_t -> unsigned char��
                        WORD/u16/uint16_t -> unsigned int��
                        DWORD/u32/uint32_t -> unsigned long
    - ������include���֣��Ƴ����ļ��зǱ�Ҫ��ͷ�ļ����������ʱ��̫��warnings��

* ��ʾ��
    - Ĭ��ʹ��P3.2/P3.3�˿ڣ��޸��뿴 iic.c -> IIC_Init() ������
    - �������ͼƬ������ʹ�� https://javl.github.io/image2cpp/ ��ͼƬתΪ���飬ע�� 
      Draw Mode ��ҪΪ Vertical������ͼƬ��ʾ��������Ȼ����ӵ� pic.h �м��ɡ�

* ע�⣺���������ѧϰ�ο�������������ҵ��;��
-----------------------------------------------------------------------------------------*/

#include "oled.h"
#include "pic.h"
#include <STC32G.H>

#define  DELAY_TIME   2000

// ���尴������
sbit key1 = P2^0;
sbit key2 = P2^1;
sbit key3 = P2^2;
sbit key4 = P2^3;
unsigned char key1_up_store = 1; // ����1״̬�ݴ�
unsigned char key2_up_store = 1; // ����2״̬�ݴ�
unsigned char key3_up_store = 1; // ����3״̬�ݴ�
unsigned char key4_up_store = 1; // ����4״̬�ݴ�

unsigned int counter = 0;
unsigned char current_mode = 0;

void OLED_Loading()
{
    OLED_BuffClear();                               //���ȫ������
    OLED_BuffShowString(0,3,"   PLEASE WAIT  ",1);  //��ʾ����
    OLED_BuffShow();                                //������д����ʾ����ʾ
}

void key1_short_press()
{
    // �̰���1����
    current_mode = 1;
    OLED_Loading();                                                                 //��ʾ������
    OLED_BuffClear();                                                               //���ȫ������
    OLED_BuffShowString(0,0,"����STC32G12K128",0);                                  //��ʾ����
    OLED_BuffShowString(0,2,"The quick brown fox jumps over the lazy dog.",0);      //��ʾ����
    OLED_BuffShow();                                                                //������д����ʾ����ʾ
}

void key2_short_press()
{
    // �̰���2����
    current_mode = 2;
    OLED_Loading();                                 //��ʾ������
    delay_ms(DELAY_TIME);                           //��ʱ
    OLED_BuffClear();                               //���ȫ������
    OLED_BuffShowBMP(0,0,128,8,OLED12864_ZWU_LOGO); //��ʾLOGOͼƬ
    OLED_BuffShow();                                //������д����ʾ����ʾ
    delay_ms(DELAY_TIME);                           //��ʱ
}

void key3_short_press()
{
    // �̰���3����
    if (current_mode != 3)
    {
        // ��ǰ���ڼ�����ģʽ����ʼ��
        OLED_Loading();                                 //��ʾ������
        // ��ʾ��ʱ��UI
        OLED_BuffClear();                               //���ȫ������
        OLED_BuffShowString(0,0,"[Simple Counter]",0);  //��ʾ����
        OLED_BuffShowString(0,3,"   Press Key3",0);     //��ʾ����
        OLED_BuffShow();                                //������д����ʾ����ʾ
        counter = 0;                                    //����������
        current_mode = 3;                               //���������ģʽ
    }
    else
    {
        counter++;                                      //��������1
        if (counter > 9) counter = 0;                   //���������
        OLED_BuffClear();                               //���ȫ������
        OLED_BuffShowString(0,0,"[Simple Counter]",0);  //��ʾ����
        OLED_BuffShowChar32(58,3,counter,0);            //��ʾ����
        OLED_BuffShow();                                //������д����ʾ����ʾ
    }
}

void key4_short_press()
{
    // �̰���4����
    current_mode = 4;
}

void key1_check() 
{
    if(key1 == 0 && key1_up_store == 1)
    {
        key1_up_store = 0; // ��ǰ���������
    }
    if(key1 == 1 && key1_up_store == 0)
    {
        key1_up_store = 1; // ��ǰ������ͷ�
        key1_short_press(); // �����̰���1����
    }
}

void key2_check()
{
    if(key2 == 0 && key2_up_store == 1)
    {
        key2_up_store = 0; // ��ǰ���������
    }
    if(key2 == 1 && key2_up_store == 0)
    {
        key2_up_store = 1; // ��ǰ������ͷ�
        key2_short_press(); // �����̰���2����
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

void sys_init()
{
    WTST = 0;  //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
    EAXFR = 1; //��չ�Ĵ���(XFR)����ʹ��
    CKCON = 0; //��߷���XRAM�ٶ�

    P0M1 = 0x00;   P0M0 = 0x00;   //����Ϊ׼˫���
    P1M1 = 0x00;   P1M0 = 0x00;   //����Ϊ׼˫���
    P2M1 = 0x00;   P2M0 = 0x00;   //����Ϊ׼˫���
    P3M1 = 0x00;   P3M0 = 0x00;   //����Ϊ׼˫���
    P4M1 = 0x00;   P4M0 = 0x00;   //����Ϊ׼˫���
    P5M1 = 0x00;   P5M0 = 0x00;   //����Ϊ׼˫���
    P6M1 = 0x00;   P6M0 = 0x00;   //����Ϊ׼˫���
    P7M1 = 0x00;   P7M0 = 0x00;   //����Ϊ׼˫���
}

void main(void)
{
    unsigned char i;
    float dat = -12.345;

    sys_init();                         //ϵͳ��ʼ��
    EA = 1;                             //�����ж�
    OLED_Init();                        //OLED��ʼ��

    OLED_BuffClear();                               //���ȫ������
    OLED_BuffShowString(0,0,"Press any key to",0);  //��ʾ����
    OLED_BuffShowString(0,2,"start...",0);          //��ʾ����
    OLED_BuffShow();                                //������д����ʾ����ʾ

    while (1)
    {
        key1_check();                    //��鰴��1
        key2_check();                    //��鰴��2
        key3_check();                    //��鰴��3
        key4_check();                    //��鰴��4

// =============================================================================================
// ���¾�ΪԴ�����Դ��Ĳ��Դ��룬����Ҫʹ�ÿ���ȡ��ע��
// //-----------------------------------------��ʾͼƬ-----------------------------------------   
    // OLED_BuffClear();                                                       //���ȫ������
    // OLED_BuffShowBMP(0,0,128,8,OLED12864_IMG);                              //��ʾLOGOͼƬ
    // OLED_BuffShow();                                                        //������д����ʾ����ʾ
    // delay_ms(DELAY_TIME);                                                   //��ʱ	

// //-----------------------------------------��ʾ�ַ�-----------------------------------------   
//     OLED_BuffClear();                                                       //���ȫ������
//     OLED_BuffShowString(0,0,"HEELO ,WELCOME TO USE STC32&OLED12864!",0);    //��ʾ����
//     OLED_BuffShow();                                                        //������д����ʾ����ʾ
//     delay_ms(DELAY_TIME);                                                   //��ʱ	
    
// //-----------------------------------------��ʾ����-----------------------------------------   
//     OLED_BuffClear();                                                       //���ȫ������
//     OLED_BuffShowString(0,0,"���,��ӭʹ��STC ��\xfd��OLED12864������",0);  //��ʾ����
//     OLED_BuffShow();                                                        //������д����ʾ����ʾ
//     delay_ms(DELAY_TIME);                                                   //��ʱ	       
    
// //-----------------------------------------��ʾ��-----------------------------------------   
//     OLED_BuffClear();                                                       //���ȫ������
//     OLED_BuffShowString(32,0,"�������");                                   //��ʾ����
//     OLED_BuffShow();                                                        //������д����ʾ����ʾ	
//     for( i=0;i<64;i++ )                                                     //ѭ��64��
//     {
//         OLED_BuffShowPoint(i*2,25);                                         //��ʾһ����
//         OLED_BuffShow();                                                    //ˢ����ʾ
//         delay_ms(50);                                                       //��ʱ
//     }
    
// //-----------------------------------------��ʾ��-----------------------------------------   
//     OLED_BuffClear();                                                       //���ȫ������
//     OLED_BuffShowString(32,0,"���߲���");                                   //��ʾ����
//     OLED_BuffShow();                                                        //������д����ʾ����ʾ	
//     for( i=0;i<6;i++ )                                                      //ѭ��6��
//     {
//         OLED_BuffShowLine(4,16+8*i,124,16+8*i);                             //��ʾ����6����
//         OLED_BuffShow();                                                    //ˢ����ʾ
//         delay_ms(100);
//     }        
//     for( i=0;i<16;i+=1 )                                                    //ѭ��16��
//     {
//         OLED_BuffShowLine(8*i+4,16,8*i+4,56);                               //��ʾ16������
//         OLED_BuffShow();                                                    //ˢ����ʾ
//         delay_ms(100);
//     }  

// //-----------------------------------------��ʾԲ-----------------------------------------   
//     OLED_BuffClear();                                                       //���ȫ������
//     OLED_BuffShowString(32,0,"��Բ����");                                   //��ʾ����
//     OLED_BuffShow();                                                        //������д����ʾ����ʾ	
//     for( i=0;i<6;i++ )                                                      //ѭ��6��
//     {
//        OLED_BuffShowCircle(64,40,5+i*3);                                    //��һ��Բ    
//         OLED_BuffShow();                                                    //ˢ����ʾ
//         delay_ms(250);
//     }         

// //-----------------------------------------��ʾ32����-----------------------------------------   
//     OLED_BuffClear();                                                       //���ȫ������
//     OLED_BuffShowString(32,0,"���ֲ���");                                   //��ʾ����
//     OLED_BuffShowChar32(24,3,1,0);                                          //��ʾ����1
//     OLED_BuffShowChar32(40,3,0,0);                                          //��ʾ����0
//     OLED_BuffShowChar32(56,3,10,0);                                         //��ʾ��
//     OLED_BuffShowChar32(72,3,5,0);                                          //��ʾ����5
//     OLED_BuffShowChar32(88,3,5,0);                                          //��ʾ����5
//     OLED_BuffShow();                                                        //������д����ʾ����ʾ	
//     delay_ms(DELAY_TIME);                                                   //��ʱ	 

// //-----------------------------------------���Բ���-----------------------------------------   
//     OLED_BuffClear();                                                       //���ȫ������
//     OLED_BuffShowString(32,0,"���Բ���",0);                                 //��ʾ����
//     OLED_BuffShowString(16,2,"  ������\xfd��  ",0);                         //��ʾ����
//     OLED_BuffShowString(16,4,"  ���Ƿ���  ",1);                             //��ʾ����
//     OLED_BuffShow();                                                        //������д����ʾ����ʾ
//     delay_ms(DELAY_TIME);   

// //-----------------------------------------������ʾ-----------------------------------------   
//     OLED_BuffClear();                                                       //���ȫ������
//     OLED_BuffShowString(32,0,"������ʾ",0);                                 //��ʾ����
//     OLED_BuffShowNum(0,2,-25169,0) ;                                        //��ʾ����
//     OLED_BuffShowNum02F(0,4,3.14159,0);                                     //��ʾ����
//     OLED_BuffShowNum02F(0,6,dat,1);                                         //��ʾ����
//     OLED_BuffShow();                                                        //������д����ʾ����ʾ
//     delay_ms(DELAY_TIME);   
    }
}
