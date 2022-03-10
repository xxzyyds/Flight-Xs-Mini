/*==============================================================================
                         ##### How to use this driver #####
==============================================================================*/

//�ⲿ�ļ�����
#include "StatusConfig.h"
#include "LED.h"
#include "FollowLine.h"


//�궨����
#define LED_FLASH_FREQ      100 


//˽�к�����
void LEDEventHandle(void);


//˽�б�����
LedManager_t g_LedManager;




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
