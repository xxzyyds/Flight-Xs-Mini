/**
  ******************************************************************************
  * Copyright (c) 2018,�����пƺƵ�Ƽ����޹�˾
  * All rights reserved.
  * �ļ����ƣ�LED.c
  * ժ    Ҫ��
  *
  * ��ǰ�汾��V1.0
  * ��    �ߣ������пƺƵ�Ƽ����޹�˾�з��� 
  * ������ڣ�    
  * �޸�˵����
  * 
  *
  * ��ʷ�汾��
  *
  *
  *******************************************************************************/

/*==============================================================================
                         ##### How to use this driver #####
==============================================================================
LED������ʹ�÷�ʽ���£�
g_LedManagerΪLED�ƿ��ƽṹ�壬Ҫ����LED����˸��ֻ��Ҫ���Ĵ˽ṹ���е�
ö��ֵ���ɡ���g_LedManager.emLed_Status
���磺
1.��Ҫ״̬��������Ҫ�������
g_LedManager.emLed_Status = StatusOn;

����ö����Դ����.h�ļ��е�emLED_Status_t

*/
//�ⲿ�ļ�����
#include "LED.h"
#include "control.h"
#include "Hardware.h"
#include "battery.h"
#include "timer_drv.h"

//�궨����
#define LED_FLASH_FREQ      100


//Extern����
extern FMUflg_t g_FMUflg;    


//˽�к�����
void LEDEventHandle(void);


//˽�б�����
LedManager_t g_LedManager;


/******************************************************************************
  * �������ƣ�LEDInit
  * ������������ʼ��LED��
  * ��    �룺void
  * ��    ����void
  * ��    �أ�void
  * ��    ע��null
  *    
  *
******************************************************************************/
/*
void LEDInit(void)        
{
    //LED��״̬����
    LED_STATUS_OFF;
    LED_POWER_OFF;
    g_LedManager.emLEDStatus = StatusOff;
    g_LedManager.emLEDPower = PowerOff;    
    
    P1DIR |= GPIO_PIN0;
    
    //����GPIO����
    P2DIR |= GPIO_PIN0;
    P2DIR |= GPIO_PIN1;
    P2DIR |= GPIO_PIN2;
	
	
    
//    3.5
//    5.0
//    5.2
    P3DIR |= GPIO_PIN5;
    P5DIR |= GPIO_PIN0;
    P5DIR |= GPIO_PIN2;
    
    P3OUT |= GPIO_PIN5;
    P5OUT |= GPIO_PIN0;
    P5OUT |= GPIO_PIN2;
    
		
//    6.2����Ϊ��openmvͨ������		 �½��ظ�openmv��־ת����
		P6DIR |= GPIO_PIN2;
		P6OUT |= GPIO_PIN2;    //�ߵ�ƽ 
//		P6OUT &= ~GPIO_PIN2;		 //�͵�ƽ
    
		
		//����GPIO���
    P1OUT &= ~GPIO_PIN0;
    P2OUT &= ~GPIO_PIN0;
    P2OUT |= GPIO_PIN2;
    
    LED_RGB_R_ON;
    Delay_ms(100);
    LED_RGB_R_OFF;
    Delay_ms(100);
    LED_RGB_B_ON;
    Delay_ms(100);
    LED_RGB_B_OFF;
    Delay_ms(100);
    
    LED_RGB_G_ON;
    Delay_ms(100);
    LED_RGB_G_OFF;
    Delay_ms(100);
}
*/
/******************************************************************************
  * �������ƣ�PollingLED
  * ������������ѯ��ǰ�Ƿ���LED�����������
  * ��    �룺void
  * ��    ����void
  * ��    �أ�void
  * ��    ע��null
  *
  *
******************************************************************************/
void PollingLED()
{
	/*
    g_LedManager.u16FlashTime++;

    //����LED�ƹ�����״̬����LED��
    switch(g_LedManager.emLEDStatus)
    {
        case StatusFlash:
            if(g_LedManager.u16FlashTime % LED_FLASH_FREQ == 0)
            {
                LED_STATUS_TOGGLE;
            }
            break;
        case StatusToggle:
            LED_STATUS_TOGGLE;
            break;
        case StatusOn:
            LED_STATUS_ON;
            break;
        case StatusOff:
            LED_STATUS_OFF;
            break;
        default:
            break;
    }
    
    //����LED�ƹ�����״̬����LED��
    switch(g_LedManager.emLEDPower)
    {
        case PowerOn:
            LED_POWER_ON;
            break;
        case PowerFlash:
            if(g_LedManager.u16FlashTime % LED_FLASH_FREQ == 0)
            {
                LED_POWER_TOGGLE;
            }
            break;
        case PowerOff:
            LED_POWER_OFF;
            break;
        case PowerToggle:
            LED_POWER_TOGGLE;
            break;
        default:
            break;
    }
    
    //����LED�ƹ�����״̬����LED��
    switch(g_LedManager.emLED_RGB_B)
    {
        case RGB_LED_On:
            LED_RGB_B_ON;
            break;
        case RGB_LED_Flash:
            if(g_LedManager.u16FlashTime % LED_FLASH_FREQ == 0)
            {
                LED_RGB_B_TOGGLE;
            }
            break;
        case RGB_LED_Off:
            LED_RGB_B_OFF;
            break;
        case RGB_LED_Toggle:
            LED_RGB_B_TOGGLE;
            break;
        default:
            break;
    }
    
    switch(g_LedManager.emLED_RGB_R)
    {
        case RGB_LED_On:
            LED_RGB_R_ON;
            break;
        case RGB_LED_Flash:
            if(g_LedManager.u16FlashTime % LED_FLASH_FREQ == 0)
            {
                LED_RGB_R_TOGGLE;
            }
            break;
        case RGB_LED_Off:
            LED_RGB_R_OFF;
            break;
        case RGB_LED_Toggle:
            LED_RGB_R_TOGGLE;
            break;
        default:
            break;
    }
    
    switch(g_LedManager.emLED_RGB_G)
    {
        case RGB_LED_On:
            LED_RGB_G_ON;
            break;
        case RGB_LED_Flash:
            if(g_LedManager.u16FlashTime % LED_FLASH_FREQ == 0)
            {
                LED_RGB_G_TOGGLE;
            }
            break;
        case RGB_LED_Off:
            LED_RGB_G_OFF;
            break;
        case RGB_LED_Toggle:
            LED_RGB_G_TOGGLE;
            break;
        default:
            break;
    }
    
    //����LED�ƹ�����״̬����LED��    
    LEDEventHandle();
		*/
}

#include "FollowLine.h"
extern FollowManager_t FollowManager;

//LED�ƴ�����
void LEDEventHandle()
{
    //�жϷɻ��Ƿ��������������LED���ж�
    if(!g_FMUflg.unlock)
    {
        g_LedManager.emLEDPower = PowerOff;
        g_LedManager.emLEDStatus = StatusOn;
    }else
    {
        g_LedManager.emLEDStatus = StatusOff;
        g_LedManager.emLEDPower = PowerOn; 
    }
    
    //������̿ص���ʱʱ����ʼ��˸
    if(FollowManager.ActionList == ActionCountdown)
    {
        static int cnt = 0;
        cnt++;
        
        if(cnt % 50 == 0)
        {
            static bool tmp = false;
            tmp = !tmp;
            if(tmp)
            {
                //LED������
                P2OUT |= GPIO_PIN1;
            }else
            {
                //LED������
                P2OUT &= ~GPIO_PIN1;
            }
        }
    }else
    {
        P2OUT &= ~GPIO_PIN1;
    }
}

/******************* (C) ��Ȩ���� 2018 �����пƺƵ�Ƽ����޹�˾ *******************/
