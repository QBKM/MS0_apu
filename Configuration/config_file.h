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
 * 	SCHEDULER
 **************************************************************** */
/* Synchro */
#define SYNC_DELAY  		100				/* [ms] time the scheduler wait to get synchronized */


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

/* TCA6408A */
#define TCA6408A_ADDR       (0x20 << 1)     /* TCA6408A address is 0x21 */


/**************************************************************** *
 * 	HMI
 **************************************************************** */
/* OLED lines position */
#define HMI_OLED_LINE_NEXT			(uint8_t)10
#define HMI_OLED_LINE_1				(uint8_t)10
#define HMI_OLED_LINE_2				(uint8_t)20
#define HMI_OLED_LINE_3				(uint8_t)30
#define HMI_OLED_LINE_4				(uint8_t)40
#define HMI_OLED_LINE_5				(uint8_t)50

/* OLED columns position */
#define HMI_OLED_INIT_LOG_COLUMN	(uint8_t)(12*7)
#define HMI_OLED_DATA_LOG_COLUMN	(uint8_t)(7*7)
#define HMI_OLED_STATUS_COLUMN		(uint8_t)(7*9)

/* OLED columns for time display in data line */
#define HMI_OLED_DATA_LOG_COLUMN_HOUR	(uint8_t)(7*7)
#define HMI_OLED_DATA_LOG_COLUMN_DOT1	(uint8_t)(7*9)
#define HMI_OLED_DATA_LOG_COLUMN_MIN	(uint8_t)(7*10)
#define HMI_OLED_DATA_LOG_COLUMN_DOT2	(uint8_t)(7*12)
#define HMI_OLED_DATA_LOG_COLUMN_SEC	(uint8_t)(7*13)

/* OLED menu line */
#define HMI_OLED_MENU_LINE_HEADER       HMI_OLED_LINE_1
#define HMI_OLED_MENU_LINE_DATA         HMI_OLED_LINE_2
#define HMI_OLED_MENU_LINE_STATUS       HMI_OLED_LINE_3
#define HMI_OLED_MENU_LINE_QUIT         HMI_OLED_LINE_4
#define HMI_OLED_MENU_LINE_RESTART      HMI_OLED_LINE_5

/* OLED init line */
#define HMI_OLED_INIT_LINE_DS3231       HMI_OLED_LINE_1
#define HMI_OLED_INIT_LINE_BMP280       HMI_OLED_LINE_2
#define HMI_OLED_INIT_LINE_MPU6050      HMI_OLED_LINE_3
#define HMI_OLED_INIT_LINE_DS18B20      HMI_OLED_LINE_4

/* OLED data line */
#define HMI_OLED_DATA_LINE_HEADER       HMI_OLED_LINE_1
#define HMI_OLED_DATA_LINE_TIME         HMI_OLED_LINE_2
#define HMI_OLED_DATA_LINE_PRESS        HMI_OLED_LINE_3
#define HMI_OLED_DATA_LINE_ANGLEX       HMI_OLED_LINE_4
#define HMI_OLED_DATA_LINE_ANGLEY       HMI_OLED_LINE_5

/* OLED status line */
#define HMI_OLED_STATUS_LINE_HEADER     HMI_OLED_LINE_1
#define HMI_OLED_STATUS_LINE_PHASE      HMI_OLED_LINE_2
#define HMI_OLED_STATUS_LINE_JACK       HMI_OLED_LINE_3
#define HMI_OLED_STATUS_LINE_ERRORS     HMI_OLED_LINE_5


#endif
