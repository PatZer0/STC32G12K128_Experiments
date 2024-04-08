#include "lsm303.h"
#include <MATH.H>

void lsm303_init_all(void) 
{
    lsm303_acc_init();
    lsm303_mag_init();
}

void lsm303_acc_init(void) 
{
    // 初始化加速度传感器
    // 写入LSM303_CTRL_REG[1:6]_A寄存器

    unsigned char CTRL_REG1;
    unsigned char CTRL_REG2;
    unsigned char CTRL_REG3;
    unsigned char CTRL_REG4;
    unsigned char CTRL_REG5;
    unsigned char CTRL_REG6;

    CTRL_REG1 = 0x27;
    // 配置为 0101 0111 = 0x57
    // 前四位： 0000 关机模式
    //          0011 通用1Hz    0010 通用10Hz   0011 通用25Hz   0100 通用50Hz   0101 通用100Hz
    //          0110 通用200Hz  0111 通用400Hz  1000 低功耗1.620kHz             1001 普通1.344kHz/低功耗5.376kHz
    // 后四位： *___ 0:低功耗模式关闭   /   1:低功耗模式开启
    //          _*__ 0:Z轴关闭          /   1:Z轴开启
    //          __*_ 0:Y轴关闭          /   1:Y轴开启
    //          ___* 0:X轴关闭          /   1:X轴开启

    CTRL_REG2 = 0x00; // 高通滤波器配置关闭
    CTRL_REG3 = 0x00; // 所有中断关闭
    CTRL_REG4 = 0x00; // 数据输出格式设置为正常模式，±2g
    CTRL_REG5 = 0x00; // FIFO关闭，不使用高通滤波器
    CTRL_REG6 = 0x00; // 中断活动配置关闭

    // 写入寄存器
    iic_write_reg(LSM303_ACC_ADDRESS, LSM303_CTRL_REG1_A, CTRL_REG1);
    // iic_write_reg(LSM303_ACC_ADDRESS, LSM303_CTRL_REG2_A, CTRL_REG2);
    // iic_write_reg(LSM303_ACC_ADDRESS, LSM303_CTRL_REG3_A, CTRL_REG3);
    // iic_write_reg(LSM303_ACC_ADDRESS, LSM303_CTRL_REG4_A, CTRL_REG4);
    // iic_write_reg(LSM303_ACC_ADDRESS, LSM303_CTRL_REG5_A, CTRL_REG5);
    // iic_write_reg(LSM303_ACC_ADDRESS, LSM303_CTRL_REG6_A, CTRL_REG6);
}

void lsm303_mag_init(void) {
    // 初始化磁场传感器
    unsigned char CRA_REG_M;
    unsigned char CRB_REG_M;
    unsigned char MR_REG_M;
    // 配置CRA_REG_M，数据输出速率为220Hz，温度传感器开
    CRA_REG_M = 0x9C; 
    iic_write_reg(LSM303_MAG_ADDRESS, LSM303_CRA_REG_M, CRA_REG_M);

    // 配置CRB_REG_M，增益配置为±1.3Gauss
    CRB_REG_M = 0x20; // 0010 0000
    // iic_write_reg(LSM303_MAG_ADDRESS, LSM303_CRB_REG_M, CRB_REG_M);

    // 配置MR_REG_M，设置为连续转换模式
    MR_REG_M = 0x00; // 0000 0000
    iic_write_reg(LSM303_MAG_ADDRESS, LSM303_MR_REG_M, MR_REG_M);
    // iic_write_reg(LSM303_MAG_ADDRESS, LSM303_MR_REG_M, MR_REG_M);
}

float lsm303_read_acc(char axis) 
{
    unsigned char acc_data[2];
    float acc_value;
    unsigned char addr_low, addr_high;

    // 根据轴选择正确的寄存器地址
    switch(axis) 
    {
        case 'x':
            addr_low = LSM303_OUT_X_L_A;
            addr_high = LSM303_OUT_X_H_A;
            break;
        case 'y':
            addr_low = LSM303_OUT_Y_L_A;
            addr_high = LSM303_OUT_Y_H_A;
            break;
        case 'z':
            addr_low = LSM303_OUT_Z_L_A;
            addr_high = LSM303_OUT_Z_H_A;
            break;
        default:
            return 0; // 如果轴不正确，则返回0
    }

    // 读取加速度计的数据
    iic_read_reg_bytes(LSM303_ACC_ADDRESS, addr_low, acc_data, 2);
    // LSM303_MasterRecieveOneBytefromSlave(LSM303_ACC_ADDRESS, addr_low, acc_data[0]);
    // LSM303_MasterRecieveOneBytefromSlave(LSM303_ACC_ADDRESS, addr_high, acc_data[1]);

    // 计算加速度值，考虑到数据为16位，左移高位并加上低位
    acc_value = ((short)(acc_data[1] << 8 | acc_data[0])) / 16384.0f;
    // acc_value = ((float)(acc_data[1] << 8 | acc_data[0])) / 16.384f;

    return acc_value;
}

float lsm303_read_mag(char axis) 
{
    unsigned char mag_data_lo, mag_data_hi;
    float mag_value;
    unsigned char addr_low, addr_high;

    // 根据轴选择正确的寄存器地址
    switch(axis) 
    {
        case 'x':
            addr_low = LSM303_OUT_X_L_M;
            addr_high = LSM303_OUT_X_H_M;
            break;
        case 'y':
            addr_low = LSM303_OUT_Y_L_M;
            addr_high = LSM303_OUT_Y_H_M;
            break;
        case 'z':
            addr_low = LSM303_OUT_Z_L_M;
            addr_high = LSM303_OUT_Z_H_M;
            break;
        default:
            return 0; // 如果轴不正确，则返回0
    }

    // 读取磁场计的数据
    // LSM303_MasterReceiveMultipleBytesfromSlave(LSM303_MAG_ADDRESS, addr, mag_data, 2);
    iic_read_reg(LSM303_MAG_ADDRESS, addr_high, &mag_data_hi);
    iic_read_reg(LSM303_MAG_ADDRESS, addr_low, &mag_data_lo);

    // 计算磁场值，由于磁场传感器的数据排列与加速度计不同，需要先读高位
    mag_value = ((short)(mag_data_hi << 8 | mag_data_lo)) / 1100.0f;

    return mag_value;
}

double lsm303_read_mag_angle()
{
    #define M_PI 3.14159265358979323846
    unsigned char mag_data[6];
    double angle;
    int mag_x, mag_y, mag_z;
    // unsigned char addr_starter;

    iic_read_reg_bytes(LSM303_MAG_ADDRESS, LSM303_OUT_X_L_M, mag_data, 6);

    mag_x = mag_data[0] << 8 | mag_data[1];
    mag_y = mag_data[2] << 8 | mag_data[3];
    mag_z = mag_data[4] << 8 | mag_data[5];

    // 计算磁场角度
    angle = atan2((double)mag_y, (double)mag_x) * (180 / M_PI) + 180;
    angle *= 10;

    return angle;
}