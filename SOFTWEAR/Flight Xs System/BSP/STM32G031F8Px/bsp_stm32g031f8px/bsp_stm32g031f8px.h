#ifndef _BSP_STM32G031F8Px
#define _BSP_STM32G031F8Px

#include <stdint.h>
#include <usart.h>
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

typedef struct{
	uint8_t head;
	uint8_t thr;
	uint8_t roll;
	uint8_t yaw;
	uint8_t pitch;
	uint8_t aux1;
	uint8_t aux2;
	uint8_t tail;
}BT_RX_DATA_t;


//外部调用区
void BSP_Init(void);
void Update_Motor(uint8_t Duty1,uint8_t Duty2,uint8_t Duty3,uint8_t Duty4);    //设置电机Motorx的占空比为Dutyx‰
uint8_t BT_UART_TX(char* msg);
uint8_t BT_UART_RX(BT_RX_DATA_t* data);
uint8_t DEBUG_UART_TX(char* msg);

uint8_t MPU6050_IIC_Write_Byte(uint16_t DevAddress, uint16_t MemAddress, uint8_t pData);
uint8_t MPU6050_IIC_Read_Byte(uint16_t DevAddress, uint16_t MemAddress);
uint8_t MPU6050_IIC_Read_Bytes(uint16_t DevAddress, uint16_t MemAddress, uint8_t *TMP, uint8_t len);
uint8_t SPL06_IIC_Write_Byte(uint16_t DevAddress, uint16_t MemAddress, uint8_t pData);
uint8_t SPL06_IIC_Read_Byte(uint16_t DevAddress, uint16_t MemAddress);
uint8_t SPL06_IIC_Read_Bytes(uint16_t DevAddress, uint16_t MemAddress, uint8_t *TMP, uint8_t len);
uint8_t SPL06_IIC_Write_Bytes(uint16_t DevAddress, uint16_t MemAddress, uint8_t *TMP, uint8_t len);

void DEBUG_LED_ON(void);
void DEBUG_LED_OFF(void);
void STATUS_LED_ON(void);
void STATUS_LED_OFF(void);
uint8_t GET_BAT_ADC(void);
uint8_t GET_KEY_STATUS(void);





//私有接口区
uint8_t UART_Write(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
void SystemClock_Config(void);


#endif
