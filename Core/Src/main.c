/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define PWM_MAX 255
#define STEP_COUNT 8

// Must be bigger than biggest point coordinate
#define MAX_COORD_VALUE 50

struct Vec2i16{
	int16_t x;
	int16_t y;
};

/* Array contain points coordinate
 * x=0 - is left, y=0 - is down */
struct Vec2i16 rawFig[] = {
		{13,8},{14,9},{15,9},{16,10},{16,11},{17,12},{17,14},{14,17},{10,19},{5,19},{3,18},{0,15},{0,16},{1,18},{3,20},{5,21},
		{13,21},{15,22},{16,22},{17,22},{16,22},{16,23},{17,25},{19,27},{19,28},{18,29},{18,34},{19,37},{21,39},{22,39},{23,37},
		{23,35},{24,34},{24,33},{25,34},{26,35},{26,37},{26,35},{25,34},{24,33},{24,32},{23,31},{23,30},{25,32},{24,33},{25,32},
		{26,32},{27,33},{29,33},{31,34},{35,34},{32,33.5},{31,33},{29,32},{27,32},{25,30},{24,30},{25,31},{33,31},{35,30},{40,25},
		{41,23},{42,21},{44,21},{43,20},{45,24},{49,29},{45,24},{43,20},{44,21},{46,19},{45,19},{49,23},{57,27},{49,23},{45,19},
		{46,19},{46,14},{43,8.5},{41,6},{41,7},{42,8},{42,10},{43,11},{43,14},{44,15},{44,18},{43,19},{41,19},{41,20},{39,24},
		{37,26},{36,26},{36,24},{37,22},{38,21},{39,21},{40,20},{40,19},{38,21},{40,19},{40,18},{41,18},{42,17},{42,16},{41,15},
		{39,15},{38,16},{39,17},{39,16},{40,16},{41,17},{40,18},{39,17},{40,18},{39,18},{38,17},{37,17},{36,16},{36,19},{34,23},
		{32,25},{31,25},{30,24},{30,22},{31,20},{33,18},{33,19},{34,19},{35,18},{35,17},{36,16},{35,15},{33,17},{33,18},{33,17},
		{35,15},{32,15},{31,16},{33,14},{36,14},{30,13},{21,9},{30,13},{36,14},{33,14},{33,13},{34,12},{35,12},{36,11},{37,12},
		{31,8},{24,0},{31,8},{37,12},{36,11},{39,11},{40,12},{41,13},{42,13},{43,14},{42,13},{41,13},{40,12},{39,11},{36,11},
		{35,12},{34,12},{33,13},{33,11},{33,10},{35,8},{36,9},{39,9},{40,8},{40,6},{39,5},{38,5},{35,8},{33,10},{33,11},{31,9},
		{29,9},{27,9},{29,9},{29,8},{33,4},{34,4},{35,3},{37,3},{38,4},{37,4},{36,5},{37,4},{38,4},{40,4},{41,5},{41,6},{41,5},
		{40,4},{38,4},{37,3},{35,3},{34,3},{33,2},{25,2},{24,3},{23,3},{21,4},{17,8},{16,10},{17,8},{21,4},{23,3},{21,1},{18,0},
		{10,0},{6,2},{3,5},{0,11},{0,15}
};

struct Vec2i16 interpolation(struct Vec2i16* c1, struct Vec2i16* c2, int8_t step)
{
	struct Vec2i16 res;
	res.x = (c1->x + (c2->x - c1->x) * step / STEP_COUNT) * PWM_MAX / MAX_COORD_VALUE;	// Transform to PWM duty cycle
	res.y = (c1->y + (c2->y - c1->y) * step / STEP_COUNT) * PWM_MAX / MAX_COORD_VALUE;
	return res;
}

void drawFig() {
	static int16_t step = 0;
	uint16_t points_count = sizeof(rawFig) / sizeof(struct Vec2i16);

	uint16_t curent_i = step / STEP_COUNT;
	uint16_t next_i = ((curent_i + 1) < points_count) ? (curent_i+1) : 0;

	// Have some interpolation between to points
	struct Vec2i16 pwmPower = interpolation(&rawFig[curent_i], &rawFig[next_i], step - curent_i*STEP_COUNT);

	// Set PWM
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwmPower.x);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pwmPower.y);

	step++;
	// Reset step if it end of figure
	if(step / STEP_COUNT == points_count)
		step = 0;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

  while (1)
  {
	drawFig();
	HAL_Delay(3);
    /* USER CODE END WHILE */

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  { }
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
