#include "lsm303.h"

void LSM303_MasterWriteOneBytetoSlave(unsigned char slave_addr, unsigned char register_addr, unsigned char dat_to_send) 
{
    // MASTER   Start   SlaveAddr+W             RegisterAddr            Data            Stop
    // SLAVE                            ACK                     ACK             ACK
    IIC_START();
    IIC_SENDBYTE(slave_addr+0);
    IIC_WAITACK();
    IIC_SENDBYTE(register_addr);
    IIC_WAITACK();
    IIC_SENDBYTE(dat_to_send);
    IIC_WAITACK();
    IIC_STOP();
}

void LSM303_MasterWriteMultipleBytestoSlave(unsigned char slave_addr, unsigned char register_addr, unsigned char *dat_to_send, unsigned char bytes_to_send)
{
    // MASTER   Start   SlaveAddr+W             RegisterAddr            Data0           Data1          ...  DataN          Stop
    // SLAVE                            ACK                     ACK             ACK             ACK             ...  ACK
    unsigned char i;

    IIC_START();                            // 发送起始信号
    IIC_SENDBYTE(slave_addr);               // 发送从设备地址加写操作位
    IIC_WAITACK();                          // 等待从设备的应答
    IIC_SENDBYTE(register_addr);            // 发送寄存器地址
    IIC_WAITACK();                          // 等待从设备的应答

    for(i = 0; i < bytes_to_send; i++)      // 对于所有要发送的数据
    {
        IIC_SENDBYTE(dat_to_send[i]);       // 发送数据
        IIC_WAITACK();                      // 等待从设备的应答
    }

    IIC_STOP();                             // 发送停止信号
}

void LSM303_MasterRecieveOneBytefromSlave(unsigned char slave_addr, unsigned char register_addr, unsigned char dat_storage) 
{
    // MASTER   Start   SlaveAddr+W             RegisterAddr            StartRepeat     SlaveAddr+R                     NACK    Stop
    // SLAVE                            ACK                     ACK                                     ACK     Data
    IIC_START();
    IIC_SENDBYTE(slave_addr+0);
    IIC_WAITACK();
    IIC_SENDBYTE(register_addr);
    IIC_WAITACK();
    IIC_START();
    IIC_SENDBYTE(slave_addr+1);
    IIC_WAITACK();
    dat_storage = IIC_READBYTE();
    IIC_SENDNACK();
    IIC_STOP();
}

void LSM303_MasterReceiveMultipleBytesfromSlave(unsigned char slave_addr, unsigned char register_addr, unsigned char *data_buffer, unsigned char bytes_to_receive)
{
    // MASTER   Start   SlaveAddr+W             RegisterAddr            StartRepeat     SlaveAddr+R                     ACK             ACK     ...     NACK    Stop
    // SLAVE                            ACK                     ACK                                     ACK     Data1           DataN           ...
    unsigned char i;

    IIC_START();                            // 发送起始信号
    IIC_SENDBYTE(slave_addr);               // 发送从设备地址加写操作位
    IIC_WAITACK();                          // 等待从设备的应答
    IIC_SENDBYTE(register_addr);            // 发送寄存器地址
    IIC_WAITACK();                          // 等待从设备的应答

    IIC_START();                            // 发送重复起始信号，切换到读模式
    IIC_SENDBYTE(slave_addr + 1);           // 发送从设备地址加读操作位
    IIC_WAITACK();                          // 等待从设备的应答

    for(i = 0; i < bytes_to_receive - 1; i++) // 对于所有要接收的数据（最后一个字节除外）
    {
        data_buffer[i] = IIC_READBYTE();    // 读取数据
        IIC_SENDACK();                      // 发送应答信号，准备接收下一个字节
    }
    
    // 读取最后一个字节并发送非应答信号
    data_buffer[bytes_to_receive - 1] = IIC_READBYTE(); // 读取最后一个字节的数据
    IIC_SENDNACK();                         // 对于最后一个字节，发送非应答信号
    IIC_STOP();                             // 发送停止信号
}

void LSM303_Init(void) 
{
    LSM303_Acc_Init();
    LSM303_Mag_Init();
}

void LSM303_Acc_Init(void) 
{
    // 初始化加速度传感器
    // 写入LSM303_CTRL_REG[1:6]_A寄存器

    unsigned char CTRL_REG1;
    unsigned char CTRL_REG2;
    unsigned char CTRL_REG3;
    unsigned char CTRL_REG4;
    unsigned char CTRL_REG5;
    unsigned char CTRL_REG6;

    CTRL_REG1 = 0x57
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
    LSM303_MasterWriteOneBytetoSlave(LSM303_ACC_ADDRESS, LSM303_CTRL_REG1_A, CTRL_REG1);
    LSM303_MasterWriteOneBytetoSlave(LSM303_ACC_ADDRESS, LSM303_CTRL_REG2_A, CTRL_REG2);
    LSM303_MasterWriteOneBytetoSlave(LSM303_ACC_ADDRESS, LSM303_CTRL_REG3_A, CTRL_REG3);
    LSM303_MasterWriteOneBytetoSlave(LSM303_ACC_ADDRESS, LSM303_CTRL_REG4_A, CTRL_REG4);
    LSM303_MasterWriteOneBytetoSlave(LSM303_ACC_ADDRESS, LSM303_CTRL_REG5_A, CTRL_REG5);
    LSM303_MasterWriteOneBytetoSlave(LSM303_ACC_ADDRESS, LSM303_CTRL_REG6_A, CTRL_REG6);
}

void LSM303_Mag_Init(void) {
    // 初始化磁场传感器
    unsigned char data;

    // 配置CRA_REG_M，数据输出速率为15Hz
    data = 0x10; // 0001 0000
    LSM303_MasterWriteOneBytetoSlave(LSM303_MAG_ADDRESS, LSM303_CRA_REG_M, data);

    // 配置CRB_REG_M，增益配置为±1.3Gauss
    data = 0x20; // 0010 0000
    LSM303_MasterWriteOneBytetoSlave(LSM303_MAG_ADDRESS, LSM303_CRB_REG_M, data);

    // 配置MR_REG_M，设置为连续转换模式
    data = 0x00; // 0000 0000
    LSM303_MasterWriteOneBytetoSlave(LSM303_MAG_ADDRESS, LSM303_MR_REG_M, data);
}

float LSM303_Read_Acc(char axis) 
{
    unsigned char acc_data[2];
    float acc_value;
    unsigned char addr;

    // 根据轴选择正确的寄存器地址
    switch(axis) {
        case 'x':
            addr = LSM303_OUT_X_L_A;
            break;
        case 'y':
            addr = LSM303_OUT_Y_L_A;
            break;
        case 'z':
            addr = LSM303_OUT_Z_L_A;
            break;
        default:
            return 0; // 如果轴不正确，则返回0
    }

    // 读取加速度计的数据
    LSM303_MasterReceiveMultipleBytesfromSlave(LSM303_ACC_ADDRESS, addr, acc_data, 2);

    // 计算加速度值，考虑到数据为16位，左移高位并加上低位
    acc_value = ((short)(acc_data[1] << 8 | acc_data[0])) / 16384.0f;

    return acc_value;
}

float LSM303_Read_Mag(char axis) 
{
    unsigned char mag_data[2];
    float mag_value;
    unsigned char addr;

    // 根据轴选择正确的寄存器地址
    switch(axis) {
        case 'x':
            addr = LSM303_OUT_X_H_M;
            break;
        case 'y':
            addr = LSM303_OUT_Y_H_M;
            break;
        case 'z':
            addr = LSM303_OUT_Z_H_M;
            break;
        default:
            return 0; // 如果轴不正确，则返回0
    }

    // 读取磁场计的数据
    LSM303_MasterReceiveMultipleBytesfromSlave(LSM303_MAG_ADDRESS, addr, mag_data, 2);

    // 计算磁场值，由于磁场传感器的数据排列与加速度计不同，需要先读高位
    mag_value = ((short)(mag_data[0] << 8 | mag_data[1])) / 1100.0f;

    return mag_value;
}
