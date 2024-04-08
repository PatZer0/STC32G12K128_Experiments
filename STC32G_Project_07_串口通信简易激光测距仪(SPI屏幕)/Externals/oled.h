#ifndef __OLED_H
#define __OLED_H
#define MAIN_Fosc 33176600UL
#include "spi.h"


//-----------------------------------------特殊宏定义-----------------------------------------
#define OLED_CMD  0	                                        //写命令
#define OLED_DATA 1	                                        //写数据


//------------------------------------------引脚选择------------------------------------------
sbit    OLED_CS     = P2^2;                                 //使能脚 
sbit    OLED_DC     = P2^1;                                 //数据/地址选择
sbit    OLED_REST   = P2^0;                                 //复位脚

//------------------------------------------变量声明------------------------------------------
extern  unsigned char xdata ShowBUFF[8][128];               //OLED全局缓存


//------------------------------------------函数声明------------------------------------------
void delay_ms(unsigned int ms);
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);     //OLED地址/数据写入函数    
void OLED_Init(void);                                       //OLED初始化函数
void OLED_Set_Pos(unsigned char x, unsigned char y);        //OLED设置显示位置
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);       //OLED显示图片
void OLED_DisplayOn(void);                                  //开显示
void OLED_DisplayOff(void);                                 //关显示

void OLED_BuffClear(void);                                  //清屏
void OLED_BuffShow(void);                                   //OLED刷新显示
void OLED_BuffShowPoint(unsigned char x,unsigned char y);   //OLED显示一个点
void OLED_BuffShowLine( unsigned char x1, unsigned char y1, unsigned char x2,unsigned char y2);         //OLED显示一条线
void OLED_BuffShowRectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);    //OLED显示一个矩形
void OLED_BuffShowCircle(unsigned char x, unsigned char y, unsigned char r);                            //OLED显示一个圆形
void OLED_BuffShowChar(unsigned char x,unsigned char y,char asc, unsigned char mode);                   //OLED显示一个8*16字符
void OLED_BuffShowGBK(unsigned char x,unsigned char y,char *gbk,unsigned char mode);                    //OLED显示一个16*16汉字
void OLED_BuffShowString(unsigned char x,unsigned char y,char *s,unsigned char mode);                   //OLED显示一个16像素的汉字和字符
void OLED_BuffShowNum(unsigned char x,unsigned char y,long num,unsigned char mode);                     //OLED显示一个整形变量
void OLED_BuffShowNum02F(unsigned char x,unsigned char y,float num,unsigned char mode);                 //OLED显示一个两位小数变量
void OLED_BuffShowChar32(unsigned char  x,unsigned char  y,unsigned char num, unsigned char mode) ;     //OLED显示一个16*32的字符   
void OLED_BuffShowBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);  //OLED显示一个图片 

#endif