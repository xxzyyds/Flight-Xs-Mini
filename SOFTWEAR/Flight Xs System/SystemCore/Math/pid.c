/**
  ******************************************************************************
  * Copyright (c) 2018,北京中科浩电科技有限公司
  * All rights reserved.
  * 文件名称：pid.c
  * 摘    要：
  *
  * 当前版本：V1.0
  * 作    者：北京中科浩电科技有限公司研发部 
  * 完成日期：    
  * 修改说明：
  * 
  *
  * 历史版本：
  *
  *
  *******************************************************************************/

/*==============================================================================
                         ##### How to use this driver #####
==============================================================================
PID驱动使用方法如下：
1.构建一个PID结构体，将所需要控制的数据存放进去；
2.调UpdatePID函数，计算PID输出结果
3.可以直接调用ClacCascadePID直接计算串级PID


*/
//外部文件引用
#include "pid.h"
#include "myMath.h"    


//宏定义区



//Extern引用



//私有函数区



//私有变量区
/*PID工程变量*/
PID PIDGroup[emNum_Of_PID_List];


/******************************************************************************
  * 函数名称：ResetPID
  * 函数描述：复位PID
  * 输    入：PID结构体指针
  * 输    出：void
  * 返    回：void
  * 备    注：null
  *    
  *
******************************************************************************/
void ResetPID(void)
{
    for(int i = 0; i < emNum_Of_PID_List; i++)
    {
        PIDGroup[i].integ = 0;
        PIDGroup[i].prevError = 0;
        PIDGroup[i].out = 0;
        PIDGroup[i].offset = 0;
    }
    
    PIDGroup[PID_Height_Pos].desired = 80;
}

void reset_i(PID* pid)
{
    pid->integ = 0;
}

/******************************************************************************
  * 函数名称：UpdatePID
  * 函数描述：计算PID相关值
  * 输    入：PID* pid：要计算的PID结构体指针
              float dt：单位运行时间
  * 输    出：void
  * 返    回：void
  * 备    注：null
  *    
  *
******************************************************************************/
void UpdatePID(PID* pid, const float dt)
{
    float deriv;
    
    pid->Err = pid->desired - pid->measured + pid->offset; //当前角度与实际角度的误差

    
    if(pid->Err < pid->DeathArea && pid->Err > -pid->DeathArea)
    {
        pid->Err = 0;
    }
    
    if(pid->Err_LimitHigh != 0 && pid->Err_LimitLow != 0)
    {
        pid->Err = LIMIT(pid->Err, pid->Err_LimitLow, pid->Err_LimitHigh);
    }
    
    pid->integ += pid->Err * dt;    
    
    if(pid->IntegLimitHigh != 0 && pid->IntegLimitLow != 0)
    {
        pid->integ = LIMIT(pid->integ, pid->IntegLimitLow, pid->IntegLimitHigh);
    }
    
    //deriv = (pid->Err - pid->prevError)/dt;  
    deriv = -(pid->measured - pid->prevError)/dt;
		
    pid->out = pid->kp * pid->Err + pid->ki * pid->integ + pid->kd * deriv;//PID输出
    
    if(pid->OutLimitHigh != 0 && pid->OutLimitLow != 0)
    {
        pid->out = LIMIT(pid->out, pid->OutLimitLow, pid->OutLimitHigh);
    }
    
    pid->prevError = pid->measured;//pid->Err;  微分先行（变式）用法
}

/******************************************************************************
  * 函数名称：ClacCascadePID
  * 函数描述：计算串级PID
  * 输    入：PID* pidRate：PID速度环
              PID* pidAngE：PID角度环
              const float dt：单位运行时间
  * 输    出：void
  * 返    回：void
  * 备    注：null    
  *    
  *
******************************************************************************/
void ClacCascadePID(PID* pidRate, PID* pidAngE, const float dt)  //串级PID
{     
    UpdatePID(pidAngE, dt);    //先计算外环
    pidRate->desired = pidAngE->out;
    UpdatePID(pidRate, dt);   
}

/******************* (C) 版权所有 2018 北京中科浩电科技有限公司 *******************/
