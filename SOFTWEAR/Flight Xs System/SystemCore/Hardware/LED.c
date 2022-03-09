/*==============================================================================
                         ##### How to use this driver #####
==============================================================================*/

//�ⲿ�ļ�����
#include "LED.h"
#include "control.h"
#include "Hardware.h"
#include "battery.h"
#include "timer_drv.h"
#include "fmuConfig.h"
#include "StatusConfig.h"

//�궨����
#define LED_FLASH_FREQ      100 


//˽�к�����
void LEDEventHandle(void);


//˽�б�����
LedManager_t g_LedManager;


#include "FollowLine.h"
extern FollowManager_t FollowManager;

//LED�ƴ�����
void LEDEventHandle()
{
    //�жϷɻ��Ƿ��������������LED���ж�
    if(!FlightStatus.unlock)
    {
        g_LedManager.emLEDPower = PowerOff;
        g_LedManager.emLEDStatus = StatusOn;
    }else
    {
        g_LedManager.emLEDStatus = StatusOff;
        g_LedManager.emLEDPower = PowerOn; 
    }
    
    //todo :������̿ص���ʱʱ����ʼ��˸

}
