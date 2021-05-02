/* USER CODE BEGIN Header */
/** ************************************************************* *
 * @file        main.h
 * @brief       
 * 
 * @date        2021-04-26
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OneWire_Pin GPIO_PIN_0
#define OneWire_GPIO_Port GPIOF
#define RST_DS3231_Pin GPIO_PIN_1
#define RST_DS3231_GPIO_Port GPIOF
#define BTN_1_Pin GPIO_PIN_2
#define BTN_1_GPIO_Port GPIOA
#define BTN_1_EXTI_IRQn EXTI2_TSC_IRQn
#define BTN_2_Pin GPIO_PIN_3
#define BTN_2_GPIO_Port GPIOA
#define BTN_2_EXTI_IRQn EXTI3_IRQn
#define BTN_3_Pin GPIO_PIN_4
#define BTN_3_GPIO_Port GPIOA
#define BTN_3_EXTI_IRQn EXTI4_IRQn
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
