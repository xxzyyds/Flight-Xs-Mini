
#ifndef __IMU_H
#define __IMU_H
//�ⲿ�ļ�����
#include "main.h"
#include "StatusConfig.h"
#include "fmuConfig.h"
//�궨����

//��������
float GetNormAccz(void);

void ATT_Update(const f_MPU *pMpu, f_AL *pAngE, float dt);
void GetAngle(f_AL *pAngE);
void IMU_Reset(void);

#endif

