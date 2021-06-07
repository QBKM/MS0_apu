/** ************************************************************* *
 * @file        datalink.c
 * @brief       
 * 
 * @date        2021-05-16
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "datalink.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "config_file.h"

#include "database.h"


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
#ifndef TIMEOUT_UART
#define TIMEOUT_UART 10
#endif

/* uart peripheral */
#define HUART  huart1

/* ------------------------------------------------------------- --
   Public functions
-- ------------------------------------------------------------- */
/** ************************************************************* *
 * @brief      send on uart a short ID for other boards
 * 
 * @param      message 
 * ************************************************************* **/
void datalink_uart_send(void)
{
	DATABASE_t database = 
	{
		.BMP280 	= BMP280_Get_Struct(),
		.DS18B20 	= DS18B20_Get_Struct(),
		.DS3231 	= DS3231_Get_Struct(),
		.MPU6050 	= MPU6050_Get_Struct()
	};

	char buffer[sizeof(database)];
	memcpy(buffer, &database, sizeof(database));

	HAL_UART_Transmit(&HUART, (uint8_t*)"[", sizeof("["), TIMEOUT_UART);			
	HAL_UART_Transmit(&HUART, (uint8_t *)buffer, sizeof(buffer), TIMEOUT_UART);
	HAL_UART_Transmit(&HUART, (uint8_t*)"]", sizeof("]"), TIMEOUT_UART);
}
