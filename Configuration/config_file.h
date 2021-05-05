/** ************************************************************* *
 * @file       config_file.h
 * @brief      
 * 
 * @date       2021-04-22
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __CONFIG_FILE_H__
#define __CONFIG_FILE_H__

/* 	+-------------------+ */
/*	| LIST OF CONSTANTS | */
/*	+-------------------+ */

/**************************************************************** *
 * 	PERIPHERALS
 **************************************************************** */
/* UART */
#define TIMEOUT_UART    	10				/* [ms] timeout value */

/* I2C */
#define TIMEOUT_I2C     	10				/* [ms] timeout value */


/**************************************************************** *
 * 	HARDWARE
 **************************************************************** */
/* MPU6050 */
#define MPU6050_ADDR 		(0x69 << 1) 	/* MPU6050 address is 0x69 if SDO pin is high, and is 0x68 if low */

/* DS3231 */
#define DS3231_ADDR     	(0x68 << 1)		/* DS3231 address is 0x68 */

/* BMP280 */
#define BMP280_ADDR			0x76			/* BMP280 address is 0x77 if SDO pin is high, and is 0x76 if low */
#define BMP280_CHIP_ID		0x58			/* BMP280 has chip-id 0x58 */

/* SSD1306 */
#define SSD1306_ADDR		0x78  			/* SSD1306 address is 0x78 */   
#define SSD1306_INT16_SIZE	9				/* SSD1306 max numbers of charater to display for 16bits number */
#define SSD1306_FLOAT_SIZE	9				/* SSD1306 max numbers of charater to display for float number */

/* TCA6408A */
#define TCA6408A_ADDR       (0x20 << 1)     /* TCA6408A address is 0x21 */


#endif
