#ifndef __SPI_H_
#define __SPI_H_

#include <STC32G.H>

//------------------------------------------引脚选择------------------------------------------
    //注意：这里SPI的引脚选择要通过P_SW1寄存器切换，在修改这里的引脚
sbit    SPI_MISO = P2^4;    //PIN2
sbit    SPI_MOSI = P2^3;    //PIN5
sbit    SPI_SCK = P2^5;     //PIN6
//------------------------------------------函数声明------------------------------------------
void SPI_init(void);
void SPI_WriteByte(unsigned char out);
    

#endif


