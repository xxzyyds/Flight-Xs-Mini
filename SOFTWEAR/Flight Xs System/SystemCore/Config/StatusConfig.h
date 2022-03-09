#ifndef __Status_CONFIG_H
#define __Status_CONFIG_H

#include "include.h"
//宏定义区
#define WAITING 0
#define WAITING_1 1
#define READY 2
#define PROCESS 3
#define EXIT 4


// 飞行状态标志位，包含解锁标志位等
// to do : 使用 bit-field 减少内存占用
typedef struct
{
		// 机身按钮解锁
    uint8_t unlock;
    // 程控启动
		uint8_t Auto;
	
		uint8_t height_lock;
		
		// 遥控器 期望起飞
    uint8_t take_off;
    uint8_t take_down;
} f_Status;

// 四元数
typedef struct {
  float q0;
  float q1;
  float q2;
  float q3;
} Quaternion;


// 角度状态定义
typedef struct{
    float roll;
    float pitch;
    float yaw;
} f_AL;

//当前角度姿态值
extern f_AL FlightAttitude;
extern f_Status FlightStatus;


#endif
