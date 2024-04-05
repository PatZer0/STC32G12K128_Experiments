#include <STC32G.H>
#include <MATH.H>
#include <STDIO.H>
#include "stdint.h"
#include "lsm303.h"
#include "oled.h"

unsigned char LSM303_Buffer[8];         //接收数据缓存区
int Output_Data;

//单字节写入LSM303内部数据*******************************
void LSM303_Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)
{
    IIC_START();                  //起始信号
    IIC_SENDBYTE(SlaveAddress);   //发送设备地址+写信号
    IIC_SENDBYTE(REG_Address);    //内部寄存器地址
    IIC_SENDBYTE(REG_data);       //内部寄存器数据
    IIC_STOP();                   //发送停止信号
}

//单字节读取LSM303内部数据********************************
unsigned char LSM303_Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   
    unsigned char REG_data;
    IIC_START();                          //起始信号
    IIC_SENDBYTE(SlaveAddress);           //发送设备地址+写信号
    IIC_SENDBYTE(REG_Address);            //发送存储单元地址
    IIC_START();                          //起始信号
    IIC_SENDBYTE(SlaveAddress+1);         //发送设备地址+读信号
    REG_data = IIC_READBYTE();              //读出寄存器数据
    IIC_SENDACK();
    IIC_STOP();                           //停止信号
    return REG_data;
}

//连续读出LSM303内部数据
void LSM303_Continuous_Read(unsigned char SlaveAddress,unsigned char ST_Address)
{   
    unsigned char i;
    IIC_START();                            //起始信号
    IIC_SENDBYTE(SlaveAddress);             //发送设备地址+写信号
    IIC_SENDBYTE(ST_Address);               //发送存储单元地址
    IIC_START();                            //起始信号
    IIC_SENDBYTE(SlaveAddress+1);           //发送设备地址+读信号
    for (i=0; i<6; i++)                     //连续读取6个地址数据，存储中BUF
    {
        LSM303_Buffer[i] = IIC_READBYTE();            //LSM303_Buffer[0]存储
        if (i == 5)
        {
            IIC_SENDNACK();                 //最后一个数据需要回NOACK
        }
        else
        {
            IIC_SENDACK();                  //回应ACK
        }
    }
    IIC_STOP();                             //停止信号
    IIC_DELAY();                            //延时
}

//加速度显示x轴
void LSM303_Get_Acc_X()
{
    float Acc_X;
    unsigned char Acc_X_Str[16];
    LSM303_Buffer[0]=LSM303_Single_Read(LSM303DLH_ACC_ADDRESS,0x28);    //OUT_X_L_A
    LSM303_Buffer[1]=LSM303_Single_Read(LSM303DLH_ACC_ADDRESS,0x29);    //OUT_X_H_A
    Output_Data=(LSM303_Buffer[1]<<8)+LSM303_Buffer[0];                 //合成数据

    // OLED显示X轴
    OLED_BuffShowString(0,0,"X:",0);              //第0行，第0列 显示X
    if(Output_Data<0) 
    {
        Output_Data=-Output_Data;
        OLED_BuffShowString(2*8,0,"-",0);         //显示正负符号位
    }
    else
    {
        OLED_BuffShowString(2*8,0," ",0);         //显示空格
    }
    Acc_X = (float)Output_Data;///16.383;         //计算数据和显示
    sprintf(Acc_X_Str, "%f", Acc_X);
    OLED_BuffShowString(3*8,0,Acc_X_Str,0);       //显示数据
    OLED_BuffShow();
}

//加速度显示y轴
void LSM303_Get_Acc_Y()
{
    float Acc_Y;
    unsigned char Acc_Y_Str[16];
    LSM303_Buffer[0]=LSM303_Single_Read(LSM303DLH_ACC_ADDRESS,0x2a);    //OUT_Y_L_A
    LSM303_Buffer[1]=LSM303_Single_Read(LSM303DLH_ACC_ADDRESS,0x2b);    //OUT_Y_H_A
    Output_Data=(LSM303_Buffer[3]<<8)+LSM303_Buffer[2];                 //合成数据

    // OLED显示Y轴
    OLED_BuffShowString(0,2,"Y:",0);              //第1行，第0列 显示Y
    if(Output_Data<0) 
    {
        Output_Data=-Output_Data;
        OLED_BuffShowString(2*8,2,"-",0);         //显示正负符号位
    }
    else
    {
        OLED_BuffShowString(2*8,2," ",0);         //显示空格
    }
    Acc_Y = (float)Output_Data;///16.383;         //计算数据和显示
    sprintf(Acc_Y_Str, "%f", Acc_Y);
    OLED_BuffShowString(3*8,2,Acc_Y_Str,0);       //显示数据
    OLED_BuffShow();
}

//加速度显示z轴
void LSM303_Get_Acc_Z()
{
    float Acc_Z;
    unsigned char Acc_Z_Str[16];
    LSM303_Buffer[0]=LSM303_Single_Read(LSM303DLH_ACC_ADDRESS,0x2c);    //OUT_Z_L_A
    LSM303_Buffer[1]=LSM303_Single_Read(LSM303DLH_ACC_ADDRESS,0x2d);    //OUT_Z_H_A
    Output_Data=(LSM303_Buffer[5]<<8)+LSM303_Buffer[4];                 //合成数据

    // OLED显示Z轴
    OLED_BuffShowString(0,4,"Z:",0);              //第2行，第0列 显示Z
    if(Output_Data<0) 
    {
        Output_Data=-Output_Data;
        OLED_BuffShowString(2*8,4,"-",0);         //显示正负符号位
    }
    else
    {
        OLED_BuffShowString(2*8,4," ",0);         //显示空格
    }
    Acc_Z = (float)Output_Data;///16.383;         //计算数据和显示
    sprintf(Acc_Z_Str, "%f", Acc_Z);
    OLED_BuffShowString(3*8,4,Acc_Z_Str,0);       //显示数据
    OLED_BuffShow();
}



// //加速度显示y轴
// void display_y()
// {   float temp;
//     LSM303_Buffer[2]=LSM303_Single_Read(LSM303DLH_ACC_ADDRESS,0x2a);//OUT_Y_L_A
//     LSM303_Buffer[3]=LSM303_Single_Read(LSM303DLH_ACC_ADDRESS,0x2b);//OUT_Y_H_A


//     Output_Data=(LSM303_Buffer[3]<<8)+LSM303_Buffer[2];  //合成数据
//     if(Output_Data<0) {
//         Output_Data=-Output_Data;
//         DisplayOneChar(2,1,'-');      //显示正负符号位
//     }
//     else DisplayOneChar(2,1,' '); //显示空格

//     temp=(float)Output_Data/16.383;  //计算数据和显示
//     LSM303_ConvertData(temp);          //转换出显示需要的数据
//     DisplayOneChar(0,1,'Y');   //第1行，第0列 显示y
//     DisplayOneChar(1,1,':');
//     DisplayOneChar(3,1,qian);
//     DisplayOneChar(4,1,'.');
//     DisplayOneChar(5,1,bai);
//     //  DisplayOneChar(6,1,shi);
//     DisplayOneChar(6,1,'g');
// }

// //加速度显示z轴
// void display_z()
// {   float temp;
//     LSM303_Buffer[4]=LSM303_Single_Read(LSM303DLH_ACC_ADDRESS,0x2c);//OUT_Z_L_A
//     LSM303_Buffer[5]=LSM303_Single_Read(LSM303DLH_ACC_ADDRESS,0x2d);//OUT_Z_H_A

//     Output_Data=(LSM303_Buffer[5]<<8)+LSM303_Buffer[4];  //合成数据
//     if(Output_Data<0) {
//         Output_Data=-Output_Data;
//         DisplayOneChar(11,1,'-');     //显示负符号位
//     }
//     else DisplayOneChar(11,1,' ');//显示空格

//     temp=(float)Output_Data/16.383;  //计算数据和显示
//     LSM303_ConvertData(temp);             //转换出显示需要的数据
//     DisplayOneChar(9,1,'Z');      //第0行，第10列 显示Z
//     DisplayOneChar(10,1,':');
//     DisplayOneChar(12,1,qian);
//     DisplayOneChar(13,1,'.');
//     DisplayOneChar(14,1,bai);
//     // DisplayOneChar(14,1,shi);
//     DisplayOneChar(15,1,'g');
// }

// void display_Angle(void)
// {   int x,y,z;
//     double  angle;

//     x=LSM303_Buffer[0] << 8 | LSM303_Buffer[1]; //Combine MSB and LSB of X Data output register
//     y=LSM303_Buffer[2] << 8 | LSM303_Buffer[3]; //Combine MSB and LSB of Z Data output register
//     z=LSM303_Buffer[4] << 8 | LSM303_Buffer[5]; //Combine MSB and LSB of Y Data output register

//     angle= atan2((double)y,(double)x) * (180 / 3.14159265) + 180; // angle in degrees
//     angle*=10;

//     LSM303_ConvertData(angle);       //计算数据和显示
//     DisplayOneChar(9,0,'A');
//     DisplayOneChar(10,0,':');
//     DisplayOneChar(11,0,qian);
//     DisplayOneChar(12,0,bai);
//     DisplayOneChar(13,0,shi);
//     DisplayOneChar(14,0,'.');
//     DisplayOneChar(15,0,ge);
// }

//初始化LSM303A(加速度)，根据需要请参考pdf进行修改
void LSM303_Acc_Init()
{
    LSM303_Single_Write(LSM303DLH_ACC_ADDRESS,0x20,0x27);  //测量范围, 正负2g, 16位模式
}

//初始化LSM303M(磁场)，根据需要请参考pdf进行修改
void LSM303_Mag_Init()
{
    LSM303_Single_Write(LSM303DLH_MAG_ADDRESS,0x02,0x00);  //
}

void LSM303_Init()
{
    LSM303_Acc_Init();
    LSM303_Mag_Init();
}