#ifndef __MPU6050_H
#define __MPU6050_H
//�ⲿ�ļ�����
#include "include.h"
#include "i2c.h"
#include "stdbool.h"


//��������
bool MPU6050Init(void);
void GetMPU6050Data(void);
void GetMPU6050Offset(void);

#endif
