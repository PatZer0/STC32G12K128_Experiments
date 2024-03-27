#ifndef __IIC_H
#define __IIC_H

//------------------------------------------����ѡ��------------------------------------------
#define  SCL P25
#define  SDA P24


//------------------------------------------��������------------------------------------------
void IIC_DELAY();				                            //�ȴ�����
void IIC_START(void);			                            //IIC��ʼ
void IIC_STOP(void);			                            //IIC����
void IIC_SENDACK(void);			                            //����ACK
void IIC_SENDNACK(void);		                            //����NACK
void IIC_WAITACK(void);			                            //�ȵ��ӻ�ACK
void IIC_SENDBYTE(unsigned char dat);		                            //����һ���ֽ�
unsigned char IIC_READBYTE(void);			                            //��ȡһ���ֽ�
void IIC_Init();				                            //IIC��ʼ��

void IIC_Write_NByte( unsigned char slave,unsigned char addr,unsigned char *p,unsigned char number );	//iicд�������ļ����ֽ�
void IIC_Read_NByte( unsigned char slave,unsigned char addr,unsigned char *p,unsigned char number );	//iic��ȡ�����ļ����ֽ�

#endif



