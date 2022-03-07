
// ѭ����������


// �ⲿ�ļ�����
#include "include.h"
#include "communication.h"
#include "speed_estimator.h"
#include "led.h"
#include "battery.h"
#include "stdio.h"
#include "HARDWARE_uart.h"
#include "timer_drv.h"
#include "pos_ctrl.h"
#include "program_ctrl.h"
#include "FollowLine.h"
#include "sdk.h"


//˽�к�����
void Update(void);
void UpdateUSBQueue(void);


//˽�б�����
            
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
void KernelPolling()
{
    static uint32_t Cnt = 0;

		// todo : init and kernel check
    
    //ʱ�����ѯ����
    Cnt++;
	
		if (Cnt % 3 == 0){
				// ��ȡMPU6050��ԭʼ����
        GetMPU6050Data();
        
        // ��̬���� ��Ԫ��
				// ���³� Z�� �� ���ٶ�
        ATT_Update(&g_MPUManager,&g_Attitude, 0.003f); 
        
        // Z������Ԥ��
        WZ_Est_Calcu(0.003f);
		}
	
    //333Hz,�޸ĵ��δ��ʱ������
    if (Cnt % 3 == 0)
    {        

			
        // PID Update
        FlightPidControl(0.003f);
        
        // Motor output update
        MotorControl();
    }
    
    //125Hz
    if (Cnt % 8 == 0)
    {
        // GetMPU6050Data();
        // ��̬���� ��Ԫ��
				// ���³� Z�� �� ���ٶ�
        // ATT_Update(&g_MPUManager,&g_Attitude, 0.003f); 
        // Z������Ԥ��
        // WZ_Est_Calcu(0.003f);
        //����ŷ����
			
        GetAngle(&FlightAttitude); 
    }
    
    //100Hz
    if (Cnt % 10 == 0)
    {        
        UpdateCentControl(0.01f);
        
        sdk_update(0.01f);
    }
    
    //50Hz����
    if (Cnt % 20 == 0)
    {

        //�۲⴫�������ݼ���
        WZ_Obs_Calcu(0.02f);
        
        //Z�����ݻ�������
        WZ_Fix_Calcu(0.02f);
        
        //�߶ȿ�����
        ALT_Ctrl(0.02f);        
        
        //λ�ÿ�����
        POS_Ctrl(0.02f);
    }

    //LED��ѵ����
    PollingLED();

    //������ѵ����
    PollingUSART();
		
}
