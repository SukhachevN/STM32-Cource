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
#include "LCD.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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
	int currentKey;
	int readKey;
	void setVCC() {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
	}
	int readRow() {
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
	LCD_init_port("D0", "E", 0); // назначение портов для ЖКИ
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
	char accum[3] = { }; // буфер хранения введенного числа
	int currentPos = 0; // позиция текущей цифры
	int wasPressed = 0; // флаг была ли нажата кнопка
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		int i;
		for (i = 0; i < 4; i++) { // в цикле опрашиваем каждый столбец
			setVCC(); // подаём 1 на все столбцы
			switch (i) { // поочерёдно подаём 0 на каждый столбец
			case 0:
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 0);
				break;
			case 1:
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
				break;
			case 2:
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 0);
				break;
			case 3:
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
				break;
			}
			readKey = readRow(); // читаем нажатую кнопку
			if (readKey != -1) {
				currentKey = readKey * 4 + i; // определяем номер клавиши
				wasPressed = 1;
			}
		}
		if (wasPressed == 1) { // если была нажата кнопка
			int isOperation = 0; // флаг была ли кнопка операцией
			int number = -1; // переменная хранения значения нажатой кнопки
			// преобразовать номер нажатой кнопки в цифру соответствующую этой кнопке
			switch (currentKey) {
			case 0:
			case 1:
			case 2:
				number = 7 + currentKey;
				break;
			case 4:
			case 5:
			case 6:
				number = currentKey;
				break;
			case 8:
			case 9:
			case 10:
				number = currentKey - 7;
				break;
			case 11: // нажата кнопка минус
				if (atoi(accum) > 0) {
					sprintf(accum, "-%d", atoi(accum)); // записать текущее число со знаком минус
					if (strlen(accum) != 3) { // сдвинуть счётчик позиции текцщей цифры
						currentPos = strlen(accum);
					} else {
						currentPos = 2;
					}
				}
				break;
			case 12: // стереть последний символ
				accum[currentPos] = *"";
				if (currentPos != 0) {
					currentPos--; // сдвинуть позицию текущей цифры
				}
				LCD_init();
				LCD_print(accum); // вывести на ЖКИ
				isOperation = 1; // установить флаг операции
				wasPressed = 0;
				break;
			case 13:
				number = 0;
				break;
			case 14:
				HAL_UART_Transmit(&huart1, (uint8_t*) accum, 3, 100); // отправить число по УАРТ
				isOperation = 1; // установить флаг операции
				wasPressed = 0;
				memset(accum, 0, 3); // очистить буфер
				currentPos = 0;
				LCD_init();
				HAL_Delay(200);
				break;
			case 15: // если кнопка плюс
				if (atoi(accum) < 0) { // сделать число положительным
					sprintf(accum, "%d", -atoi(accum));
					if (strlen(accum) != 3) {
						currentPos = strlen(accum); // сдвинуть позицию текущей цифры
					} else {
						currentPos = 2;
					}
				}
				break;
			}
			if (isOperation == 0) { // если не операция
				if (number != -1) {
					char digit[1];
					sprintf(digit, "%d", number);
					accum[currentPos] = digit[0]; // записать нажатую цифру в буфер
					if (currentPos != 2) {
						currentPos++;
					}
				}
				LCD_init();
				LCD_print(&accum[0]); // вывести на ЖКИ
				wasPressed = 0;
			}
			HAL_Delay(200);
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
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE,
			GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_2 | GPIO_PIN_5
					| GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_7
					| GPIO_PIN_9, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_4,
			GPIO_PIN_RESET);

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

	/*Configure GPIO pins : PD7 PD5 PD6 PD4 */
	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : PD3 PD1 PD2 PD0 */
	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_0;
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
