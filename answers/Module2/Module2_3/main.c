/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "LCD2004.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	int currentKey;
	int lastKey;
	int readKey;
	void setVCC() { // функция подачи 1 на столбцы
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);
	}
	int readRow() { // функция чтения строк
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == 0) {
			return 0;
		} else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) == 0) {
			return 1;
		} else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2) == 0) {
			return 2;
		} else if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3) == 0) {
			return 3;
		}
		return -1;
	}

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  LCD_init_port("D0","E",0); // назначение портов для ЖКИ
  LCD_init_port("D1","E",1);
  LCD_init_port("D2","E",2);
  LCD_init_port("D3","E",3);
  LCD_init_port("D4","E",4);
  LCD_init_port("D5","E",5);
  LCD_init_port("D6","E",6);
  LCD_init_port("D7","E",7);
  LCD_init_port("RS","E",8);
  LCD_init_port("E","E",9);
  LCD_init();
  LCD_set_cursor(0,0);
  LCD_print("LAST PRESSED KEY - ", huart1);
  currentKey = -1;
  lastKey = currentKey;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  int i;
	  for (i = 0; i < 3; i++) { // в цикле опрашиваем каждый столбец
		  setVCC(); // подаём 1 на все столбцы
		  switch (i){ // поочерёдно подаём 0 на каждый столбец
		  	case 0:
		  		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 0);
		  	 break;
		  	case 1:
		  		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
		  	break;
		  	case 2:
		  		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 0);
		  	break;
		  }
		  readKey = readRow(); // читаем нажатую кнопку
		  if (readKey != -1) {
			  currentKey = readKey*3+i; // определяем номер клавиши
		  }
	  }
	  if (currentKey != lastKey) {
		  LCD_set_cursor(19,0);
		  switch (currentKey) { // в зависимости от номера клавиши отправляем разный ASCII код
		  	case 0:
		  		LCD_ASCII(49, huart1);
		  	break;
		  	case 1:
		  		LCD_ASCII(50, huart1);
		  	break;
		  	case 2:
		  		LCD_ASCII(51, huart1);
		  	break;
		  	case 3:
		  		LCD_ASCII(52, huart1);
		  	break;
		  	case 4:
		  		LCD_ASCII(53, huart1);
		  	break;
		  	case 5:
		  		LCD_ASCII(54, huart1);
		  	break;
		  	case 6:
		  		LCD_ASCII(55, huart1);
		  	break;
		  	case 7:
		  		LCD_ASCII(56, huart1);
		  	break;
		  	case 8:
		  		LCD_ASCII(57, huart1);
		  	break;
		  	case 9:
		  		LCD_ASCII(42, huart1);
		  	break;
		  	case 10:
		  		LCD_ASCII(48, huart1);
		  	break;
		  	case 11:
		  		LCD_ASCII(35, huart1);
		  	break;
		  }
		  lastKey = currentKey;
	  }
	  HAL_Delay(200);

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_2
                          |GPIO_PIN_5|GPIO_PIN_0|GPIO_PIN_6|GPIO_PIN_8
                          |GPIO_PIN_7|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE3 PE1 PE4 PE2
                           PE5 PE0 PE6 PE8
                           PE7 PE9 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_2
                          |GPIO_PIN_5|GPIO_PIN_0|GPIO_PIN_6|GPIO_PIN_8
                          |GPIO_PIN_7|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD5 PD6 PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD3 PD1 PD2 PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
