#ifndef _BSP_STM32G031F8Px
#define _BSP_STM32G031F8Px

#include <stdint.h>
#include <usart.h>


void Update_Motor(uint8_t Motorx,uint8_t Duty);    //设置电机Motorx的占空比为Duty%
uint8_t IIC_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
uint8_t IIC_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
uint8_t UART_Write(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

void DEBUG_LED_ON();
void DEBUG_LED_OFF();
void STATUS_LED_ON();
void STATUS_LED_OFF();
uint8_t GET_BAT_ADC();
uint8_t GET_KEY_STATUS();

#endif
