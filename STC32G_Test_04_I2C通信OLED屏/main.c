/*-----------------------------------------------------------------------------------------

* 时钟选择 22.1184MHZ

* 参考：STC32G驱动OLED12864，SPI或I2C两种方式
https://www.stcaimcu.com/forum.php?mod=viewthread&tid=2592
(出处: 国芯论坛-STC全球32位8051爱好者互助交流社区)

* 修改 By PatZer0 @ 2024.03.27
    - 统一了类型定义，  BYTE/u8/uint8_t -> unsigned char，
                        WORD/u16/uint16_t -> unsigned int，
                        DWORD/u32/uint32_t -> unsigned long
    - 精简了include部分，移除了文件中非必要的头文件，否则编译时有太多warnings；

* 提示：
    - 默认使用P3.2/P3.3端口，修改请看 iic.c -> IIC_Init() 函数。
    - 如需添加图片，可以使用 https://javl.github.io/image2cpp/ 将图片转为数组，注意 
      Draw Mode 需要为 Vertical，否则图片显示不正常，然后添加到 pic.h 中即可。

* 注意：本程序仅供学习参考，请勿用于商业用途。
-----------------------------------------------------------------------------------------*/

#include "oled.h"
#include "pic.h"
#include <STC32G.H>

#define  DELAY_TIME   2000

// 定义按键引脚
sbit key1 = P2^0;
sbit key2 = P2^1;
sbit key3 = P2^2;
sbit key4 = P2^3;
unsigned char key1_up_store = 1; // 按键1状态暂存
unsigned char key2_up_store = 1; // 按键2状态暂存
unsigned char key3_up_store = 1; // 按键3状态暂存
unsigned char key4_up_store = 1; // 按键4状态暂存

unsigned int counter = 0;
unsigned char current_mode = 0;

void OLED_Loading()
{
    OLED_BuffClear();                               //清除全部缓存
    OLED_BuffShowString(0,3,"   PLEASE WAIT  ",1);  //显示数据
    OLED_BuffShow();                                //将缓存写入显示屏显示
}

void key1_short_press()
{
    // 短按键1功能
    current_mode = 1;
    OLED_Loading();                                                                 //显示加载中
    OLED_BuffClear();                                                               //清除全部缓存
    OLED_BuffShowString(0,0,"测试STC32G12K128",0);                                  //显示数据
    OLED_BuffShowString(0,2,"The quick brown fox jumps over the lazy dog.",0);      //显示数据
    OLED_BuffShow();                                                                //将缓存写入显示屏显示
}

void key2_short_press()
{
    // 短按键2功能
    current_mode = 2;
    OLED_Loading();                                 //显示加载中
    delay_ms(DELAY_TIME);                           //延时
    OLED_BuffClear();                               //清除全部缓存
    OLED_BuffShowBMP(0,0,128,8,OLED12864_ZWU_LOGO); //显示LOGO图片
    OLED_BuffShow();                                //将缓存写入显示屏显示
    delay_ms(DELAY_TIME);                           //延时
}

void key3_short_press()
{
    // 短按键3功能
    if (current_mode != 3)
    {
        // 当前不在计数器模式，初始化
        OLED_Loading();                                 //显示加载中
        // 显示计时器UI
        OLED_BuffClear();                               //清除全部缓存
        OLED_BuffShowString(0,0,"[Simple Counter]",0);  //显示数据
        OLED_BuffShowString(0,3,"   Press Key3",0);     //显示数据
        OLED_BuffShow();                                //将缓存写入显示屏显示
        counter = 0;                                    //计数器清零
        current_mode = 3;                               //进入计数器模式
    }
    else
    {
        counter++;                                      //计数器加1
        if (counter > 9) counter = 0;                   //计数器溢出
        OLED_BuffClear();                               //清除全部缓存
        OLED_BuffShowString(0,0,"[Simple Counter]",0);  //显示数据
        OLED_BuffShowChar32(58,3,counter,0);            //显示数字
        OLED_BuffShow();                                //将缓存写入显示屏显示
    }
}

void key4_short_press()
{
    // 短按键4功能
    current_mode = 4;
}

void key1_check() 
{
    if(key1 == 0 && key1_up_store == 1)
    {
        key1_up_store = 0; // 标记按键被按下
    }
    if(key1 == 1 && key1_up_store == 0)
    {
        key1_up_store = 1; // 标记按键被释放
        key1_short_press(); // 触发短按键1功能
    }
}

void key2_check()
{
    if(key2 == 0 && key2_up_store == 1)
    {
        key2_up_store = 0; // 标记按键被按下
    }
    if(key2 == 1 && key2_up_store == 0)
    {
        key2_up_store = 1; // 标记按键被释放
        key2_short_press(); // 触发短按键2功能
    }
}

void key3_check()
{
    if(key3 == 0 && key3_up_store == 1)
    {
        key3_up_store = 0; // 标记按键被按下
    }
    if(key3 == 1 && key3_up_store == 0)
    {
        key3_up_store = 1; // 标记按键被释放
        key3_short_press(); // 触发短按键3功能
    }
}

void key4_check()
{
    if(key4 == 0 && key4_up_store == 1)
    {
        key4_up_store = 0; // 标记按键被按下
    }
    if(key4 == 1 && key4_up_store == 0)
    {
        key4_up_store = 1; // 标记按键被释放
        key4_short_press(); // 触发短按键4功能
    }
}

void sys_init()
{
    WTST = 0;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    EAXFR = 1; //扩展寄存器(XFR)访问使能
    CKCON = 0; //提高访问XRAM速度

    P0M1 = 0x00;   P0M0 = 0x00;   //设置为准双向口
    P1M1 = 0x00;   P1M0 = 0x00;   //设置为准双向口
    P2M1 = 0x00;   P2M0 = 0x00;   //设置为准双向口
    P3M1 = 0x00;   P3M0 = 0x00;   //设置为准双向口
    P4M1 = 0x00;   P4M0 = 0x00;   //设置为准双向口
    P5M1 = 0x00;   P5M0 = 0x00;   //设置为准双向口
    P6M1 = 0x00;   P6M0 = 0x00;   //设置为准双向口
    P7M1 = 0x00;   P7M0 = 0x00;   //设置为准双向口
}

void main(void)
{
    unsigned char i;
    float dat = -12.345;

    sys_init();                         //系统初始化
    EA = 1;                             //开总中断
    OLED_Init();                        //OLED初始化

    OLED_BuffClear();                               //清除全部缓存
    OLED_BuffShowString(0,0,"Press any key to",0);  //显示数据
    OLED_BuffShowString(0,2,"start...",0);          //显示数据
    OLED_BuffShow();                                //将缓存写入显示屏显示

    while (1)
    {
        key1_check();                    //检查按键1
        key2_check();                    //检查按键2
        key3_check();                    //检查按键3
        key4_check();                    //检查按键4

// =============================================================================================
// 以下均为源代码自带的测试代码，如需要使用可以取消注释
// //-----------------------------------------显示图片-----------------------------------------   
    // OLED_BuffClear();                                                       //清除全部缓存
    // OLED_BuffShowBMP(0,0,128,8,OLED12864_IMG);                              //显示LOGO图片
    // OLED_BuffShow();                                                        //将缓存写入显示屏显示
    // delay_ms(DELAY_TIME);                                                   //延时	

// //-----------------------------------------显示字符-----------------------------------------   
//     OLED_BuffClear();                                                       //清除全部缓存
//     OLED_BuffShowString(0,0,"HEELO ,WELCOME TO USE STC32&OLED12864!",0);    //显示数据
//     OLED_BuffShow();                                                        //将缓存写入显示屏显示
//     delay_ms(DELAY_TIME);                                                   //延时	
    
// //-----------------------------------------显示汉字-----------------------------------------   
//     OLED_BuffClear();                                                       //清除全部缓存
//     OLED_BuffShowString(0,0,"你好,欢迎使用STC 驱\xfd动OLED12864的历程",0);  //显示数据
//     OLED_BuffShow();                                                        //将缓存写入显示屏显示
//     delay_ms(DELAY_TIME);                                                   //延时	       
    
// //-----------------------------------------显示点-----------------------------------------   
//     OLED_BuffClear();                                                       //清除全部缓存
//     OLED_BuffShowString(32,0,"画点测试");                                   //显示数据
//     OLED_BuffShow();                                                        //将缓存写入显示屏显示	
//     for( i=0;i<64;i++ )                                                     //循环64次
//     {
//         OLED_BuffShowPoint(i*2,25);                                         //显示一个点
//         OLED_BuffShow();                                                    //刷新显示
//         delay_ms(50);                                                       //延时
//     }
    
// //-----------------------------------------显示线-----------------------------------------   
//     OLED_BuffClear();                                                       //清除全部缓存
//     OLED_BuffShowString(32,0,"画线测试");                                   //显示数据
//     OLED_BuffShow();                                                        //将缓存写入显示屏显示	
//     for( i=0;i<6;i++ )                                                      //循环6次
//     {
//         OLED_BuffShowLine(4,16+8*i,124,16+8*i);                             //显示横着6条线
//         OLED_BuffShow();                                                    //刷新显示
//         delay_ms(100);
//     }        
//     for( i=0;i<16;i+=1 )                                                    //循环16次
//     {
//         OLED_BuffShowLine(8*i+4,16,8*i+4,56);                               //显示16条数线
//         OLED_BuffShow();                                                    //刷新显示
//         delay_ms(100);
//     }  

// //-----------------------------------------显示圆-----------------------------------------   
//     OLED_BuffClear();                                                       //清除全部缓存
//     OLED_BuffShowString(32,0,"画圆测试");                                   //显示数据
//     OLED_BuffShow();                                                        //将缓存写入显示屏显示	
//     for( i=0;i<6;i++ )                                                      //循环6次
//     {
//        OLED_BuffShowCircle(64,40,5+i*3);                                    //画一个圆    
//         OLED_BuffShow();                                                    //刷新显示
//         delay_ms(250);
//     }         

// //-----------------------------------------显示32大字-----------------------------------------   
//     OLED_BuffClear();                                                       //清除全部缓存
//     OLED_BuffShowString(32,0,"大字测试");                                   //显示数据
//     OLED_BuffShowChar32(24,3,1,0);                                          //显示数字1
//     OLED_BuffShowChar32(40,3,0,0);                                          //显示数字0
//     OLED_BuffShowChar32(56,3,10,0);                                         //显示：
//     OLED_BuffShowChar32(72,3,5,0);                                          //显示数字5
//     OLED_BuffShowChar32(88,3,5,0);                                          //显示数字5
//     OLED_BuffShow();                                                        //将缓存写入显示屏显示	
//     delay_ms(DELAY_TIME);                                                   //延时	 

// //-----------------------------------------反显测试-----------------------------------------   
//     OLED_BuffClear();                                                       //清除全部缓存
//     OLED_BuffShowString(32,0,"反显测试",0);                                 //显示数据
//     OLED_BuffShowString(16,2,"  这是正\xfd显  ",0);                         //显示数据
//     OLED_BuffShowString(16,4,"  这是反显  ",1);                             //显示数据
//     OLED_BuffShow();                                                        //将缓存写入显示屏显示
//     delay_ms(DELAY_TIME);   

// //-----------------------------------------变量显示-----------------------------------------   
//     OLED_BuffClear();                                                       //清除全部缓存
//     OLED_BuffShowString(32,0,"变量显示",0);                                 //显示数据
//     OLED_BuffShowNum(0,2,-25169,0) ;                                        //显示数据
//     OLED_BuffShowNum02F(0,4,3.14159,0);                                     //显示数据
//     OLED_BuffShowNum02F(0,6,dat,1);                                         //显示数据
//     OLED_BuffShow();                                                        //将缓存写入显示屏显示
//     delay_ms(DELAY_TIME);   
    }
}
