
// ѭ����������


// �ⲿ�ļ�����
#include "include.h"
#include "speed_estimator.h"
#include "led.h"
#include "battery.h"
#include "stdio.h"
#include "timer_drv.h"
#include "pos_ctrl.h"
#include "program_ctrl.h"
#include "mpu6050.h"
#include "imu.h"
#include "fmuConfig.h"
#include "StatusConfig.h"
#include "control.h"
#include "FollowLine.h"
#include "SDK.h"
#include "height_control.h"
#include "bsp_stm32g031f8px.h"

//˽�к�����
void Update(void);
void UpdateUSBQueue(void);

            
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

		if(!InitComplete) 
		{
			DEBUG_LED_ON();
			STATUS_LED_ON();
			return;
		}
		
		// todo : init and kernel check
		if(!timetick)
		{
			return;
		}
		timetick = 0;
	
    //ʱ�����ѯ����
    Cnt++;
	
		if ((Cnt+1) % 3 == 0){
				// ��ȡMPU6050��ԭʼ����
        GetMPU6050Data();
        
        // ��̬���� ��Ԫ��
				// ���³� Z�� �� ���ٶ�
        ATT_Update(&MPU6050, &FlightAttitude, 0.003f); 
        
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
		
}
