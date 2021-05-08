/* USER CODE BEGIN Header */
/** ************************************************************* *
 * @file        main.c
 * @brief       
 * 
 * @date        2021-04-26
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ds3231.h"
#include "mpu6050.h"
#include "bmp280.h"
#include "ssd1306.h"
#include "tca6408a.h"
#include "bitmap.h"
#include "hmi.h"

#include "ds18b20.h"
#include "delay.h"

#include "broadcast.h"

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
HMI_FrameSelector_t Frame;

DS3231_t TIME;
BMP280_t PRESS;
MPU6050_t ANGLE;
DS18B20_t TEMP;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void routine_no_frame(void);
void routine_menu_frame(void);
void routine_data_log_frame(void);

void routine_DS3231(void);
void routine_BMP280(void);
void routine_MPU6050(void);
void routine_DS18B20(void);
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
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

	HW_status_t HW_init =
	{
	.DS3231 	= DS3231_Init(),
	.MPU6050 	= MPU6050_Init(),
	.BMP280 	= BMP280_Init(),
	.SSD1306 	= SSD1306_Init(),
	/* .DS18B20	= DS18B20_Init()*/
	.TCA6408A	= TCA6408A_Init()
	};

	/* check the errors ... TO DO */
	ERR_MNGR_HW_init(HW_init);

	/* delay_init */
	delay_init();

	/* init the OLED */
	HMI_OLED_init();
	/* display MS0 logo a start */
	HMI_OLED_display_bitmap(logo_ms0, 1000, ClearAfter);
	/* display the HW init log */
	HMI_OLED_display_init_log(HW_init, 2000, ClearAfter);

	/* display a static message for */
	HMI_OLED_display_running();

	uint8_t pData[1];

	HAL_UART_Receive_IT(&huart2, &pData, 1);

	broadcast_uart_send(0xA1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {	 
	/* check if the user open the menu */
	HMI_OLED_check_menu();

	switch (Frame)
	{
	case NO_FRAME		: routine_no_frame(); 		break;
	case MENU_FRAME		: routine_menu_frame(); 	break;
	case DATA_LOG_FRAME	: routine_data_log_frame(); break;
	default: break;
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/** ************************************************************* *
 * @brief       
 * 
 * @param       GPIO_Pin 
 * ************************************************************* **/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_2) HMI_OLED_IT_btn_up();
	if(GPIO_Pin == GPIO_PIN_3) HMI_OLED_IT_btn_middle();
	if(GPIO_Pin == GPIO_PIN_4) HMI_OLED_IT_btn_bottom();
}

/** ************************************************************* *
 * @brief       
 * 
 * @param       huart 
 * ************************************************************* **/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2) broadcast_uart_receive();
}


/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void routine_no_frame(void)
{
	routine_DS3231();
	routine_BMP280();
	routine_MPU6050();
	//routine_DS18B20();
}


/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void routine_menu_frame(void)
{
	routine_DS3231();
	routine_BMP280();
	routine_MPU6050();
	//routine_DS18B20();

	HMI_OLED_display_menu_selector();
	SSD1306_UpdateScreen();
}


/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void routine_data_log_frame(void)
{
	routine_DS3231();
	routine_BMP280();
	routine_MPU6050();
	//routine_DS18B20();

	if(HW_status.DS3231 == HAL_OK)
	{
		HMI_OLED_display_data_log_time(TIME, HMI_OLED_LINE_2);
	}
	else
	{
		HMI_OLED_display_data_log_failed(HMI_OLED_LINE_2);
	}

	if(HW_status.BMP280 == HAL_OK)
	{
		HMI_OLED_display_data_log_press(PRESS, HMI_OLED_LINE_3);
	}
	else
	{
		HMI_OLED_display_data_log_failed(HMI_OLED_LINE_3);
	}

	if(HW_status.MPU6050 == HAL_OK)
	{
		HMI_OLED_display_data_log_angle(ANGLE, HMI_OLED_LINE_4, HMI_OLED_LINE_5);
	}
	else
	{
		HMI_OLED_display_data_log_failed(HMI_OLED_LINE_4);
		HMI_OLED_display_data_log_failed(HMI_OLED_LINE_5);
	}

	SSD1306_UpdateScreen();
}

/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void routine_DS3231(void)
{
	if(DS3231_Read_All() == HAL_OK)
	{
		HW_status.DS3231 = HAL_OK;
	 	TIME = DS3231_Get_Struct();
	}
	else
	{
		HW_status.DS3231 = HAL_ERROR;
	}
}

/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void routine_BMP280(void)
{
	if(BMP280_Read_All() == HAL_OK)
	{
		if(HW_status.BMP280 == HAL_ERROR) BMP280_Init();
		HW_status.BMP280 = HAL_OK;
		PRESS = BMP280_Get_Struct();
	}
	else
	{
		HW_status.BMP280 = HAL_ERROR;
	}
}


/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void routine_MPU6050(void)
{
	if(MPU6050_Read_All_Kalman() == HAL_OK)
	{
		if(HW_status.MPU6050 == HAL_ERROR) MPU6050_Init();
		HW_status.MPU6050 = HAL_OK;
		ANGLE = MPU6050_Get_Struct();
	}
	else
	{
		HW_status.MPU6050 = HAL_ERROR;
	}
}

/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void routine_DS18B20(void)
{
	if(DS18B20_Get_Temp() == HAL_OK)
	{
		if(HW_status.DS18B20 == HAL_ERROR) DS18B20_Init();
		HW_status.DS18B20 = HAL_OK;
		TEMP = DS18B20_Get_Struct();
	}
	else
	{
		HW_status.DS18B20 = HAL_ERROR;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
