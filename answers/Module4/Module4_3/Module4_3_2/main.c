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
int main(void) {
	/* USER CODE BEGIN 1 */

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
	char buffer[1];
	LCD_init_port("D0", "E", 0); // назначение портов для ЖК�?
	LCD_init_port("D1", "E", 1);
	LCD_init_port("D2", "E", 2);
	LCD_init_port("D3", "E", 3);
	LCD_init_port("D4", "E", 4);
	LCD_init_port("D5", "E", 5);
	LCD_init_port("D6", "E", 6);
	LCD_init_port("D7", "E", 7);
	LCD_init_port("RS", "E", 8);
	LCD_init_port("E", "E", 9);
	LCD_init();
	LCD_set_cursor(0, 0);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	HAL_UART_Receive(&huart1, buffer, 1, 100); // очистка UART перед началом работы
	char minus = *"-"; // переменные для знаков
	char plus = *"+";
	char div = *"/";
	char mult = *"x";
	char equal = *"=";
	int64_t result = 0; // результат
	int32_t operand = 0; // переменная для хранения операнда
	uint16_t high = 0; // переменная для хранения старших битов
	uint16_t low = 0; // переменная для хранения младших битов
	int isResultSet = 0; // флаг начала выражения
	char resultBuffer[100];
	char sign; // переменная для хранения знака
	while (1) {
		HAL_UART_Receive(&huart1, buffer, 1, HAL_MAX_DELAY); // получение числа, максимальная задержка, чтобы "зависнуть пока не придёт число"
		high = GPIOC->IDR; // получение старших бит
		low = GPIOA->IDR; // получение младших бит
		if (isResultSet == 1) { // проверка, записали ли первый операнд
			operand = (high << 16) + low; // запись 32-х разрядного операнда
			if (sign == minus) { // проверка на знак
				result = result - operand;
				sprintf(resultBuffer, "%d", result);
				LCD_init();
				LCD_print(resultBuffer, huart1); // вывод на ЖКИ
			} else if (sign == plus) {
				result = result + operand;
				LCD_init();
				sprintf(resultBuffer, "%d", result);
				LCD_print(resultBuffer, huart1);
			} else if (sign == div) {
				result = result / operand;
				LCD_init();
				sprintf(resultBuffer, "%d", result);
				LCD_print(resultBuffer, huart1);
			} else if (sign == mult) {
				result = result * operand;
				LCD_init();
				sprintf(resultBuffer, "%d", result);
				LCD_print(resultBuffer, huart1);
			} else if (sign == equal) {
				result = operand;
				LCD_init();
				sprintf(resultBuffer, "%d", result);
				LCD_print(resultBuffer, huart1);
			}
			if (buffer[0] == minus) { // запомнить знак
				sign = minus;
			} else if (buffer[0] == plus) {
				sign = plus;
			} else if (buffer[0] == div) {
				sign = div;
			} else if (buffer[0] == mult) {
				sign = mult;
			} else if (buffer[0] == equal) {
				isResultSet = 0;
				sign = equal;
			}
		} else { // запись самого первого операнда
			result = (high << 16) + low;
			isResultSet = 1;
			sign = buffer[0];
			LCD_init();
			sprintf(resultBuffer, "%d", result);
			LCD_print(resultBuffer, huart1);
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

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
	if (HAL_UART_Init(&huart1) != HAL_OK) {
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
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE,
			GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_2 | GPIO_PIN_5
					| GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_7
					| GPIO_PIN_9, GPIO_PIN_RESET);

	/*Configure GPIO pins : PE3 PE1 PE4 PE2
	 PE5 PE0 PE6 PE8
	 PE7 PE9 */
	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_2
			| GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_7
			| GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : PA15 PA14 PA13 PA12
	 PA11 PA10 PA9 PA8
	 PA2 PA5 PA0 PA3
	 PA6 PA1 PA4 PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12
			| GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8 | GPIO_PIN_2
			| GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_1
			| GPIO_PIN_4 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PC12 PC10 PC13 PC11
	 PC14 PC9 PC15 PC8
	 PC7 PC6 PC0 PC1
	 PC2 PC3 PC4 PC5 */
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_11
			| GPIO_PIN_14 | GPIO_PIN_9 | GPIO_PIN_15 | GPIO_PIN_8 | GPIO_PIN_7
			| GPIO_PIN_6 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
			| GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
