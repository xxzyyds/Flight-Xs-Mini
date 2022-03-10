#include <bsp_stm32g031f8px.h>
#include <string.h>



uint8_t UART_Write(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
		return HAL_UART_Transmit_DMA(huart,pData,Size);
}
uint8_t BT_UART_TX(char* msg)
{
	return HAL_UART_Transmit_DMA(&huart1,(uint8_t*)msg,strlen(msg));
}
uint8_t BT_UART_RX(BT_RX_DATA_t* data)
{
	return 0;
}
uint8_t DEBUG_UART_TX(char* msg)
{
	return 0;
}
void DEBUG_LED_ON()
{
	return;
}

void DEBUG_LED_OFF()
{
	return;
}

void STATUS_LED_ON()
{
	return;
}

void STATUS_LED_OFF()
{
	return;
}

uint8_t GET_BAT_ADC()
{
	return 0;
}

uint8_t GET_KEY_STATUS()
{
	return 0;
}








void BSP_Init()
{
	HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
	
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	
	Update_Motor(0,0,0,0);
}

void Update_Motor(uint8_t Duty1,uint8_t Duty2,uint8_t Duty3,uint8_t Duty4)    //设置电机Motorx的占空比为Dutyx‰
{
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,Duty1);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,Duty2);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,Duty3);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,Duty4);
}

// MPU6050
uint8_t MPU6050_IIC_Write_Byte(uint16_t DevAddress, uint16_t MemAddress, uint8_t pData)
{
	return HAL_I2C_Mem_Write(&hi2c1,DevAddress,MemAddress,I2C_MEMADD_SIZE_8BIT,&pData,0x01,0xff);
}
uint8_t MPU6050_IIC_Read_Byte(uint16_t DevAddress, uint16_t MemAddress)
{
	uint8_t TMP;
	HAL_I2C_Mem_Read(&hi2c1,DevAddress,MemAddress,I2C_MEMADD_SIZE_8BIT,&TMP,0x01,0xff);
	return TMP;
}

uint8_t MPU6050_IIC_Read_Bytes(uint16_t DevAddress, uint16_t MemAddress, uint8_t *TMP, uint8_t len)
{
	return HAL_I2C_Mem_Read(&hi2c1,DevAddress,MemAddress,I2C_MEMADD_SIZE_8BIT,TMP,len,0xff);
}

// SPL06
uint8_t SPL06_IIC_Write_Byte(uint16_t DevAddress, uint16_t MemAddress, uint8_t pData)
{
	return HAL_I2C_Mem_Write(&hi2c2,DevAddress,MemAddress,I2C_MEMADD_SIZE_8BIT,&pData,0x01,0xff);
}
uint8_t SPL06_IIC_Read_Byte(uint16_t DevAddress, uint16_t MemAddress)
{
	uint8_t TMP;
	HAL_I2C_Mem_Read(&hi2c2,DevAddress,MemAddress,I2C_MEMADD_SIZE_8BIT,&TMP,0x01,0xff);
	return TMP;
}
uint8_t SPL06_IIC_Read_Bytes(uint16_t DevAddress, uint16_t MemAddress, uint8_t *TMP, uint8_t len)
{
	return HAL_I2C_Mem_Read(&hi2c2,DevAddress,MemAddress,I2C_MEMADD_SIZE_8BIT,TMP,len,0xff);
}
uint8_t SPL06_IIC_Write_Bytes(uint16_t DevAddress, uint16_t MemAddress, uint8_t *TMP, uint8_t len)
{
	return HAL_I2C_Mem_Write(&hi2c2,DevAddress,MemAddress,I2C_MEMADD_SIZE_8BIT,TMP,len,0xff);
}



void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}
