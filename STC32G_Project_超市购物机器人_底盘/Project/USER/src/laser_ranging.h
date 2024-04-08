#ifndef __LASER_RANGING_H
#define __LASER_RANGING_H

extern float LaserRanging_Value;
extern unsigned char LaserRanging_Char[16];
extern unsigned char LaserRanging_Range;

extern unsigned char LaserRanging_Cmd_Single[];
extern unsigned char LaserRanging_Cmd_Continous[];
extern unsigned char LaserRanging_Cmd_Shutdown[];
extern unsigned char LaserRanging_Cmd_SetFreq_1Hz[];
extern unsigned char LaserRanging_Cmd_SetFreq_5Hz[];
extern unsigned char LaserRanging_Cmd_SetFreq_10Hz[];
extern unsigned char LaserRanging_Cmd_SetFreq_20Hz[];
extern unsigned char LaserRanging_Cmd_SetResolution_1mm[];
extern unsigned char LaserRanging_Cmd_SetResolution_0_1mm[];
extern unsigned char LaserRanging_Cmd_Single_Broadcast[];
extern unsigned char LaserRanging_Cmd_ReadCache[];
extern unsigned char LaserRanging_Cmd_LaserOn[];
extern unsigned char LaserRanging_Cmd_LaserOff[];

#define LASER_RANGING_CMD_SINGLE_LEN  4
#define LASER_RANGING_CMD_CONTINOUS_LEN  4
#define LASER_RANGING_CMD_SHUTDOWN_LEN  4
#define LASER_RANGING_CMD_SET_FREQ_1HZ_LEN  5
#define LASER_RANGING_CMD_SET_FREQ_5HZ_LEN  5
#define LASER_RANGING_CMD_SET_FREQ_10HZ_LEN  5
#define LASER_RANGING_CMD_SET_FREQ_20HZ_LEN  5
#define LASER_RANGING_CMD_SET_RANGE_5M_LEN 5
#define LASER_RANGING_CMD_SET_RANGE_10M_LEN 5
#define LASER_RANGING_CMD_SET_RANGE_30M_LEN 5
#define LASER_RANGING_CMD_SET_RANGE_50M_LEN 5
#define LASER_RANGING_CMD_SET_RANGE_80M_LEN 5
#define LASER_RANGING_CMD_SET_RESOLUTION_1MM_LEN  5
#define LASER_RANGING_CMD_SET_RESOLUTION_0_1MM_LEN  5
#define LASER_RANGING_CMD_SINGLE_BROADCAST_LEN  4
#define LASER_RANGING_CMD_READ_CACHE_LEN  4
#define LASER_RANGING_CMD_LASER_ON_LEN  5
#define LASER_RANGING_CMD_LASER_OFF_LEN  5

void LaserRanging_Receiving();
void LaserRanging_IRQHandler();
void LaserRanging_Continous();
void LaserRanging_LaserOff();
void LaserRanging_Range_Increase();
void LaserRanging_Range_Decrease();
void LaserRanging_Init();
void LaserRanging_Range_Set();

#endif