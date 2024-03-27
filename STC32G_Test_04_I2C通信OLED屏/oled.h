#ifndef __OLED_H
#define __OLED_H

#include "iic.h"
#define MAIN_Fosc       24000000L   //定义主时钟
//-----------------------------------------����궨��-----------------------------------------
#define OLED_CMD  0	                                        //д����
#define OLED_DATA 1	                                        //д����


//------------------------------------------����ѡ��------------------------------------------


//------------------------------------------��������------------------------------------------
extern  unsigned char xdata ShowBUFF[8][128];               //OLEDȫ�ֻ���


//------------------------------------------��������------------------------------------------
void delay_ms(unsigned int ms);
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);                           //OLED��ַ/����д�뺯��    
void OLED_Init(void);                                       //OLED��ʼ������
void OLED_Set_Pos(unsigned char x, unsigned char y);        //OLED������ʾλ��
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);       //OLED��ʾͼƬ
void OLED_DisplayOn(void);                                  //����ʾ
void OLED_DisplayOff(void);                                 //����ʾ

void OLED_BuffClear(void);                                  //����
void OLED_BuffShow(void);                                   //OLEDˢ����ʾ
void OLED_BuffShowPoint(unsigned char x,unsigned char y);   //OLED��ʾһ����
void OLED_BuffShowLine( unsigned char x1, unsigned char y1, unsigned char x2,unsigned char y2);         //OLED��ʾһ����
void OLED_BuffShowRectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);    //OLED��ʾһ������
void OLED_BuffShowCircle(unsigned char x, unsigned char y, unsigned char r);                 //OLED��ʾһ��Բ��
void OLED_BuffShowChar(unsigned char x,unsigned char y,char asc, unsigned char mode);        //OLED��ʾһ��8*16�ַ�
void OLED_BuffShowGBK(unsigned char x,unsigned char y,char *gbk,unsigned char mode);         //OLED��ʾһ��16*16����
void OLED_BuffShowString(unsigned char x,unsigned char y,char *s,unsigned char mode);        //OLED��ʾһ��16���صĺ��ֺ��ַ�
void OLED_BuffShowNum(unsigned char x,unsigned char y,long num,unsigned char mode);          //OLED��ʾһ�����α���
void OLED_BuffShowNum02F(unsigned char x,unsigned char y,float num,unsigned char mode);      //OLED��ʾһ����λС������
void OLED_BuffShowChar32(unsigned char  x,unsigned char  y,unsigned char num, unsigned char mode) ;     //OLED��ʾһ��16*32���ַ�   
void OLED_BuffShowBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);  //OLED��ʾһ��ͼƬ 
    
#endif



