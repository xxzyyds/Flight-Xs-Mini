
// 循环核心驱动


// 外部文件引用
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

//私有函数区
void Update(void);
void UpdateUSBQueue(void);

            
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
	
    //时间段轮询计数
    Cnt++;
	
		if ((Cnt+1) % 3 == 0){
				// 获取MPU6050的原始数据
        GetMPU6050Data();
        
        // 姿态更新 四元数
				// 更新出 Z轴 的 加速度
        ATT_Update(&MPU6050, &FlightAttitude, 0.003f); 
        
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
		
}
