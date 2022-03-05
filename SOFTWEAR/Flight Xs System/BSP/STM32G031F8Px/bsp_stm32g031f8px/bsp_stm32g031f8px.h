#ifndef _BSP_STM32G031F8Px
#define _BSP_STM32G031F8Px

#include <stdint.h>
#include <usart.h>

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

//私有接口区
uint8_t IIC_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
uint8_t IIC_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
uint8_t UART_Write(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);


//外部调用区
void Update_Motor(uint8_t Motorx,uint8_t Duty);    //设置电机Motorx的占空比为Duty%
uint8_t BT_UART_TX(char* msg);
uint8_t BT_UART_RX(BT_RX_DATA_t* data);
uint8_t DEBUG_UART_TX(char* msg);
uint8_t MPU6050_IIC_Write();
uint8_t MPU6050_IIC_Read();
uint8_t SPL06_IIC_Write();
uint8_t SPL06_IIC_Read();

void DEBUG_LED_ON();
void DEBUG_LED_OFF();
void STATUS_LED_ON();
void STATUS_LED_OFF();
uint8_t GET_BAT_ADC();
uint8_t GET_KEY_STATUS();

#endif
