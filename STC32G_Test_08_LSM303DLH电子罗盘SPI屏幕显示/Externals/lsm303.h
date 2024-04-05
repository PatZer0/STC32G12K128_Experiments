#ifndef __LSM303_H
#define __LSM303_H

#include "iic.h"

#define	LSM303DLH_ACC_ADDRESS 0x30 //定义器件在IIC总线中的从地址,(加速度)根据ALT  ADDRESS地址引脚不同修改 0x30（ADDRESS=0）                                 
#define	LSM303DLH_MAG_ADDRESS 0x3C //定义器件在IIC总线中的从地址,(磁场)根据ALT    ADDRESS地址引脚不同修改

#define MAG_SLAVE_ADDR_READ 0x3D
#define MAG_SLAVE_ADDR_WRITE 0x3C
#define ACC_SLAVE_ADDR_READ 0x33
#define ACC_SLAVE_ADDR_WRITE 0x32

#define CTRL_REG1_A 0x20
#define CTRL_REG4_A 0x23
#define MR_REG_M 0x02
#define OUT_X_L_A 0x28 // 加速度计X轴低位
#define OUT_X_H_A 0x29 // 加速度计X轴高位
#define OUT_Y_L_A 0x2A // 加速度计Y轴低位
#define OUT_Y_H_A 0x2B // 加速度计Y轴高位
#define OUT_Z_L_A 0x2C // 加速度计Z轴低位
#define OUT_Z_H_A 0x2D // 加速度计Z轴高位
#define OUT_X_H_M 0x03 // 磁场计X轴高位
#define OUT_X_L_M 0x04 // 磁场计X轴低位
#define OUT_Y_H_M 0x07 // 磁场计Y轴高位
#define OUT_Y_L_M 0x08 // 磁场计Y轴低位
#define OUT_Z_H_M 0x05 // 磁场计Z轴高位
#define OUT_Z_L_M 0x06 // 磁场计Z轴低位

void LSM303_Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char LSM303_Single_Read(unsigned char SlaveAddress,unsigned char REG_Address);
void LSM303_Continuous_Read(unsigned char SlaveAddress,unsigned char ST_Address);
void LSM303_Get_Acc_X();
void LSM303_Get_Acc_Y();
void LSM303_Get_Acc_Z();
void LSM303_Acc_Init();
void LSM303_Mag_Init();
void LSM303_Init();

#endif // __LSM303_H
