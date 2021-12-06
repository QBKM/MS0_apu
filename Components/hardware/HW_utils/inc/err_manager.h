/** ************************************************************* *
 * @file        err_manager.h
 * @brief       
 * 
 * @date        2021-04-27
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __ERR_MNGR__
#define __ERR_MNGR__

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "stm32f3xx_hal.h"


/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef struct HW_status_t
{
	HAL_StatusTypeDef DS3231;
	HAL_StatusTypeDef MPU6050;
	HAL_StatusTypeDef BMP280;
	HAL_StatusTypeDef SSD1306;
	HAL_StatusTypeDef DS18B20;
	HAL_StatusTypeDef TCA6408A;
	HAL_StatusTypeDef SDCARD;
}HW_status_t;


/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
uint16_t err_counter;

/* ------------------------------------------------------------- --
   function prototypes
-- ------------------------------------------------------------- */

#endif
