#include "include.h"


//宏定义区
#define WAITING 1
#define READY 2
#define PROCESS 3
#define EXIT 4

// 电机状态定义
#define MOTOR_MAX_VALUE 200


// 飞行状态标志位，包含解锁标志位等
typedef struct
{
		// 机身按钮解锁
    uint8_t unlock:0;
    // 程控启动
		uint8_t Auto:0;
	
		uint8_t height_lock:0;
		
		// 遥控器 期望起飞
    uint8_t take_off:0;
    uint8_t take_down:0;
}FS;

FS FlightStatus;


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
}FA;

//当前角度姿态值
FA FlightAttitude;





