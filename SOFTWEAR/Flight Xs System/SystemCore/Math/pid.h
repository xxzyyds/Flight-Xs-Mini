/**
  ******************************************************************************
  * Copyright (c) 2018,北京中科浩电科技有限公司
  * All rights reserved.
  * 文件名称：pid.h
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
#ifndef __PID_H
#define __PID_H
//外部文件引用
#include <stdbool.h>
#include "stdint.h"
#include "pidConfig.h"

//宏定义区


//函数声明
void reset_i(PID* pid);
void ResetPID(void);
void ClacCascadePID(PID* pidRate, PID* pidAngE, const float dt);  //串级PID
void UpdatePID(PID* pid, const float dt);  //PID
#endif

/******************* (C) 版权所有 2018 北京中科浩电科技有限公司 *******************/
