/*==============================================================================
                         ##### How to use this driver #####
==============================================================================
MPU6050������ʹ�÷������£�
1.����MPU6050Init�������鿴��ǰMPU6050�Ƿ��ʼ���ɹ���
2.�̶����ڵ���GetMPU6050Data���Ի�ȡ���������ݣ�
PS�����������ݴ���� MPU6050 ��

*/
//�ⲿ�ļ�����
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



//�궨����
#define SMPLRT_DIV          0x19    //�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define CONFIGL             0x1A    //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define GYRO_CONFIG         0x1B    //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define ACCEL_CONFIG        0x1C    //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
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
#define PWR_MGMT_1          0x6B    //��Դ��������ֵ��0x00(��������)
#define WHO_AM_I            0x75    //IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define MPU6050_PRODUCT_ID  0x68
#define MPU6052C_PRODUCT_ID 0x72
#define MPU6050_ADDRESS     0xD0    //0x68

#define Acc_Read()          MPU6050_IIC_Read_Bytes(MPU6050_ADDRESS, 0x3B, buffer, 6) //��ȡ���ٶ�
#define Gyro_Read()         MPU6050_IIC_Read_Bytes(MPU6050_ADDRESS, 0x43, &buffer[6], 6)  //  ��ȡ���ٶ�


// MPU6050 ԭʼ����
int16_t *pMpu = (int16_t *)&MPU6050;

/******************************************************************************
  * �������ƣ�MPU6050Init
  * ����������MPU6050�ĳ�ʼ��
  * ��    �룺void
  * ��    ����MPU6050��ʼ�����   
              0:��ʼ���ɹ�
              1:��ʼ��ʧ��
  * ��    �أ� 
  * ��    ע��    
  *    
  *
******************************************************************************/
bool MPU6050Init(void) //��ʼ��
{
    uint8_t check = 0;

    check = MPU6050_IIC_Read_Byte(MPU6050_ADDRESS, 0x75);  //�ж�MPU6050��ַ
    
    if(check != MPU6050_PRODUCT_ID) //�����ַ����ȷ
    {
        MPU6050.Check = false;
        return false;
    }
    else
    {
        HAL_Delay(200);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, PWR_MGMT_1,    0x80);   //��λ
        HAL_Delay(200);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, SMPLRT_DIV,   0x00);   //�����ǲ����ʣ�0x00(1000Hz)
        HAL_Delay(10);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, PWR_MGMT_1,   0x03);   //�����豸ʱ��Դ��������Z��
        HAL_Delay(10);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, CONFIGL,      0x04);   //��ͨ�˲�Ƶ�ʣ�0x03(42Hz)
        HAL_Delay(10);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, GYRO_CONFIG,  0x18);   //+-2000deg/s
        HAL_Delay(10);
        MPU6050_IIC_Write_Byte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x18);   //+-16
        HAL_Delay(10);
        
        GetMPU6050Offset(); //����У׼����
        MPU6050.Check = true;
        return true;
    }
}

/******************************************************************************
  * �������ƣ�GetMPU6050Data
  * ������������ȡ�����Ǻͼ��ٶȼƵ����ݲ����˲�����
  * ��    �룺void
  * ��    ����void
  * ��    �أ�void
  * ��    ע��null
  *    
  *
******************************************************************************/

void GetMPU6050Data(void) 
{
    static float mpu_filter[2][6];
    int16_t mpu_filter_tmp[6];
    uint8_t buffer[12];

    //��ȡ���ٶȼ����ݺ�����������
    Acc_Read();
    Gyro_Read();

    
    for(int i = 0; i < 6; i++)
    {
        //ƴ�Ӷ�ȡ����ԭʼ����
        mpu_filter_tmp[i] = (((int16_t)buffer[i << 1] << 8) | buffer[(i << 1) + 1])
                - MPU6050.Offset[i];

        //ԭʼ����LPF
        mpu_filter[0][i] += 0.3f *(mpu_filter_tmp[i] - mpu_filter[0][i]);
        mpu_filter[1][i] += 0.3f *(mpu_filter[0][i]  - mpu_filter[1][i]);

        //��ֵ���ṹ��
        pMpu[i] = (int16_t)mpu_filter[1][i];
    }
}

/******************************************************************************
  * �������ƣ�GetMPU6050Offset
  * ������������ȡMPU6050��̬�´�����ƫ��
  * ��    �룺void
  * ��    ����void
  * ��    �أ�void
  * ��    ע��null
  *    
  *
******************************************************************************/
void GetMPU6050Offset(void) //У׼
{
    int32_t buffer[6] = { 0 };
    int16_t i = 0;  
    const int8_t MAX_GYRO_QUIET = 5;

    int16_t LastGyro[3] = {0};          /*wait for calm down*/
    int16_t ErrorGyro[3] = {0};         /*set offset initial to zero*/
    
    memset(MPU6050.Offset, 0, 12);
    MPU6050.Offset[2] = 2048;   //�����ֲ������趨���ٶȱ궨ֵ 

    //����ǰ300������
    for(int i = 0;i < 300;i++)
    {   
        HAL_Delay(2);
        GetMPU6050Data();
    }
    
    //�жϷɻ��Ƿ�ƽ������ֹб�ſ���
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
    
    //�ж��ɻ��Ƿ��Ѿ��ȶ�
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

    //ȡ��100����356���ƽ��ֵ��ΪУ׼ֵ
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

    //����У׼ֵ
    for(i = 0; i < 6; i++)
    {
        //����8λ�����ݳ���256
        MPU6050.Offset[i] = buffer[i] >> 8;
    }
}

/******************* (C) ��Ȩ���� 2018 �����пƺƵ�Ƽ����޹�˾ *******************/
