#ifndef __LSM303_H
#define __LSM303_H

#include "headfile.h"

// LSM303DLHC 设备地址定义
#define LSM303_ACC_ADDRESS 0x32         // 加速度计地址，写操作+0，读操作+1
#define LSM303_MAG_ADDRESS 0x3C         // 磁场计地址，  写操作+0，读操作+1

// LSM303DLHC 加速度计的寄存器地址
#define LSM303_CTRL_REG1_A      0x20    // Page24.Table20: Data rate configuration;
#define LSM303_CTRL_REG2_A      0x21
#define LSM303_CTRL_REG3_A      0x22
#define LSM303_CTRL_REG4_A      0x23
#define LSM303_CTRL_REG5_A      0x24
#define LSM303_CTRL_REG6_A      0x25
#define LSM303_REFERENCE_A      0x26
#define LSM303_STATUS_REG_A     0x27
#define LSM303_OUT_X_L_A        0x28
#define LSM303_OUT_X_H_A        0x29
#define LSM303_OUT_Y_L_A        0x2A
#define LSM303_OUT_Y_H_A        0x2B
#define LSM303_OUT_Z_L_A        0x2C
#define LSM303_OUT_Z_H_A        0x2D
#define LSM303_FIFO_CTRL_REG_A  0x2E
#define LSM303_FIFO_SRC_REG_A   0x2F
#define LSM303_INT1_CFG_A       0x30
#define LSM303_INT1_SRC_A       0x31
#define LSM303_INT1_THS_A       0x32
#define LSM303_INT1_DURATION_A  0x33
#define LSM303_INT2_CFG_A       0x34
#define LSM303_INT2_SRC_A       0x35
#define LSM303_INT2_THS_A       0x36
#define LSM303_INT2_DURATION_A  0x37
#define LSM303_CLICK_CFG_A      0x38
#define LSM303_CLICK_SRC_A      0x39
#define LSM303_CLICK_THS_A      0x3A
#define LSM303_TIME_LIMIT_A     0x3B
#define LSM303_TIME_LATENCY_A   0x3C
#define LSM303_TIME_WINDOW_A    0x3D

// LSM303DLHC 磁场计的寄存器地址
#define LSM303_CRA_REG_M        0x00
#define LSM303_CRB_REG_M        0x01
#define LSM303_MR_REG_M         0x02
#define LSM303_OUT_X_H_M        0x03
#define LSM303_OUT_X_L_M        0x04
#define LSM303_OUT_Y_H_M        0x05
#define LSM303_OUT_Y_L_M        0x06
#define LSM303_OUT_Z_H_M        0x07
#define LSM303_OUT_Z_L_M        0x08
#define LSM303_SR_REG_MG        0x09
#define LSM303_IRA_REG_M        0x0A
#define LSM303_IRB_REG_M        0x0B
#define LSM303_IRC_REG_M        0x0C

// LSM303DLHC 温度传感器寄存器
#define LSM303_TEMP_OUT_H_M     0x31
#define LSM303_TEMP_OUT_L_M     0x32

void    lsm303_init_all(void);
void    lsm303_acc_init(void);
void    lsm303_mag_init(void);
float   lsm303_read_acc(char axis);
float   lsm303_read_mag(char axis);
double  lsm303_read_mag_Angle();

#endif
