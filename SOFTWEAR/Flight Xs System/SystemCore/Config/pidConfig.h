// X / Y 位置 --> R / Y / P 角度  --> 角速度 --> 姿态转速
// 高度 --> 四轴的角速度 -->【基础转速】

// pid
#ifndef __PID_CONFIG_H
#define __PID_CONFIG_H
typedef struct
{
    float kp;           //< proportional gain
    float ki;           //< integral gain
    float kd;           //< derivative gain
    float out;
    float Err;
    float desired;     //< set point
    float measured;
    
    float DeathArea;
    
    float Err_LimitHigh;
    float Err_LimitLow;
    
    float offset;      //
    float prevError;    //< previous error
    float integ;        //< integral

    float IntegLimitHigh;       //< integral limit
    float IntegLimitLow;

    float OutLimitHigh;
    float OutLimitLow;
}PID;

typedef enum
{
		// 内环
    PID_Pitch_Spd = 0,
    PID_Roll_Spd,
    PID_Yaw_Spd,
		// 外环
    PID_Pitch_Pos,
    PID_Roll_Pos,
    PID_Yaw_Pos,
	
		// 高度
    PID_Height_Spd,
    PID_Height_Pos,
		
		// 程控
    //sdk_pos_x,
    //sdk_pos_y,
	
    Num_Of_PIDList,
}PIDList;

//PID PIDGroup[Num_Of_PIDList];

// height pid


#endif
