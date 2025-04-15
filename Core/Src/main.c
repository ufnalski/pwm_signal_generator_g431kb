/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define DRONE_ESC_PRESETS
#define SG90_SERVO_PRESETS
//#define THROTTLE_BODY_PRESETS

#define OLED_REFRESH_RATE 200

// https://github.com/bitdump/BLHeli/blob/master/BLHeli_32%20ARM/BLHeli_32%20manual%20ARM%20Rev32.x.pdf
#ifdef DRONE_ESC_PRESETS
// Left button (blue)
#define FREQUENCY_PRESET_1 50
#define DUTY_CYCLE_PRESET_1 0
// Right button (white)
#define FREQUENCY_PRESET_2 50
#define DUTY_CYCLE_PRESET_2 10  // 2 ms (max throttle)
// Left encoder knob
#define FREQUENCY_PRESET_3 50
#define DUTY_CYCLE_PRESET_3 5  // 1 ms (min throttle)
// Right encoder knob
#define FREQUENCY_PRESET_4 50
#define DUTY_CYCLE_PRESET_4 8  // more than 50% throttle
// Initial frequency
#define INIT_FREQ 50
#define PRESETS_NAME "  Drone ESC presets"
#endif

#ifdef SG90_SERVO_PRESETS
// Left button (blue)
#define FREQUENCY_PRESET_1 50
#define DUTY_CYCLE_PRESET_1 0
// Right button (white)
#define FREQUENCY_PRESET_2 50
#define DUTY_CYCLE_PRESET_2 10
// Left encoder knob
#define FREQUENCY_PRESET_3 50
#define DUTY_CYCLE_PRESET_3 3 // 0.6 ms (close to 0.5 ms [min value])
// Right encoder knob
#define FREQUENCY_PRESET_4 50
#define DUTY_CYCLE_PRESET_4 12 // 2.4 ms (close to 2.5 ms [max value])
// Initial frequency
#define INIT_FREQ 50
#define PRESETS_NAME "  SG90 servo presets"
#endif

#ifdef THROTTLE_BODY_PRESETS
// Left button (blue)
#define FREQUENCY_PRESET_1 2000
#define DUTY_CYCLE_PRESET_1 0
// Right button (white)
#define FREQUENCY_PRESET_2 4000
#define DUTY_CYCLE_PRESET_2 0
// Left encoder knob
#define FREQUENCY_PRESET_3 2000
#define DUTY_CYCLE_PRESET_3 5
// Right encoder knob
#define FREQUENCY_PRESET_4 2000
#define DUTY_CYCLE_PRESET_4 40
// Initial frequency
#define INIT_FREQ 2000
#define PRESETS_NAME "Throttle body presets"
#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

COM_InitTypeDef BspCOMInit;

/* USER CODE BEGIN PV */
uint16_t left_enc_count;
uint16_t right_enc_count;

char lcd_line[32];
uint32_t OledSoftTimer;

int32_t duty_cycle = 0;
int32_t duty_cycle_x2 = 0;
int32_t pwm_freq = INIT_FREQ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void UpdateARRandCCR(int32_t *_pwm_freq, int32_t *_duty_cycle);
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
	MX_SPI1_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_TIM2_Init();
	/* USER CODE BEGIN 2 */
	ssd1306_Init();
	ssd1306_Fill(Black);
	ssd1306_SetCursor(4, 10);
	ssd1306_WriteString("PWM signal generator", Font_6x8, White);
	ssd1306_SetCursor(0, 24);
	ssd1306_WriteString(PRESETS_NAME, Font_6x8, White);
	ssd1306_UpdateScreen();

	HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL); // left encoder knob
	HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_ALL); // right encoder knob

	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	/* USER CODE END 2 */

	/* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
	BspCOMInit.BaudRate = 115200;
	BspCOMInit.WordLength = COM_WORDLENGTH_8B;
	BspCOMInit.StopBits = COM_STOPBITS_1;
	BspCOMInit.Parity = COM_PARITY_NONE;
	BspCOMInit.HwFlowCtl = COM_HWCONTROL_NONE;
	if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
	{
		Error_Handler();
	}

	/* USER CODE BEGIN BSP */

	/* -- Sample board code to send message over COM1 port ---- */
	printf("Welcome to PWM world!\n\r");

	/* USER CODE END BSP */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	OledSoftTimer = HAL_GetTick();
	while (1)
	{

		if (HAL_GetTick() - OledSoftTimer > OLED_REFRESH_RATE)
		{
			OledSoftTimer = HAL_GetTick();
			sprintf(lcd_line, "Frequency:  %li Hz    ", pwm_freq);
			ssd1306_SetCursor(5, 42);
			ssd1306_WriteString(lcd_line, Font_6x8, White);
			sprintf(lcd_line, "Duty cycle: %li %%    ", duty_cycle);
			ssd1306_SetCursor(5, 54);
			ssd1306_WriteString(lcd_line, Font_6x8, White);
			ssd1306_UpdateScreen();
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
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct =
	{ 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct =
	{ 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if (htim->Instance == TIM3)
	{
//		left_enc_count = __HAL_TIM_GET_COUNTER(&htim3);

		if (((TIM3->CR1) & (0x00000001 << 4)) == 0)  // DIR bit
		{
			if (pwm_freq >= 1000)
			{
				pwm_freq += 500;
			}
			else if (pwm_freq >= 100)
			{
				pwm_freq += 50;
			}
			else
			{
				pwm_freq += 5;
			}
		}
		else
		{
			if (pwm_freq >= 2000)
			{
				pwm_freq -= 500;
			}
			else if (pwm_freq >= 200)
			{
				pwm_freq -= 50;
			}
			else
			{
				pwm_freq -= 5;
			}
		}

		if (pwm_freq < 10)
		{
			pwm_freq = 10;
		}
		else if (pwm_freq > 10000)
		{
			pwm_freq = 10000;
		}

		duty_cycle = duty_cycle_x2 / 2;
		UpdateARRandCCR(&pwm_freq, &duty_cycle);
	}

	if (htim->Instance == TIM4)
	{
//		right_enc_count = __HAL_TIM_GET_COUNTER(&htim4);

		if (((TIM4->CR1) & (0x00000001 << 4)) == 0)  // DIR bit
		{
			duty_cycle_x2 += 1;
		}
		else
		{
			duty_cycle_x2 -= 1;
		}
		if (duty_cycle_x2 < 0)
		{
			duty_cycle_x2 = 0;
		}
		else if (duty_cycle_x2 > 200)
		{
			duty_cycle_x2 = 200;
		}
		duty_cycle = duty_cycle_x2 / 2;
		UpdateARRandCCR(&pwm_freq, &duty_cycle);
	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) // Presets
{
	if (GPIO_Pin == BLUE_BUTTON_Pin)
	{
		pwm_freq = FREQUENCY_PRESET_1;
		duty_cycle = DUTY_CYCLE_PRESET_1;
		duty_cycle_x2 = duty_cycle * 2;
		UpdateARRandCCR(&pwm_freq, &duty_cycle);
	}

	if (GPIO_Pin == WHITE_BUTTON_Pin)
	{
		pwm_freq = FREQUENCY_PRESET_2;
		duty_cycle = DUTY_CYCLE_PRESET_2;
		duty_cycle_x2 = duty_cycle * 2;
		UpdateARRandCCR(&pwm_freq, &duty_cycle);
	}

	if (GPIO_Pin == LEFT_ENCODER_KNOB_Pin)
	{
		pwm_freq = FREQUENCY_PRESET_3;
		duty_cycle = DUTY_CYCLE_PRESET_3;
		duty_cycle_x2 = duty_cycle * 2;
		UpdateARRandCCR(&pwm_freq, &duty_cycle);
	}

	if (GPIO_Pin == RIGHT_ENCODER_KNOB_Pin)
	{
		pwm_freq = FREQUENCY_PRESET_4;
		duty_cycle = DUTY_CYCLE_PRESET_4;
		duty_cycle_x2 = duty_cycle * 2;
		UpdateARRandCCR(&pwm_freq, &duty_cycle);
	}
}

void UpdateARRandCCR(int32_t *_pwm_freq, int32_t *_duty_cycle)
{
	__HAL_TIM_SET_AUTORELOAD(&htim2, 10000000 / (*_pwm_freq));
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,
			100000 * (*_duty_cycle) / (*_pwm_freq));
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/**
 * @}
 */

/**
 * @}
 */

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
