#ifndef __FMU_CONFIG_H
#define __FMU_CONFIG_H
//外部文件引用
#include "SPL06.h"
//宏定义区
/*开启相关硬件*/
//设备选择区
#define ZKHD_HAWK_I2C_ENABLE
#define ZKHD_HAWK_SPI_ENABLE
#define ZKHD_HAWK_USART1_ENABLE
#define ZKHD_HAWK_USART2_ENABLE
#define ZKHD_HAWK_MOTOR_LED_ENABLE
#define ZKHD_HAWK_RGB_LED_ENABLE
#define ZKHD_HAWK_ADC_ENABLE
#define ZKHD_HAWK_FLASH_ENABLE
#define ZKHD_HAWK_GPIO_ENABLE

/*设备总线选择定义*/
#ifdef HARDWARE_NRF24L01 
#define NRF_RW  Hardware_SPI1_RW
#define SET_NRF_CSN                 Hardware_SPI1_Device1_CSN_H
#define CLR_NRF_CSN                 Hardware_SPI1_Device1_CSN_L
#define NRF24L01CE_H                GPIO_Pin1_Set(1)
#define NRF24L01CE_L                GPIO_Pin1_Set(0)
#define READ_NRF24L01_IRQ           (GPIOA->IDR&GPIO_PIN_15)          //IRQ主机数据输入 PA15
#endif



#define STM32


#define DEBUG_PORT             A2
#define DEBUG
#define FIRMWARE_INFO          102

/*高度控制常量*/

#define MAX_ISFD_ATTITUDE      40
#define MIN_ALT_CM             50
#define MAX_ALT_CM             200
#define MAX_ALT_RATE           0.05
#define FIX_ALT_RATE           20
#define MAX_REMOTE_THROTTLE    2000
#define THROTTLE_DEAD_ZONE     0.2f

#define MOTOR_MAX_INIT_VALUE   850
#define MOTOR_MAX_VALUE   1000

/*
const float M_PI = 3.1415926535;
const float RtA = 57.2957795f;
const float AtR = 0.0174532925f;
const float Gyro_G = 0.03051756f * 2;          
const float Gyro_Gr = 0.0005326f * 2;    
const float PI_2 = 1.570796f;
*/
//数据结构声明

typedef struct{
    int16_t accX;
    int16_t accY;
    int16_t accZ;
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;
  
    int16_t Offset[6];
    bool Check;
}f_MPU;

extern f_MPU MPU6050;

//数据结构声明


typedef struct
{    
    int16_t i16C0;
    int16_t i16C1;
    int32_t i32C00;
    int32_t i32C10;
    int16_t i16C01;
    int16_t i16C11;
    int16_t i16C20;
    int16_t i16C21;
    int16_t i16C30;       
}SPL06;

typedef struct
{    
    int16_t c0;
    int16_t c1;
    int32_t c00;
    int32_t c10;
    int16_t c01;
    int16_t c11;
    int16_t c20;
    int16_t c21;
    int16_t c30;       
}SPL06Param;

typedef struct
{    
    SPL06Param Param;
    uint8_t u8Chip_id;
    int32_t i32RawPressure;
    int32_t i32RawTemperature;
    int32_t i32KP;
    int32_t i32KT;
    
    float fGround_Alt;
    float fALT;                  //height above sea level        
    float fRelative_Alt;
    
    float fTemperature;
    float fPressure;
    float fLast_Pressure;
    
    float fOffset;
    bool Check;
}SPL;

typedef struct
{
    Device_t Device;
    uint8_t ChipId;                  /*产品ID | 版本ID*/
    SPL06Param    SPL06Param;
    float SPL06Temperature;
    float SPL06Press;
    int32_t RawPressure;
    int32_t RawTemperature;
    int32_t RawAltitude;            /*原始海拔高度 cm*/
    int32_t i32RawAltitude;
    int32_t Kp;
    int32_t Kt;
//    xQueueHandle Spl06DeviceQueue;
}SPL06_t;

//Extern引用
extern SPL f_SPL06;
extern SPL06 f_SPL06Params;
extern SPL06_t device_SPL06;

//函数声明

#endif

