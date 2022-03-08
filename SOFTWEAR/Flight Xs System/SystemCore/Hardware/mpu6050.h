#ifndef __MPU6050_H
#define __MPU6050_H
//外部文件引用
#include "include.h"
#include "i2c.h"
#include "stdbool.h"


//函数声明
bool MPU6050Init(void);
void GetMPU6050Data(void);
void GetMPU6050Offset(void);

#endif
