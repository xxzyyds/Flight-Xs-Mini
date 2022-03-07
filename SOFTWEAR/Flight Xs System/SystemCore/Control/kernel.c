
// 循环核心驱动


// 外部文件引用
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


//私有函数区
void Update(void);
void UpdateUSBQueue(void);


//私有变量区
            
/******************************************************************************
  * 函数名称：KernelPolling
  * 函数描述：核心轮询程序
  * 输    入：void
  * 输    出：void
  * 返    回：void 
  * 备    注：1ms运行一次    
  *    
  *
******************************************************************************/
void KernelPolling()
{
    static uint32_t Cnt = 0;

		// todo : init and kernel check
    
    //时间段轮询计数
    Cnt++;
	
		if (Cnt % 3 == 0){
				// 获取MPU6050的原始数据
        GetMPU6050Data();
        
        // 姿态更新 四元数
				// 更新出 Z轴 的 加速度
        ATT_Update(&g_MPUManager,&g_Attitude, 0.003f); 
        
        // Z轴数据预估
        WZ_Est_Calcu(0.003f);
		}
	
    //333Hz,修改到滴答计时器里面
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
        // 姿态更新 四元数
				// 更新出 Z轴 的 加速度
        // ATT_Update(&g_MPUManager,&g_Attitude, 0.003f); 
        // Z轴数据预估
        // WZ_Est_Calcu(0.003f);
        //更新欧拉角
			
        GetAngle(&FlightAttitude); 
    }
    
    //100Hz
    if (Cnt % 10 == 0)
    {        
        UpdateCentControl(0.01f);
        
        sdk_update(0.01f);
    }
    
    //50Hz任务
    if (Cnt % 20 == 0)
    {

        //观测传感器数据计算
        WZ_Obs_Calcu(0.02f);
        
        //Z轴数据互补修正
        WZ_Fix_Calcu(0.02f);
        
        //高度控制器
        ALT_Ctrl(0.02f);        
        
        //位置控制器
        POS_Ctrl(0.02f);
    }

    //LED轮训函数
    PollingLED();

    //串口轮训函数
    PollingUSART();
		
}
