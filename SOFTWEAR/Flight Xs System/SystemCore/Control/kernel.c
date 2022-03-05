/**
  ******************************************************************************
  * Copyright (c) 2018,�����пƺƵ�Ƽ����޹�˾
  * All rights reserved.
  * �ļ����ƣ�kernel.c
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
ѭ������������ʹ�øú�����ѵ��������


*/
//�ⲿ�ļ�����
#include "include.h"
#include "communication.h"
#include "gcs.h"
#include "speed_estimator.h"
#include "led.h"
#include "battery.h"
#include "stdio.h"
#include "HARDWARE_uart.h"
#include "SPL06.h"
#include "timer_drv.h"
#include "Ano_OF.h"
#include "pos_ctrl.h"
#include "program_ctrl.h"
#include "ANO_GCS_DT.h"
#include "FollowLine.h"
#include "ANO_DT.h"
#include "sdk.h"
#include "beep.h"

//�궨����
#define InitCheck        if(!InitComplete)\
                        {\
                            P2OUT |= GPIO_PIN1;\
                            P2OUT |= GPIO_PIN2;\
                            P2OUT |= GPIO_PIN0;\
                            P1OUT |= GPIO_PIN0;\
                             return ;\
                        }\

#define KernelRunningCheck  if(!KernelRunning)\
                                return;\

//Extern����



//˽�к�����
void Update(void);
void UpdateUSBQueue(void);


//˽�б�����
bool KernelRunning = false;
bool InitComplete = false;
int16_t Angle_Int16[3];
uint8_t Buff[20];
int sum = 0;
            
/******************************************************************************
  * �������ƣ�KernelPolling
  * ����������������ѯ����
  * ��    �룺void
  * ��    ����void
  * ��    �أ�void 
  * ��    ע��1ms����һ��    
  *    
  *
******************************************************************************/
uint8_t buffer[5] = {0xBB,0xBB,0x00,0x01,0xFF};       //���̸˱�־λ
uint32_t test_time[5];
uint32_t test_runtime[3];
uint8_t data[5] = {0xAA,0xBB,0xCC,0xDD,0xEE};
int cnttime=0,stoping=0;
extern int jiguang;
extern FollowManager_t FollowManager;
//uint32_t test_systick[255];
//uint8_t test_cnt;
void KernelPolling()
{
    static uint32_t Cnt = 0;

    //��ʼ����飬���ʧ�ܣ�������LED����
    InitCheck;

    //�������м��
    KernelRunningCheck;

    KernelRunning = false;
	
		
    //��ѯ�Ƿ���������Ҫ����
    ANO_DT_Data_Receive_Anl_Task();
    
    //ʱ�����ѯ����
    Cnt++;
//  if (Cnt % 500 == 0)  
//	{	 
//		for(int b=0;b<5;b++)
//			{
//				UART_transmitData(UsartGroup[UART_A1].moduleInstance, buffer[b]);
//			
//		
//			}
//	}

	
    //333Hz,�޸ĵ��δ��ʱ������
    if (Cnt % 3 == 0)
    {        
        //����PID����������
        FlightPidControl(0.003f);
        
        //����߼�����
        MotorControl();
			 
    }
    
    //125Hz
    if (Cnt % 8 == 0)
    { 
        //����ŷ����
        GetAngle(&g_Attitude);
    }

    //91Hz
    if (Cnt % 11 == 0)
    {
        //����״̬��������
        AnoOF_State_Task(11);
        
        float distance = FollowManager.FrontOpenmvFramePtr->cnt1 | (FollowManager.FrontOpenmvFramePtr->Target >> 8);
    }
    
    //100Hz
    if (Cnt % 10 == 0)
    {        
        //����Ѳ������
        //���棡����
        //�˺��������Զ��������ݣ������д˺����Ժ�
        //����LaunchPad�尴ť���Զ�����ʱ�����ɻ�������������������̵�ʱ��
        //��Ҫ���׽��ע��
        
        //�˺���Ϊ�̿���ں����������˽�̿ػ��ƺ󣬿�ȡ���˺���ע��
        UpdateCentControl(0.01f);
        
        sdk_update(0.01f);
    }
    
    //50Hz����
    if (Cnt % 20 == 0)
    {
        //��ѹ�Ƹ���
        UpdateSPL06Info();

        //�۲⴫�������ݼ���
        WZ_Obs_Calcu(0.02f);
        
        //Z�����ݻ�������
        WZ_Fix_Calcu(0.02f);
        
        //�߶ȿ�����
        ALT_Ctrl(0.02f);        
        
        //λ�ÿ�����
        POS_Ctrl(0.02f);
    }
    
    //20Hz����
    if (Cnt % 50 == 0)
    {
        PollingGCS();
    }

    //LED��ѵ����
    PollingLED();
		
    
    //������ѵ����
    PollingUSART();
    //USART_TX(&UsartGroup[UART_A1], buffer, 5);

		
    beep_update();
		
//		if (Cnt % 10 == 0)
//		{
//				if(jiguang && stoping && FollowManager.GroundOpenmvFramePtr->cnt1 )
//				{
//						P8OUT &= ~GPIO_PIN7;
//				}
//				else
//				{
//						P8OUT |= GPIO_PIN7;
//				}
//		}
		
}

/******************* (C) ��Ȩ���� 2018 �����пƺƵ�Ƽ����޹�˾ *******************/
