#ifndef __IIC_H
#define __IIC_H

//------------------------------------------???????------------------------------------------
#define  SCL P25
#define  SDA P24


//------------------------------------------????????------------------------------------------
void IIC_DELAY();				                            //???????
void IIC_START(void);			                            //IIC???
void IIC_STOP(void);			                            //IIC????
void IIC_SENDACK(void);			                            //????ACK
void IIC_SENDNACK(void);		                            //????NACK
void IIC_WAITACK(void);			                            //??????ACK
void IIC_SENDBYTE(unsigned char dat);		                            //??????????
unsigned char IIC_READBYTE(void);			                            //?????????
void IIC_Init();				                            //IIC?????

void IIC_Write_NByte( unsigned char slave,unsigned char addr,unsigned char *p,unsigned char number );	//iic§Õ??????????????
void IIC_Read_NByte( unsigned char slave,unsigned char addr,unsigned char *p,unsigned char number );	//iic???????????????

#endif



