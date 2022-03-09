
#ifndef __IMU_H__
#define __IMU_H__

//�ⲿ�ļ�����
#include "fmuConfig.h"
#include "main.h"
#include "StatusConfig.h"
//�궨����

//��������
float GetNormAccz(void);

void ATT_Update(const f_MPU *pMpu, f_AL *pAngE, float dt);
void GetAngle(f_AL *pAngE);
void IMU_Reset(void);

#endif

