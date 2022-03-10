/*==============================================================================
                         ##### How to use this driver #####
==============================================================================
MPU6050驱动的使用方法如下：
1.调用MPU6050Init函数，查看当前MPU6050是否初始化成功；
2.固定周期调用GetMPU6050Data，以获取传感器数据；
PS：传感器数据存放在 MPU6050 中

*/
//外部文件引用
#include "include.h"
#include "mpu6050.h"
#include "filter.h"
#include <string.h>
#include "LED.h"
#include "myMath.h"
#include "kalman.h"
#include "bsp_stm32g031f8px.h"
#include "fmuConfig.h"
#include "timer_drv.h"



//宏定义区
#define SMPLRT_DIV          0x19    //陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIGL             0x1A    //低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG         0x1B    //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG        0x1C    //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_ADDRESS       0x3B
#define ACCEL_XOUT_H        0x3B
#define ACCEL_XOUT_L        0x3C
#define ACCEL_YOUT_H        0x3D
#define ACCEL_YOUT_L        0x3E
#define ACCEL_ZOUT_H        0x3F
#define ACCEL_ZOUT_L        0x40
#define TEMP_OUT_H          0x41
#define TEMP_OUT_L          0x42
#define GYRO_XOUT_H         0x43
#define GYRO_XOUT_L         0x44    
#define GYRO_YOUT_H         0x45
#define GYRO_YOUT_L         0x46
#define GYRO_ZOUT_H         0x47
#define GYRO_ZOUT_L         0x48
#define PWR_MGMT_1          0x6B    //电源管理，典型值：0x00(正常启用)
#define WHO_AM_I            0x75    //IIC地址寄存器(默认数值0x68，只读)
#define MPU6050_PRODUCT_ID  0x68
#define MPU6052C_PRODUCT_ID 0x72
#define MPU6050_ADDRESS     0xD0    //0x68

#define Acc_Read()          MPU6050_IIC_Read_Bytes(MPU6050_ADDRESS, 0x3B, buffer, 6) //读取加速度
#define Gyro_Read()         MPU6050_IIC_Read_Bytes(MPU6050_ADDRESS, 0x43, &buffer[6], 6)  //  读取角速度


// MPU6050 原始数据
int16_t *pMpu = (int16_t *)&MPU6050;

/******************************************************************************
  * 函数名称：MPU6050Init
  * 函数描述：MPU6050的初始化
  * 输    入：void
  * 输    出：MPU6050初始化结果   
              0:初始化成功
              1:初始化失败
  * 返    回： 
  * 备    注：    
  *    
  *
******************************************************************************/
bool MPU6050Init(void) //初始化
{
    uint8_t check = 0;

    check = MPU6050_IIC_Read_Byte(MPU6050_ADDRESS, 0x75);  //判断MPU6050地址
    
    if(check != MPU6050_PRODUCT_ID) //如果地址不正确
    {
        MPU6050.Check = false;
        return false;
    }
    else
    {
        HAL_Delay(200);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, PWR_MGMT_1,    0x80);   //复位
        HAL_Delay(200);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, SMPLRT_DIV,   0x00);   //陀螺仪采样率，0x00(1000Hz)
        HAL_Delay(10);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, PWR_MGMT_1,   0x03);   //设置设备时钟源，陀螺仪Z轴
        HAL_Delay(10);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, CONFIGL,      0x04);   //低通滤波频率，0x03(42Hz)
        HAL_Delay(10);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, GYRO_CONFIG,  0x18);   //+-2000deg/s
        HAL_Delay(10);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x18);   //+-16
        HAL_Delay(10);
        
        GetMPU6050Offset(); //调用校准数据
        MPU6050.Check = true;
        return true;
    }
}

/******************************************************************************
  * 函数名称：GetMPU6050Data
  * 函数描述：读取陀螺仪和加速度计的数据并做滤波处理
  * 输    入：void
  * 输    出：void
  * 返    回：void
  * 备    注：null
  *    
  *
******************************************************************************/

void GetMPU6050Data(void) 
{
    static float mpu_filter[2][6];
    int16_t mpu_filter_tmp[6];
    uint8_t buffer[12];

    //读取加速度计数据和陀螺仪数据
    Acc_Read();
    Gyro_Read();

    
    for(int i = 0; i < 6; i++)
    {
        //拼接读取到的原始数据
        mpu_filter_tmp[i] = (((int16_t)buffer[i << 1] << 8) | buffer[(i << 1) + 1])
                - MPU6050.Offset[i];

        //原始数据LPF
        mpu_filter[0][i] += 0.3f *(mpu_filter_tmp[i] - mpu_filter[0][i]);
        mpu_filter[1][i] += 0.3f *(mpu_filter[0][i]  - mpu_filter[1][i]);

        //赋值到结构体
        pMpu[i] = (int16_t)mpu_filter[1][i];
    }
}

/******************************************************************************
  * 函数名称：GetMPU6050Offset
  * 函数描述：获取MPU6050静态下传感器偏差
  * 输    入：void
  * 输    出：void
  * 返    回：void
  * 备    注：null
  *    
  *
******************************************************************************/
void GetMPU6050Offset(void) //校准
{
    int32_t buffer[6] = { 0 };
    int16_t i = 0;  
    const int8_t MAX_GYRO_QUIET = 5;

    int16_t LastGyro[3] = {0};          /*wait for calm down*/
    int16_t ErrorGyro[3] = {0};         /*set offset initial to zero*/
    
    memset(MPU6050.Offset, 0, 12);
    MPU6050.Offset[2] = 2048;   //根据手册量程设定加速度标定值 

    //丢弃前300个数据
    for(int i = 0;i < 300;i++)
    {   
        HAL_Delay(2);
        GetMPU6050Data();
    }
    
    //判断飞机是否平方，防止斜放开机
    while(1)
    {
        static int cnt = 0;
        cnt++;
        
        HAL_Delay(2);
        GetMPU6050Data();
        
        if(MPU6050.accX < 400 && MPU6050.accX > -400 && 
           MPU6050.accY < 400 && MPU6050.accY > -400 && 
           MPU6050.accZ < 400 && MPU6050.accZ > -400)
        {
            if (cnt > 2 * 500 * 2)
            {
                break;
            }
        }else
        {
            cnt = 0;
        }
    }
    
    //判定飞机是否已经稳定
    while(1)
    {
        if(ABS(MPU6050.gyroX) != 0 ||  
           ABS(MPU6050.gyroY) != 0 || 
           ABS(MPU6050.gyroZ) != 0)
        {
            for(i = 0; i < 3; i++)
            {
                ErrorGyro[i] = pMpu[i + 3] - LastGyro[i];
                LastGyro[i] = pMpu[i + 3];    
            }
            
            if(ABS(ErrorGyro[0]) < MAX_GYRO_QUIET && 
               ABS(ErrorGyro[1]) < MAX_GYRO_QUIET && 
               ABS(ErrorGyro[2]) < MAX_GYRO_QUIET)
            {
                break;
            }
        }
    }

    //取第100到第356组的平均值做为校准值
    for(i = 0; i < 356; i++)  
    {
        GetMPU6050Data();
        
        if(100 <= i)
        {
            for(int k = 0; k < 6; k++)
            {
                buffer[k] += pMpu[k];
            }
        }
    }

    //保存校准值
    for(i = 0; i < 6; i++)
    {
        //右移8位，数据除以256
        MPU6050.Offset[i] = buffer[i] >> 8;
    }
}

/******************* (C) 版权所有 2018 北京中科浩电科技有限公司 *******************/
