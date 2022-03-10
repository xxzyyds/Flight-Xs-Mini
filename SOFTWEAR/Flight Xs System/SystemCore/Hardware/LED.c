/*==============================================================================
                         ##### How to use this driver #####
==============================================================================*/

//外部文件引用
#include "StatusConfig.h"
#include "LED.h"
#include "FollowLine.h"


//宏定义区
#define LED_FLASH_FREQ      100 


//私有函数区
void LEDEventHandle(void);


//私有变量区
LedManager_t g_LedManager;




//LED灯处理函数
void LEDEventHandle()
{
    //判断飞机是否解锁，作出解锁LED灯判断
    if(!FlightStatus.unlock)
    {
        g_LedManager.emLEDPower = PowerOff;
        g_LedManager.emLEDStatus = StatusOn;
    }else
    {
        g_LedManager.emLEDStatus = StatusOff;
        g_LedManager.emLEDPower = PowerOn; 
    }
    
    //todo :当进入程控倒计时时，开始闪烁

}
