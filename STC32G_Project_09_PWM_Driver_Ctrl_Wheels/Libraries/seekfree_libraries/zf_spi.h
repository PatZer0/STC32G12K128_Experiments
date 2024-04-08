/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		spi
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#ifndef __ZF_SPI_H
#define __ZF_SPI_H
#include "common.h"
#include "zf_gpio.h"



//STC16只有一个SPI，可以通过切换通道来切换引脚
typedef enum
{
    SPI_CH1=0,
    SPI_CH2,  
    SPI_CH3,	
    SPI_CH4,	
	
	//其中SS引脚由软件控制
} SPIN_enum;


//该枚举体禁止用户修改
typedef enum
{
	//SPI只能使用同一组引脚，不允许混用引脚
	SPI_CH1_SCLK_P15 = 0x00<<8 | P1_5, 
	SPI_CH1_MOSI_P13 = 0x01<<8 | P1_3, 
	SPI_CH1_MISO_P14 = 0x02<<8 | P1_4, 
					
	//SPI只能使用同一组引脚，不允许混用引脚	
	SPI_CH2_SCLK_P25 = 0x10<<8 | P2_5, 
	SPI_CH2_MOSI_P23 = 0x11<<8 | P2_3, 
	SPI_CH2_MISO_P24 = 0x12<<8 | P2_4, 
		
	//SPI只能使用同一组引脚，不允许混用引脚	
	SPI_CH3_SCLK_P43 = 0x20<<8 | P4_3, 
	SPI_CH3_MOSI_P40 = 0x21<<8 | P4_0, 
	SPI_CH3_MISO_P41 = 0x22<<8 | P4_1, 
						
	//SPI只能使用同一组引脚，不允许混用引脚	
	SPI_CH4_SCLK_P32 = 0x30<<8 | P3_2, 
	SPI_CH4_MOSI_P34 = 0x31<<8 | P3_4, 
	SPI_CH4_MISO_P33 = 0x32<<8 | P3_3, 
	
	SPI_NULL_PIN = 0xFFFF,
	//其中CS引脚由软件控制
} SPI_PIN_enum;


//该枚举体禁止用户修改
typedef enum
{
    MASTER = 0,	 //主机
    SLAVE, //从机
} SPI_MSTR_enum;

//该枚举体禁止用户修改
typedef enum
{
	SPI_SYSclk_DIV_4 = 0,
	SPI_SYSclk_DIV_8,
	SPI_SYSclk_DIV_16,
	SPI_SYSclk_DIV_2,
}SPI_BAUD_enum;


void spi_init(SPIN_enum spi_n,
			  SPI_PIN_enum sck_pin, 
			  SPI_PIN_enum mosi_pin, 
			  SPI_PIN_enum miso_pin, 
			  uint8 mode,
			  SPI_MSTR_enum mstr,
			  SPI_BAUD_enum baud);

void spi_change_pin(SPIN_enum spi_n, 
					SPI_PIN_enum sck_pin, 
					SPI_PIN_enum mosi_pin, 
					SPI_PIN_enum miso_pin);

void spi_change_mode(uint8 mode);

uint8 spi_mosi(uint8 dat);




#endif


