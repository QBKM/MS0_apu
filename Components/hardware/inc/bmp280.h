/** ************************************************************* *
 * @file        bmp280.h
 * @brief       
 * 
 * @date        2021-04-24
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __BMP280_H__
#define __BMP280_H__


/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
/* BMP6050 mode settings */
typedef enum {
    BMP280_MODE_SLEEP   = 0,	/* Sleep  - Stop measurement */
    BMP280_MODE_FORCED  = 1,	/* Forced - Measurement is initiated by user */
    BMP280_MODE_NORMAL  = 3		/* Normal - Continues measurement */
} BMP280_Mode;

/* filter settings */
typedef enum {
    BMP280_FILTER_OFF 	= 0,
    BMP280_FILTER_2 	= 1,
    BMP280_FILTER_4 	= 2,
    BMP280_FILTER_8 	= 3,
    BMP280_FILTER_16 	= 4
} BMP280_Filter;

/* Pressure oversampling settings */
typedef enum {
    BMP280_SKIPPED 			= 0,   /* no measurement  */
    BMP280_ULTRA_LOW_POWER 	= 1,   /* oversampling x1 */
    BMP280_LOW_POWER 		= 2,   /* oversampling x2 */
    BMP280_STANDARD 		= 3,   /* oversampling x4 */
    BMP280_HIGH_RES 		= 4,   /* oversampling x8 */
    BMP280_ULTRA_HIGH_RES 	= 5    /* oversampling x16 */
} BMP280_Oversampling;

/* Stand by time between measurements in normal mode */
typedef enum {
    BMP280_STANDBY_05 	= 0,    /* stand by time 0.5ms */
    BMP280_STANDBY_62 	= 1,    /* stand by time 62.5ms */
    BMP280_STANDBY_125 	= 2,    /* stand by time 125ms */
    BMP280_STANDBY_250 	= 3,    /* stand by time 250ms */
    BMP280_STANDBY_500 	= 4,    /* stand by time 500ms */
    BMP280_STANDBY_1000	= 5,    /* stand by time 1s */
    BMP280_STANDBY_2000 = 6,    /* stand by time 2s BMP280, 10ms BME280 */
    BMP280_STANDBY_4000 = 7,    /* stand by time 4s BMP280, 20ms BME280 */
} BMP280_StandbyTime;



/* Configuration parameters for BMP280 module */
typedef struct {
    BMP280_Mode mode;
    BMP280_Filter filter;
    BMP280_Oversampling oversampling_pressure;
    BMP280_Oversampling oversampling_temperature;
    BMP280_StandbyTime standby;
} BMP280_config_t;

/* calibration structure */
typedef struct
{
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
}BMP280_calibration_t;

/* data structure */
typedef struct BMP280_t{
	float temperature;
	float pressure;

	BMP280_config_t config;
	BMP280_calibration_t calib;
}BMP280_t;


/* ------------------------------------------------------------- --
   fonctions
-- ------------------------------------------------------------- */
uint8_t BMP280_Init(void);
uint8_t BMP280_Read_All(void);
BMP280_t BMP280_Get_Struct(void);

#endif  // __BMP280_H__
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
