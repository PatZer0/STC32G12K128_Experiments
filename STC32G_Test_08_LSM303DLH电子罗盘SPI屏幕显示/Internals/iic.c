#include "iic.h"
#include <STC32G.H>

//========================================================================
// 函数名称: IIC_Init
// 函数功能: IIC初始化
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 
//========================================================================
void IIC_Init()				//IIC初始化
{
    I2C_S1 =2;              //I2C功能脚选择，00:P1.5,P1.4; 01:P2.5,P2.4; 11:P3.2,P3.3
    I2C_S0 =1;
    I2CCFG = 0XE0;			//设置速度和使能IIC
    I2CMSST = 0;			//清空主机状态
}

//========================================================================
// 函数名称: IIC_DELAY
// 函数功能: IIC等待结束
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 
//========================================================================
void IIC_DELAY()	
{
    while( !(I2CMSST & 0X40) );
    I2CMSST &= (~0X40);
}

//========================================================================
// 函数名称: IIC_START
// 函数功能: IIC开始
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 
//========================================================================
void IIC_START(void)	//IIC开始
{
    I2CMSCR = 0X01;			//开始命令
    IIC_DELAY();			//等待结束
}

//========================================================================
// 函数名称: IIC_STOP
// 函数功能: IIC结束
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 
//========================================================================
void IIC_STOP(void)		//IIC结束
{
    I2CMSCR = 0X06;			//结束命令
    IIC_DELAY();
}

//========================================================================
// 函数名称: IIC_SENDACK
// 函数功能: IIC发送ACK
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 
//========================================================================
void IIC_SENDACK(void)	//发送ACK
{
    I2CMSST = 0X00;			//发送aCK
    I2CMSCR = 0X05;			//aCK命令
    IIC_DELAY();
}

//========================================================================
// 函数名称: IIC_SENDNACK
// 函数功能: IIC发送NACK
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 
//========================================================================
void IIC_SENDNACK(void)	//发送NACK
{
    I2CMSST = 0X01;			//发送naCK
    I2CMSCR = 0X05;			//aCK命令
    IIC_DELAY();
}

//========================================================================
// 函数名称: IIC_WAITACK
// 函数功能: IIC等到从机ACK
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 
//========================================================================
void IIC_WAITACK(void)	//等到从机ACK
{
    I2CMSCR = 0X03;			//接收aCK
    IIC_DELAY();
}

//========================================================================
// 函数名称: IIC_SENDBYTE
// 函数功能: IIC发送一个字节
// 入口参数: @dat：发送的数据
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 
//========================================================================
void IIC_SENDBYTE(unsigned char dat)	//发送一个字节
{
    I2CTXD = dat;			//发送的数据写入寄存器
    I2CMSCR = 0X02;			//发送
    IIC_DELAY();
}

//========================================================================
// 函数名称: IIC_READBYTE
// 函数功能: IIC读取一个字节
// 入口参数: 无
// 函数返回: 读到的数据
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 
//========================================================================
unsigned char IIC_READBYTE(void)	//读取一个字节
{
    I2CMSCR = 0X04;			//发送
    IIC_DELAY();		
    return I2CRXD;
}



//========================================================================
// 函数名称: IIC_Write_NByte
// 函数功能: IIC连续写入几个字节
// 入口参数: @slave：从机地址  @addr：数据地址  @p:数组   @number：数据个数
// 函数返回: 读到的数据
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 
//========================================================================
void IIC_Write_NByte( unsigned char slave,unsigned char addr,unsigned char *p,unsigned char number )	//iic写入连续的几个字节
{
    IIC_START();
    IIC_SENDBYTE(slave);
    IIC_WAITACK();

    IIC_SENDBYTE(addr);
    IIC_WAITACK();

    do
    {
        IIC_SENDBYTE(*p);
        p++;
        IIC_WAITACK();

    }
    while(--number);
    IIC_STOP();
}

//========================================================================
// 函数名称: IIC_Read_NByte
// 函数功能: IIC连续读取几个字节
// 入口参数: @slave：从机地址  @addr：数据地址  @p:数组   @number：数据个数
// 函数返回: 读到的数据
// 当前版本: VER1.0
// 修改日期: 2023 - 6 - 7
// 当前作者: ZCF
// 其他备注: 
//========================================================================
void IIC_Read_NByte( unsigned char slave,unsigned char addr,unsigned char *p,unsigned char number )	//iic读取连续的几个字节
{
    IIC_START();
    IIC_SENDBYTE(slave);
    IIC_WAITACK();

    IIC_SENDBYTE(addr);
    IIC_WAITACK();

    IIC_START();
    IIC_SENDBYTE((unsigned char)(slave+0x01));	
    IIC_WAITACK();

    do
    {
        *p = IIC_READBYTE();	//P[0] => P[1]
        p++;
        if( number!=1 )
            IIC_SENDACK();
    }
    while(--number);
    IIC_SENDNACK();
    IIC_STOP();	
}

	
