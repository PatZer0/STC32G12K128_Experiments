#include "oled.h"
#include "font.h"
#include <STDIO.H>
#include <STC32G.H>
#include <STRING.H>

unsigned char xdata ShowBUFF[8][128];                           //OLED全局缓存

//========================================================================
// 函数名称: delay
// 函数功能: OLED驱动用的延时
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注:
//========================================================================
void delay(void)
{
    int xdata i;
    
    for (i=0; i<100; i++);
}

//========================================================================
// 函数名称: delay_ms
// 函数功能: ms延时函数
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注:
//========================================================================
void delay_ms(unsigned int ms)
{
     unsigned int xdata i;
     do{
          i = MAIN_Fosc / 6000;
          while(--i);   //6T per loop
     }while(--ms);
}

//========================================================================
// 函数名称: OLED_WR_Byte
// 函数功能: OLED地址/数据写入函数
// 入口参数: @dat:数据    @cmd:命令
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注:
//========================================================================
void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
    unsigned char udat[1] ;
    udat[0] = dat;
    if(cmd)
    {
        OLED_DC = 1;
    }
    else
    {
        OLED_DC = 0;
    }
    OLED_CS = 0;
    SPI_WriteByte(dat);
    delay();
    OLED_CS = 1;
    OLED_DC = 1;
}

//========================================================================
// 函数名称: OLED_Init
// 函数功能: OLED初始化函数
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注:
//========================================================================
void OLED_Init(void)
{
    SPI_init();
    delay_ms(100);
    OLED_REST = 1;
    delay_ms(200);
    OLED_REST=0;//复位
    delay_ms(200);
    OLED_REST = 1;

    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x00,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
    OLED_WR_Byte(0xAF,OLED_CMD);
    
    OLED_DisplayOn();
}


//========================================================================
// 函数名称: OLED_Set_Pos
// 函数功能: OLED设置显示位置
// 入口参数: @x:x坐标     @y:y坐标
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注:
//========================================================================
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte((unsigned char)(0xb0+y),OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   


//========================================================================
// 函数名称: OLED_DrawBMP
// 函数功能: OLED显示图片
// 入口参数: @x0:x起点  @y0:y起点    @x1:x终点   @y1:y终点  @BMP：显示数组
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注:
//========================================================================
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
    unsigned int xdata j=0;
    unsigned char xdata x,y;

    for(y=y0;y<(y1+y0);y++)
    {
        OLED_Set_Pos(x0,y);
        for(x=0;x<x1;x++)
        {      
            OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
        }
    }
} 

//========================================================================
// 函数名称: OLED_DisplayOn
// 函数功能: OLED开显示
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注:
//========================================================================
void OLED_DisplayOn(void)     //开显示
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
    OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕       
}
  
//========================================================================
// 函数名称: OLED_DisplayOff
// 函数功能: OLED关显示
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注:
//========================================================================
void OLED_DisplayOff(void)     //关显示
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
    OLED_WR_Byte(0xAF,OLED_CMD);//关闭屏幕        
}

//========================================================================
// 函数名称: OLED_LightSet
// 函数功能: OLED亮度设置
// 入口参数: @num： 0-255
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注:
//========================================================================
void OLED_LightSet(unsigned char num)     //亮度设置
{
    OLED_WR_Byte(0x81,OLED_CMD);//
    OLED_WR_Byte(num,OLED_CMD);//  
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x20,OLED_CMD);//Set VCOM Deselect Level   
}


//========================================================================
// 函数名称: OLED_BuffClear
// 函数功能: OLED清空缓存
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 写入的是缓存，需要电调用OLED_BuffShow才能显示出来
//========================================================================
void OLED_BuffClear(void)     //清屏
{
    memset(ShowBUFF,0,128*8);
}

//========================================================================
// 函数名称: OLED_BuffShow
// 函数功能: OLED刷新显示
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2024 - 4 - 3
// 当前作者: ZCF, PatZer0
// 其他备注:
//========================================================================
void OLED_BuffShow(void)     
{
    unsigned char xdata i,n,j;
    for(i=0;i<8;i++)
    {
        j = 0xb0 + i;             //C251更加严谨，这样写以避免warning
        OLED_WR_Byte(j,OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址    // 修改低列起始，否则测试用屏幕显示不全
        OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址

        for(n=0;n<128;n++)
            OLED_WR_Byte(ShowBUFF[i][n],OLED_DATA);
    }
}

//========================================================================
// 函数名称: OLED_BuffShowPoint
// 函数功能: OLED显示一个点
// 入口参数: @x：x起点   @y:y起点
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 写入的是缓存，需要电调用OLED_BuffShow才能显示出来
//========================================================================
void OLED_BuffShowPoint(unsigned char x,unsigned char y)     //OLED显示一个点
{
    ShowBUFF[y/8][x] |= 1<<(y%8);
}

//========================================================================
// 函数名称: OLED_BuffShowPoint
// 函数功能: OLED显示一条线
// 入口参数: @x1：x起点  @y1：y起点  @x2：x终点   @y2：y终点
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 写入的是缓存，需要电调用OLED_BuffShow才能显示出来
//========================================================================
void OLED_BuffShowLine( unsigned char x1, unsigned char y1, unsigned char x2,unsigned char y2)
{
    unsigned char x,y;
    if( x1>x2 )
    {
        x=x1;x1=x2;x2=x;
        y=y1;y1=y2;y2=y;
    }
    if(x1!=x2)
    {
        for( x = x1; x <= x2; x++ )
        {
            if( y2>y1 )
                OLED_BuffShowPoint(x, (unsigned char)(y1+(unsigned int)(y2-y1)*(unsigned int)x/(unsigned int)(x2-x1)));
            else
                OLED_BuffShowPoint(x, (unsigned char)(y1-(unsigned int)(y1-y2)*(unsigned int)x/(unsigned int)(x2-x1)));
        }        
    }
    else
    {
        if( y1>y2 )
        {
            for( y = y2; y <= y1; y++ )
               OLED_BuffShowPoint(x1, y); 
        }
        else
        {
            for( y = y1; y <= y2; y++ )
               OLED_BuffShowPoint(x1, y);             
        }
    }
}

//========================================================================
// 函数名称: OLED_BuffShowRectangle
// 函数功能: OLED显示一个矩形
// 入口参数: @x1：x起点  @y1：y起点  @x2：x终点   @y2：y终点
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 写入的是缓存，需要电调用OLED_BuffShow才能显示出来
//========================================================================
void OLED_BuffShowRectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    OLED_BuffShowLine(x1, y1, x2, y1);
    OLED_BuffShowLine(x1, y1, x1, y2);
    OLED_BuffShowLine(x1, y2, x2, y2);
    OLED_BuffShowLine(x2, y1, x2, y2);
}

//========================================================================
// 函数名称: OLED_BuffShowCircle
// 函数功能: OLED显示一个圆形
// 入口参数: @x：x点  @y：y点  @r:半径
// 函数返回: 无
// 当前版本: VER1.1
// 修改日期: 2024 - 3 - 28
// 当前作者: ZCF, PatZer0
// 其他备注: 写入的是缓存，需要电调用OLED_BuffShow才能显示出来
//========================================================================
void OLED_BuffShowCircle(unsigned char x, unsigned char y, unsigned char r)
{
    int a, b;
    int di;
    unsigned char x_add_a, x_add_b, x_sub_a, x_sub_b, y_add_a, y_add_b, y_sub_a, y_sub_b;

    a = 0;
    b = r;
    di = 3 - (r << 1);       //判断下个点位置的标志
 
    while (a <= b)
    {
        x_add_a = x + a;
        x_add_b = x + b;
        x_sub_a = x - a;
        x_sub_b = x - b;
        y_add_a = y + a;
        y_add_b = y + b;
        y_sub_a = y - a;
        y_sub_b = y - b;
        OLED_BuffShowPoint(x_add_a, y_sub_b);        //5
        OLED_BuffShowPoint(x_add_b, y_sub_a);        //0
        OLED_BuffShowPoint(x_sub_b, y_add_a);        //4
        OLED_BuffShowPoint(x_sub_a, y_add_b);        //6
        OLED_BuffShowPoint(x_sub_a, y_sub_b);        //1
        OLED_BuffShowPoint(x_add_b, y_add_a);
        OLED_BuffShowPoint(x_sub_b, y_sub_a);        //2
        OLED_BuffShowPoint(x_add_a, y_add_b);        //7
        //重写代码，避免C251报Warning

        a++;
 
        //使用Bresenham算法画圆
        if (di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

//========================================================================
// 函数名称: OLED_BuffShowChar
// 函数功能: OLED显示一个8*16的字符
// 入口参数: @x：x点  @y：y点  @asc:字符   @mode：0正显  1反显
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 写入的是缓存，需要电调用OLED_BuffShow才能显示出来
//========================================================================
void OLED_BuffShowChar(unsigned char x,unsigned char y,char asc, unsigned char mode)     //OLED显示一个8*16字符
{
    unsigned char j,k;
    for( j=0;j<2;j++ )
    {
        for( k=0;k<8;k++ )
        {
            if(mode==0)            
                ShowBUFF[j+y][x+k]=Ascll_16[(asc-' ')*2][j*8+k];
            else
                ShowBUFF[j+y][x+k]=~Ascll_16[(asc-' ')*2][j*8+k];
        }
    }    
}

//========================================================================
// 函数名称: OLED_BuffShowGBK
// 函数功能: OLED显示汉字
// 入口参数: @x：x点  @y：y点  @gbk:汉字   @mode：0正显  1反显
// 函数返回: 无
// 当前版本: VER1.1
// 修改日期: 2024 - 3 - 29
// 当前作者: ZCF, PatZer0
// 其他备注: 写入的是缓存，需要电调用OLED_BuffShow才能显示出来
//          用到的汉字需要自己取模放到字库里，字库多的话，这里这个循环变量要放大
//          字库文件font.h里有汉字的GBK16数组，可以自己修改
//          原代码使用固定的遍历次数，这里直接改为GBK16数组大小，确保所有字库都被遍历
//========================================================================
void OLED_BuffShowGBK(unsigned char x,unsigned char y,char *gbk,unsigned char mode)     //OLED显示一个16*16汉字
{
    unsigned char i;
    unsigned char j,k;
    for( i=0;i<sizeof(GBK16)/sizeof(GBK16[0]);i++ )
    {
        if((gbk[0] == GBK16[i].gbn_bum[0])
         &&(gbk[1] == GBK16[i].gbn_bum[1])
			)
        {
            for( j=0;j<2;j++ )
            {
                for( k=0;k<16;k++ )
                {
                    if( mode ==0 )
                        ShowBUFF[j+y][x+k]=GBK16[i].gbk_font[j*16+k];
                    else
                        ShowBUFF[j+y][x+k]=~GBK16[i].gbk_font[j*16+k];
                }
            }
            break;
        }
    }    
}

//========================================================================
// 函数名称: OLED_BuffShowString
// 函数功能: OLED显示字符串。汉字字符均可
// 入口参数: @x：x点  @y：y点  @s字符串   @mode：0正显  1反显
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 写入的是缓存，需要电调用OLED_BuffShow才能显示出来
//          用到的汉字需要自己取模放到字库里，字库多的话，这里这个循环变量要放大
//========================================================================
void OLED_BuffShowString(unsigned char x,unsigned char y,char *s,unsigned char mode)
{
    char s_num[2];
	while(*s != '\0')       //字符串不为空，循环
	{
        if ((unsigned char)*s < 0x80)     //根据输入数据的大小判断是字符还是汉字，
        {
            OLED_BuffShowChar(x,y,*s,mode);
            x+=8;
            s++;
        }
        else
        {
            s_num[0] = *s ;
            s_num[1] = *(s+1) ;
            OLED_BuffShowGBK(x,y,s_num,mode);
            x+=16;
            s+=2;
        }
		if(x>127)
        {
            x=0;
            y+=2;
        }
	}       
}

//========================================================================
// 函数名称: OLED_BuffShowNum
// 函数功能: OLED显示整形变量
// 入口参数: @x：x点  @y：y点  @num：整形变量   @mode：0正显  1反显
// 函数返回: 无
// 当前版本: VER1.1
// 修改日期: 2024 - 3 - 28
// 当前作者: ZCF, PatZer0
// 其他备注: 写入的是缓存，需要电调用OLED_BuffShow才能显示出来
//========================================================================
void OLED_BuffShowNum(unsigned char x,unsigned char y,long num,unsigned char mode)
{
    unsigned char xdata str[10];
    memset(str,0,10);
    sprintf(str,"%ld",num);
    OLED_BuffShowString(x,y,(unsigned char*)str, mode);
}

//========================================================================
// 函数名称: OLED_BuffShowNum02F
// 函数功能: OLED显示一个两位小数变量
// 入口参数: @x：x点  @y：y点  @num：小数变量   @mode：0正显  1反显
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 写入的是缓存，需要电调用OLED_BuffShow才能显示出来
//========================================================================
void OLED_BuffShowNum02F(unsigned char x,unsigned char y,float num,unsigned char mode)
{
    unsigned char xdata str[10];
    memset(str,0,10);
    sprintf(str,"%.2f",num);
    OLED_BuffShowString(x,y,(unsigned char*)str,mode);
}


//========================================================================
// 函数名称: OLED_BuffShowChar32
// 函数功能: OLED显示16*32的字符
// 入口参数: @x：x点  @y：y点  @num：显示第几个字符   @mode：0正显  1反显
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 写入的是缓存，需要电调用OLED_BuffShow才能显示出来
//          用到的需要自己取模哦
//========================================================================
void OLED_BuffShowChar32(unsigned char  x,unsigned char  y,unsigned char num, unsigned char mode)     
{
    unsigned char j,k;
    for( j=0;j<4;j++ )
    {
        for( k=0;k<16;k++ )
        {
            if( mode==0 )
                ShowBUFF[j+y][x+k]=lib_num1632[num][j*16+k];
            else
                ShowBUFF[j+y][x+k]=~lib_num1632[num][j*16+k];
        }
    }            
}


//========================================================================
// 函数名称: OLED_BuffShowBMP
// 函数功能: OLED缓存写入图片
// 入口参数: @x0:x起点  @y0:y起点    @x1:x终点   @y1:y终点  @BMP：显示数组
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注:
//========================================================================
void OLED_BuffShowBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
    unsigned int xdata num=0;
    unsigned char k,j;
    
    for( j=0;j<y1;j++ )
    {
        for( k=0;k<x1;k++ )
        {
            ShowBUFF[j+y0][x0+k]=BMP[num++];
        }
    }  
} 
 