/*==============================================================================
                         ##### How to use this driver #####
==============================================================================
飞行函数自动调用，在轮训系统中调用
1.FlightPidControl函数3ms时间调用一次
2.MotorControl函数3ms调用一次

*/


//外部文件引用
#include "include.h" 
#include "StatusConfig.h"
#include "pidConfig.h"
#include "mymath.h"

//定义
float PIDGroup_desired_yaw_pos_tmp,fb_gyro_lpf[3];

//
extern PID PIDGroup[Num_Of_PIDList];

int16_t motor[4];

#define MOTOR1      motor[0] 
#define MOTOR2      motor[1] 
#define MOTOR3      motor[2] 
#define MOTOR4      motor[3]


#define ClearMotor  memset(motor, 0, sizeof(int16_t) * 4)



/******************************************************************************
  * 函数名称：FlightPidControl
  * 函数描述：飞行PID控制函数
  * 输    入：float dt：单位运行时间
  * 输    出：void
  * 返    回：void 
  * 备    注：null  
******************************************************************************/

void FlightPidControl(float dt)
{
    volatile static uint8_t status = WAITING;

    //状态更新switch
    switch(status)
    {
        //等待状态
        case WAITING:
            if(FlightStatus.unlock)
            {
                status = READY;    
            }
            break
							;
        //准备状态
        case READY:
						// 批量复位PID数据，防止上次遗留的数据影响本次控制
            ResetPID();                             
            IMU_Reset();
            FlightAttitude.yaw = 0;
            PIDGroup_desired_yaw_pos_tmp = FlightAttitude.yaw;
            PIDGroup[PID_Yaw_Pos].measured = 0;
            status = PROCESS;
            break;
				
        //正式进入控制
        case PROCESS:                
						fb_gyro_lpf[0] += 0.5f *(MPU6050.gyroX * Gyro_G - fb_gyro_lpf[0]); //内环测量值 角度/秒
						fb_gyro_lpf[1] += 0.5f *(MPU6050.gyroY * Gyro_G - fb_gyro_lpf[1]); //内环测量值 角度/秒
						fb_gyro_lpf[2] += 0.5f *(MPU6050.gyroZ * Gyro_G - fb_gyro_lpf[2]); //内环测量值 角度/秒

            //速度环PID测量值由陀螺仪给出
            PIDGroup[PID_Roll_Spd].measured = fb_gyro_lpf[0];
            PIDGroup[PID_Pitch_Spd].measured = fb_gyro_lpf[1];
            PIDGroup[PID_Yaw_Spd].measured = fb_gyro_lpf[2];
        
            //位置环PID测量值由解算出来的姿态给出
            PIDGroup[PID_Pitch_Pos].measured = FlightAttitude.pitch; //外环测量值 单位：角度
            PIDGroup[PID_Roll_Pos].measured = FlightAttitude.roll;
            
            //YAW做个特殊处理
            PIDGroup[PID_Yaw_Pos].measured = 0;
            PIDGroup[PID_Yaw_Pos].desired = (PIDGroup_desired_yaw_pos_tmp - FlightAttitude.yaw);
            if(PIDGroup[PID_Yaw_Pos].desired>=180)
            {
                PIDGroup[PID_Yaw_Pos].desired -= 360;
            }
            else if(PIDGroup[PID_Yaw_Pos].desired<=-180)
            {
                PIDGroup[PID_Yaw_Pos].desired += 360;
            }

            ClacCascadePID(&PIDGroup[PID_Roll_Spd],  &PIDGroup[PID_Roll_Pos],  dt);      //X轴
            ClacCascadePID(&PIDGroup[PID_Pitch_Spd], &PIDGroup[PID_Pitch_Pos], dt);     //Y轴
            ClacCascadePID(&PIDGroup[PID_Yaw_Spd],   &PIDGroup[PID_Yaw_Pos],   dt);       //Z轴
            break;
						
				// 退出控制
        case EXIT:                  
            ResetPID();
            IMU_Reset();
						// 返回等待解锁
            status = WAITING;         
          break;
				
        default:
            status = EXIT;
            break;
    }
    // 紧急制动
    if(FlightStatus.unlock == 0)     
    {
        status = EXIT;
    }
}



/******************************************************************************
  * 函数名称：MotorControl
  * 函数描述：更新电机控制逻辑
  * 输    入：void
  * 输    出：void
  * 返    回：void 
  * 备    注：null    
  *    
  *
******************************************************************************/
void MotorControl(void)
{    
    volatile static uint8_t status = WAITING;

    //电机解锁判定
    if(FlightStatus.unlock == 0)
    {
        status = EXIT;
    }
    
    //电机状态控制
    switch(status)
    {
        //等待状态1
        case WAITING: 
            if(FlightStatus.unlock)
            {
                FlightStatus.take_off = 0;    
                FlightStatus.height_lock = 0; 
                status = WAITING_2;
            }
        //等待状态2
        case WAITING_1:
            {
								// todo: 如果  不处于一键起飞并且 目标Z速度 小于0， 怠速 
								if(FlightStatus.Auto || FlightStatus.take_off) //刚解锁时，如果，认为操作者还不想飞行
                {
                    status = PROCESS_31;
                }
								
                //怠速电机转速
                MOTOR1 = 100;
                MOTOR2 = 100;
                MOTOR3 = 100;
                MOTOR4 = 100;
                break;
            }
        //主处理
        case PROCESS:
            {
                int16_t temp = 0;
                
                // 紧急制动，当姿态角小于某一些值时，制动
                if(FlightAttitude.pitch < -MAX_ISFD_ATTITUDE 
                || FlightAttitude.pitch > MAX_ISFD_ATTITUDE
                || FlightAttitude.roll  < -MAX_ISFD_ATTITUDE
                || FlightAttitude.roll  > MAX_ISFD_ATTITUDE)
                {
                    FlightStatus.unlock = 0;
                    status = EXIT_255;
                    ResetAlt();
                    ResetPID();
                    IMU_Reset();
                    ClearMotor;
                    break;
                }
								// todo: 将 高度环的输出 给 PWM 当作基础值
                
								// temp = z_out
                
								//将油门值作为基础值给PWM
                MOTOR1 = LIMIT(temp, 0, MOTOR_MAX_INIT_VALUE); 
                MOTOR2 = LIMIT(temp, 0, MOTOR_MAX_INIT_VALUE); 
                MOTOR3 = LIMIT(temp, 0, MOTOR_MAX_INIT_VALUE); 
                MOTOR4 = LIMIT(temp, 0, MOTOR_MAX_INIT_VALUE); 
                
                //电机控制
                MOTOR1 += +PIDGroup[PID_Roll_Spd].out - PIDGroup[PID_Pitch_Spd].out + PIDGroup[PID_Yaw_Spd].out;
                MOTOR2 += +PIDGroup[PID_Roll_Spd].out + PIDGroup[PID_Pitch_Spd].out - PIDGroup[PID_Yaw_Spd].out;
                MOTOR3 += -PIDGroup[PID_Roll_Spd].out + PIDGroup[PID_Pitch_Spd].out + PIDGroup[PID_Yaw_Spd].out;
                MOTOR4 += -PIDGroup[PID_Roll_Spd].out - PIDGroup[PID_Pitch_Spd].out - PIDGroup[PID_Yaw_Spd].out;
                
                //电机限速
                MOTOR1 = LIMIT(MOTOR1, 100, MOTOR_MAX_VALUE); 
                MOTOR2 = LIMIT(MOTOR2, 100, MOTOR_MAX_VALUE); 
                MOTOR3 = LIMIT(MOTOR3, 100, MOTOR_MAX_VALUE); 
                MOTOR4 = LIMIT(MOTOR4, 100, MOTOR_MAX_VALUE); 
            }
            break;
        case EXIT:
            status = WAITING;    //返回等待解锁
            ClearMotor;
            break;
        default:
            break;
    }
    //更新电机输出
    Update_Motor(MOTOR1, MOTOR2, MOTOR3, MOTOR4);
}
