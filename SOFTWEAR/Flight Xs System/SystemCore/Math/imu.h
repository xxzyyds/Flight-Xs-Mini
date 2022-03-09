
#ifndef __IMU_H__
#define __IMU_H__

//外部文件引用
#include "fmuConfig.h"
#include "main.h"
#include "StatusConfig.h"
//宏定义区

//函数声明
float GetNormAccz(void);

void ATT_Update(const f_MPU *pMpu, f_AL *pAngE, float dt);
void GetAngle(f_AL *pAngE);
void IMU_Reset(void);

#endif

