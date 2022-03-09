/*==============================================================================
                         ##### How to use this driver #####
==============================================================================
Ӳ����ʼ������ʹ��ʱֻ��Ҫ����Hadrware_Init����


*/
//�ⲿ�ļ�����
#include "include.h"
#include "led.h"
#include "spl06.h"
#include "battery.h"
#include "FollowLine.h"
#include "sdk.h"
#include "timer_drv.h"
#include "power.h"
#include "fmuConfig.h"
#include "pidConfig.h"
#include "StatusConfig.h"
#include "MPU6050.h"

//�궨����
#define HARDWARE_CHECK_LED    LED_STATUS_ON;LED_POWER_ON;Delay_ms(100);\
                              LED_STATUS_OFF;LED_POWER_OFF;Delay_ms(100);\
                              LED_STATUS_ON;LED_POWER_ON;Delay_ms(100);\
                              LED_STATUS_OFF;LED_POWER_OFF;Delay_ms(100);
#ifdef STM32
#define HARDWARE_CHECK        MPU6050.Check && \
                              g_SPL06Manager.Check && \
                              g_NRFManager.Check
#endif
//Extern����
extern PID PIDGroup[Num_Of_PIDList];
extern bool InitComplete;
//˽�к�����
void PID_Init(void); 

//˽�б�����
uint32_t SysTick_count; //ϵͳʱ�����
Queue_t USB_Send_Queue;

/******************************************************************************
  * �������ƣ�Hadrware_Init
  * ������������ʼ������Ӳ���Ͳ���
  * ��    �룺void
  * ��    ����void
  * ��    �أ�void 
  * ��    ע��null    
  *    
  *
******************************************************************************/
 void Hadrware_Init(void)
{

    //POWER����
    LEDInit();                      //LED���Ƴ�ʼ��
    MPU6050Init();                  //MPU6050��ʼ��
    SPL06Init();                    //SPL06��ʼ��
    PID_Init();                     //PID������ʼ��
    Follow_Init();                  //Ѳ�߿�������ʼ��
    Remote_init();                  //ң������ʼ��
    sdk_init();                     //SDK��ʼ��
    
    //�ж�MPU6050��SPL06��ʼ���Ƿ�ɹ�
    if(MPU6050.Check)
    {
        InitComplete = true;
    }
}

/******************************************************************************
  * �������ƣ�PID_Init
  * ������������ʼ������PID����
  * ��    �룺void
  * ��    ����void
  * ��    �أ�void 
  * ��    ע��null
  *
  *
******************************************************************************/
void PID_Init(void)
{
    PIDGroup[PID_Pitch_Pos].kp   = 8.0f;
    PIDGroup[PID_Pitch_Spd].kp   = 1.1f;
    PIDGroup[PID_Pitch_Spd].ki   = 1.1f;
    PIDGroup[PID_Pitch_Spd].kd   = 0.06f;
    PIDGroup[PID_Pitch_Spd].IntegLimitHigh = 10;
    PIDGroup[PID_Pitch_Spd].IntegLimitLow = -10;

    PIDGroup[PID_Roll_Pos].kp    = 8.0f;
    PIDGroup[PID_Roll_Spd].kp    = 1.1f;
    PIDGroup[PID_Roll_Spd].ki    = 1.0f;
    PIDGroup[PID_Roll_Spd].kd    = 0.04f;
    PIDGroup[PID_Roll_Spd].IntegLimitHigh = 10;
    PIDGroup[PID_Roll_Spd].IntegLimitLow = -10;

    PIDGroup[PID_Yaw_Pos].kp     = 8.0f;
    PIDGroup[PID_Yaw_Spd].kp     = 2.0f;
    PIDGroup[PID_Yaw_Spd].kd     = 0.00f;
    PIDGroup[PID_Yaw_Spd].OutLimitHigh = 100;
    PIDGroup[PID_Yaw_Spd].OutLimitLow = -100;

    PIDGroup[PID_Roll_Spd].IntegLimitHigh = 50; 
    PIDGroup[PID_Roll_Spd].IntegLimitLow = -50;
}


/******************* (C) ��Ȩ���� 2018 �����пƺƵ�Ƽ����޹�˾ *******************/
