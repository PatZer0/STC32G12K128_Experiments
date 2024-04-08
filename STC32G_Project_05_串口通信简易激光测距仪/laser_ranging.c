/*

激光测距模块

通信参数：波特率9600bps, 8位数据位，1位起始位，1位停止位，无奇偶校验
电气参数：3.0~3.3V 100mA

功能描述                       | 指令                      | 说明                                                   | 返回值(成功/错误)
-------------------------------|---------------------------|--------------------------------------------------------|-------------------------
读取版本号	                    FA 06 00 FF                                                                          FA 06 81 ADDR xx xx xx	CS
读取参数	                    FA 06 01 FF                                                                          FA 06 84 “DAT1 DAT2……DAT16”
读取机器号	                    FA 06 04 FC                                                                          FA 04 81 81 / FA 84 81 02 FF
设置地址	                    FA 04 01 ADDR CS                                                                     FA 04 8B 77 / FA 84 8B 01 F6
距离修改	                    FA 04 06 符号 0xXX CS       符号为-/+，-为0x2d，+为0x2b)，(0下XX为修正值，一个字节   
连续测量时设置数据返回时间间隔	FA 04 05 MeaInterver CS
设置距离起止点	                FA 04 08 Position CS        注释：起始点可前、后端选择
设定量程	                    FA 04 09 Range CS           Range : 05,10,30,50,80m
设定频率                        FA 04 0A Freq CS            Freq : 05 10 20
设定分辨率	                    FA 04 0C Resolution CS      Resolution : 1(1mm),2(0.1mm)
设定上电即测	                FA 04 0D Start CS           Start : 0(关闭),1(开启)
单次测量                        FA 06 06 FA                 广播命令，返回结果存入模块缓存
读取缓存	                    ADDR 06 07 CS
单次测量（1mm）	                ADDR 06 02 CS
单次测量 (0.1mm)	            ADDR 06 02 CS
连续测量（1mm）	                ADDR 06 03 CS	
连续测量（0.1mm）	            ADDR 06 03 CS
控制激光打开或关闭	            ADDR 06 05 LASER CS         LASER : 00 关闭，01 开启
关机	                        ADDR 04 02 CS

出厂设置可直接使用的指令（ADDR出厂为0x80）：
单次测量            80 06 02 78
连续测量            80 06 03 77
关机                80 04 02 7A
设置地址            FA 04 01 80 81
距离修改-1          FA 04 06 2D 01 CE
距离修改+1          FA 04 06 2B 01 D0
时间间隔(1S)        FA 04 05 01 FC
设置起始点 顶端     FA 04 08 01 F9
设置起始点 尾端     FA 04 08 00 FA
设置量程5m          FA 04 09 05 F4
设置量程10m         FA 04 09 0A EF 10m
设置量程30m         FA 04 09 1E DB 30m
设置量程50m         FA 04 09 32 C7 50m
设置量程80m         FA 04 09 50 A9 80m
设置频率1Hz         FA 04 0A 00 F8
设置频率5Hz         FA 04 0A 05 F3
设置频率10Hz        FA 04 0A 0A EE
设置频率20Hz        FA 04 0A 14 E4
设定分辨率1mm       FA 04 0C 01 F5
设定分辨率0.1mm     FA 04 0C 02 F4
设定上电就测关闭    FA 04 0D 00 F5
设定上电就测开启    FA 04 0D 01 F4
单次测量（广播）    FA 06 06 FA
读取缓存            80 06 07 73
控制激光开启        80 06 05 01 74
控制激光关闭        80 06 05 00 75
*/
#include <STC32G.H>
#include <STDIO.H>
#include "uart.h"
#include "oled.h"
#include "laser_ranging.h"

float LaserRanging_Value;           // 存储测距值
unsigned char LaserRanging_Char[16];// 存储测距值字符
unsigned char LaserRanging_Range = 10;                       // 距离值
unsigned char LaserRanging_Range_Index = 1;                  // 距离值索引
unsigned char LaserRanging_Ranges[] = {5, 10, 30, 50, 80};   // 距离值数组

unsigned char LaserRanging_Cmd_Single[] = {0x80, 0x06, 0x02, 0x78};
unsigned char LaserRanging_Cmd_Continous[] = {0x80, 0x06, 0x03, 0x77};
unsigned char LaserRanging_Cmd_Shutdown[] = {0x80, 0x04, 0x02, 0x7A};
unsigned char LaserRanging_Cmd_SetFreq_1Hz[] = {0xFA, 0x04, 0x0A, 0x00, 0xF8};
unsigned char LaserRanging_Cmd_SetFreq_5Hz[] = {0xFA, 0x04, 0x0A, 0x05, 0xF3};
unsigned char LaserRanging_Cmd_SetFreq_10Hz[] = {0xFA, 0x04, 0x0A, 0x0A, 0xEE};
unsigned char LaserRanging_Cmd_SetFreq_20Hz[] = {0xFA, 0x04, 0x0A, 0x14, 0xE4};
unsigned char LaserRanging_Cmd_SetRange_5m[] = {0xFA, 0x04, 0x09, 0x05, 0xF4};
unsigned char LaserRanging_Cmd_SetRange_10m[] = {0xFA, 0x04, 0x09, 0x0A, 0xEF};
unsigned char LaserRanging_Cmd_SetRange_30m[] = {0xFA, 0x04, 0x09, 0x1E, 0xDB};
unsigned char LaserRanging_Cmd_SetRange_50m[] = {0xFA, 0x04, 0x09, 0x32, 0xC7};
unsigned char LaserRanging_Cmd_SetRange_80m[] = {0xFA, 0x04, 0x09, 0x50, 0xA9};
unsigned char LaserRanging_Cmd_SetResolution_1mm[] = {0xFA, 0x04, 0x0C, 0x01, 0xF5};
unsigned char LaserRanging_Cmd_SetResolution_0_1mm[] = {0xFA, 0x04, 0x0C, 0x02, 0xF4};
unsigned char LaserRanging_Cmd_Single_Broadcast[] = {0xFA, 0x06, 0x06, 0xFA};
unsigned char LaserRanging_Cmd_ReadCache[] = {0x80, 0x06, 0x07, 0x73};
unsigned char LaserRanging_Cmd_LaserOn[] = {0x80, 0x06, 0x05, 0x01, 0x74};
unsigned char LaserRanging_Cmd_LaserOff[] = {0x80, 0x06, 0x05, 0x00, 0x75};

void LaserRanging_IRQHandler(void)
{
    unsigned int i;
    if (Uart1_RX_Buffer[0] == 0x80)
    {
        if(Uart1_RX_Counter > 2)
        {
            if(Uart1_RX_Buffer[1] == 0x06 && Uart1_RX_Buffer[2] == 0x83)
            {
                if(Uart1_RX_Counter >= 11)
                {
                    Uart1_RX_Counter=0;
                    if( (Uart1_RX_Buffer[3]<0x34) && (Uart1_RX_Buffer[3]!='E') && (Uart1_RX_Buffer[4]!='R') && (Uart1_RX_Buffer[5]!='R') && (Uart1_RX_Buffer[10] == (unsigned char)(~(0x80+0x06+0x83+Uart1_RX_Buffer[3]+Uart1_RX_Buffer[4]+Uart1_RX_Buffer[5]+0x2E+Uart1_RX_Buffer[7]+Uart1_RX_Buffer[8]+Uart1_RX_Buffer[9])+1)) ) 
                    {
                        // LaserRanging_Value = (Uart1_RX_Buffer[4]-0x30)*10000+(Uart1_RX_Buffer[5]-0x30)*1000 + (Uart1_RX_Buffer[7]-0x30)*100 + (Uart1_RX_Buffer[8]-0x30)*10 + (Uart1_RX_Buffer[9]-0x30);                 
                        // sprintf(LaserRanging_Char, "%f", LaserRanging_Value);
                        // 将Uart1_RX_Buffer[3]~Uart1_RX_Buffer[10]存储到LaserRanging_Char数组中
                        for(i=3;i<11;i++)
                        {
                            LaserRanging_Char[i-3] = Uart1_RX_Buffer[i];
                        }
                        OLED_BuffShowString(0,2,"                ", 0);
                        OLED_BuffShowString(8*4, 2, LaserRanging_Char, 0);
                        OLED_BuffShowString(8*11, 2, "m", 0);
                        OLED_BuffShowString(0, 4, "   持续测距中   ", 0);
                    }
                    else
                    {
                        // 错误信息
                        OLED_BuffShowString(0, 4, "    测距错误    ", 1);
                    }
                }
            }
        }
    }
}

void LaserRanging_Continous()
{
    Uart1_SendCmd(LaserRanging_Cmd_Continous, LASER_RANGING_CMD_CONTINOUS_LEN);
}

void LaserRanging_LaserOff()
{
    Uart1_SendCmd(LaserRanging_Cmd_LaserOff, LASER_RANGING_CMD_LASER_OFF_LEN);
}

void LaserRanging_SetFreq_1Hz()
{
    Uart1_SendCmd(LaserRanging_Cmd_SetFreq_1Hz, LASER_RANGING_CMD_SET_FREQ_1HZ_LEN);
}

void LaserRanging_SetFreq_5Hz()
{
    Uart1_SendCmd(LaserRanging_Cmd_SetFreq_5Hz, LASER_RANGING_CMD_SET_FREQ_5HZ_LEN);
}

void LaserRanging_SetFreq_10Hz()
{
    Uart1_SendCmd(LaserRanging_Cmd_SetFreq_10Hz, LASER_RANGING_CMD_SET_FREQ_10HZ_LEN);
}

void LaserRanging_SetFreq_20Hz()
{
    Uart1_SendCmd(LaserRanging_Cmd_SetFreq_20Hz, LASER_RANGING_CMD_SET_FREQ_20HZ_LEN);
}

void LaserRanging_Range_Set()
{
    if(LaserRanging_Range == 5)
    {
        Uart1_SendCmd(LaserRanging_Cmd_SetRange_5m, LASER_RANGING_CMD_SET_RANGE_5M_LEN);
    }
    else if(LaserRanging_Range == 10)
    {
        Uart1_SendCmd(LaserRanging_Cmd_SetRange_10m, LASER_RANGING_CMD_SET_RANGE_10M_LEN);
    }
    else if(LaserRanging_Range == 30)
    {
        Uart1_SendCmd(LaserRanging_Cmd_SetRange_30m, LASER_RANGING_CMD_SET_RANGE_30M_LEN);
    }
    else if(LaserRanging_Range == 50)
    {
        Uart1_SendCmd(LaserRanging_Cmd_SetRange_50m, LASER_RANGING_CMD_SET_RANGE_50M_LEN);
    }
    else if(LaserRanging_Range == 80)
    {
        Uart1_SendCmd(LaserRanging_Cmd_SetRange_80m, LASER_RANGING_CMD_SET_RANGE_80M_LEN);
    }
}

void LaserRanging_Range_Increase()
{
    LaserRanging_Range_Index++;
    if(LaserRanging_Range_Index > 4)
    {
        LaserRanging_Range_Index = 4;
    }
    LaserRanging_Range = LaserRanging_Ranges[LaserRanging_Range_Index];
    LaserRanging_Range_Set();
}

void LaserRanging_Range_Decrease()
{
    LaserRanging_Range_Index--;
    if(LaserRanging_Range_Index < 0)
    {
        LaserRanging_Range_Index = 0;
    }
    LaserRanging_Range = LaserRanging_Ranges[LaserRanging_Range_Index];
    LaserRanging_Range_Set();
}

void LaserRanging_Init()
{
    LaserRanging_Range = LaserRanging_Ranges[LaserRanging_Range_Index];
    LaserRanging_Range_Set();
    LaserRanging_SetFreq_5Hz();
}