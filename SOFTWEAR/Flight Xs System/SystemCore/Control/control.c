/*==============================================================================
                         ##### How to use this driver #####
==============================================================================
���к����Զ����ã�����ѵϵͳ�е���
1.FlightPidControl����3msʱ�����һ��
2.MotorControl����3ms����һ��

*/


//�ⲿ�ļ�����
#include "include.h" 
#include "StatusConfig.h"
#include "pidConfig.h"
#include "mymath.h"

//����
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
  * �������ƣ�FlightPidControl
  * ��������������PID���ƺ���
  * ��    �룺float dt����λ����ʱ��
  * ��    ����void
  * ��    �أ�void 
  * ��    ע��null  
******************************************************************************/

void FlightPidControl(float dt)
{
    volatile static uint8_t status = WAITING;

    //״̬����switch
    switch(status)
    {
        //�ȴ�״̬
        case WAITING:
            if(FlightStatus.unlock)
            {
                status = READY;    
            }
            break
							;
        //׼��״̬
        case READY:
						// ������λPID���ݣ���ֹ�ϴ�����������Ӱ�챾�ο���
            ResetPID();                             
            IMU_Reset();
            FlightAttitude.yaw = 0;
            PIDGroup_desired_yaw_pos_tmp = FlightAttitude.yaw;
            PIDGroup[PID_Yaw_Pos].measured = 0;
            status = PROCESS;
            break;
				
        //��ʽ�������
        case PROCESS:                
						fb_gyro_lpf[0] += 0.5f *(MPU6050.gyroX * Gyro_G - fb_gyro_lpf[0]); //�ڻ�����ֵ �Ƕ�/��
						fb_gyro_lpf[1] += 0.5f *(MPU6050.gyroY * Gyro_G - fb_gyro_lpf[1]); //�ڻ�����ֵ �Ƕ�/��
						fb_gyro_lpf[2] += 0.5f *(MPU6050.gyroZ * Gyro_G - fb_gyro_lpf[2]); //�ڻ�����ֵ �Ƕ�/��

            //�ٶȻ�PID����ֵ�������Ǹ���
            PIDGroup[PID_Roll_Spd].measured = fb_gyro_lpf[0];
            PIDGroup[PID_Pitch_Spd].measured = fb_gyro_lpf[1];
            PIDGroup[PID_Yaw_Spd].measured = fb_gyro_lpf[2];
        
            //λ�û�PID����ֵ�ɽ����������̬����
            PIDGroup[PID_Pitch_Pos].measured = FlightAttitude.pitch; //�⻷����ֵ ��λ���Ƕ�
            PIDGroup[PID_Roll_Pos].measured = FlightAttitude.roll;
            
            //YAW�������⴦��
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

            ClacCascadePID(&PIDGroup[PID_Roll_Spd],  &PIDGroup[PID_Roll_Pos],  dt);      //X��
            ClacCascadePID(&PIDGroup[PID_Pitch_Spd], &PIDGroup[PID_Pitch_Pos], dt);     //Y��
            ClacCascadePID(&PIDGroup[PID_Yaw_Spd],   &PIDGroup[PID_Yaw_Pos],   dt);       //Z��
            break;
						
				// �˳�����
        case EXIT:                  
            ResetPID();
            IMU_Reset();
						// ���صȴ�����
            status = WAITING;         
          break;
				
        default:
            status = EXIT;
            break;
    }
    // �����ƶ�
    if(FlightStatus.unlock == 0)     
    {
        status = EXIT;
    }
}



/******************************************************************************
  * �������ƣ�MotorControl
  * �������������µ�������߼�
  * ��    �룺void
  * ��    ����void
  * ��    �أ�void 
  * ��    ע��null    
  *    
  *
******************************************************************************/
void MotorControl(void)
{    
    volatile static uint8_t status = WAITING;

    //��������ж�
    if(FlightStatus.unlock == 0)
    {
        status = EXIT;
    }
    
    //���״̬����
    switch(status)
    {
        //�ȴ�״̬1
        case WAITING: 
            if(FlightStatus.unlock)
            {
                FlightStatus.take_off = 0;    
                FlightStatus.height_lock = 0; 
                status = WAITING_2;
            }
        //�ȴ�״̬2
        case WAITING_1:
            {
								// todo: ���  ������һ����ɲ��� Ŀ��Z�ٶ� С��0�� ���� 
								if(FlightStatus.Auto || FlightStatus.take_off) //�ս���ʱ���������Ϊ�����߻��������
                {
                    status = PROCESS_31;
                }
								
                //���ٵ��ת��
                MOTOR1 = 100;
                MOTOR2 = 100;
                MOTOR3 = 100;
                MOTOR4 = 100;
                break;
            }
        //������
        case PROCESS:
            {
                int16_t temp = 0;
                
                // �����ƶ�������̬��С��ĳһЩֵʱ���ƶ�
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
								// todo: �� �߶Ȼ������ �� PWM ��������ֵ
                
								// temp = z_out
                
								//������ֵ��Ϊ����ֵ��PWM
                MOTOR1 = LIMIT(temp, 0, MOTOR_MAX_INIT_VALUE); 
                MOTOR2 = LIMIT(temp, 0, MOTOR_MAX_INIT_VALUE); 
                MOTOR3 = LIMIT(temp, 0, MOTOR_MAX_INIT_VALUE); 
                MOTOR4 = LIMIT(temp, 0, MOTOR_MAX_INIT_VALUE); 
                
                //�������
                MOTOR1 += +PIDGroup[PID_Roll_Spd].out - PIDGroup[PID_Pitch_Spd].out + PIDGroup[PID_Yaw_Spd].out;
                MOTOR2 += +PIDGroup[PID_Roll_Spd].out + PIDGroup[PID_Pitch_Spd].out - PIDGroup[PID_Yaw_Spd].out;
                MOTOR3 += -PIDGroup[PID_Roll_Spd].out + PIDGroup[PID_Pitch_Spd].out + PIDGroup[PID_Yaw_Spd].out;
                MOTOR4 += -PIDGroup[PID_Roll_Spd].out - PIDGroup[PID_Pitch_Spd].out - PIDGroup[PID_Yaw_Spd].out;
                
                //�������
                MOTOR1 = LIMIT(MOTOR1, 100, MOTOR_MAX_VALUE); 
                MOTOR2 = LIMIT(MOTOR2, 100, MOTOR_MAX_VALUE); 
                MOTOR3 = LIMIT(MOTOR3, 100, MOTOR_MAX_VALUE); 
                MOTOR4 = LIMIT(MOTOR4, 100, MOTOR_MAX_VALUE); 
            }
            break;
        case EXIT:
            status = WAITING;    //���صȴ�����
            ClearMotor;
            break;
        default:
            break;
    }
    //���µ�����
    Update_Motor(MOTOR1, MOTOR2, MOTOR3, MOTOR4);
}
