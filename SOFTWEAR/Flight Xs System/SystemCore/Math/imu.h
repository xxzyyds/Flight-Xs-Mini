
#ifndef __IMU_H
#define __IMU_H
//外部文件引用
#include "main.h"
#include "StatusConfig.h"
#include "fmuConfig.h"
//宏定义区

//函数声明
float GetNormAccz(void);

void ATT_Update(const f_MPU *pMpu, f_AL *pAngE, float dt);
void GetAngle(f_AL *pAngE);
void IMU_Reset(void);

#endif

