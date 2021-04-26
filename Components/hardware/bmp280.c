/** ************************************************************* *
 * @file        bmp280.c
 * @brief       
 * 
 * @date        2021-04-24
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* ------------------------------------------------------------- --
   Includes
-- ------------------------------------------------------------- */
#include "bmp280.h"
#include "config_file.h"
#include "i2c.h"


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
/* BMP280 registers */
#define BMP280_REG_TEMP_XLSB   0xFC /* bits: 7-4 */
#define BMP280_REG_TEMP_LSB    0xFB
#define BMP280_REG_TEMP_MSB    0xFA
#define BMP280_REG_TEMP        (BMP280_REG_TEMP_MSB)
#define BMP280_REG_PRESS_XLSB  0xF9 /* bits: 7-4 */
#define BMP280_REG_PRESS_LSB   0xF8
#define BMP280_REG_PRESS_MSB   0xF7
#define BMP280_REG_PRESSURE    (BMP280_REG_PRESS_MSB)
#define BMP280_REG_CONFIG      0xF5 /* bits: 7-5 t_sb; 4-2 filter; 0 spi3w_en */
#define BMP280_REG_CTRL        0xF4 /* bits: 7-5 osrs_t; 4-2 osrs_p; 1-0 mode */
#define BMP280_REG_STATUS      0xF3 /* bits: 3 measuring; 0 im_update */
#define BMP280_REG_RESET       0xE0
#define BMP280_REG_ID          0xD0
#define BMP280_REG_CALIB       0x88
#define BMP280_RESET_VALUE     0xB6

#ifndef TIMEOUT_I2C
#define TIMEOUT_I2C 10
#endif


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
typedef struct {
	float temperature;
	float pressure;

	BMP280_config_t config;
	BMP280_calibration_t calib;
}BMP280_t;


/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
BMP280_t BMP280;


/* ------------------------------------------------------------- --
   private prototypes
-- ------------------------------------------------------------- */
static uint8_t bmp280_read_fixed(int32_t *temperature, uint32_t *pressure);
static uint8_t bmp280_read_register16(uint8_t addr, uint16_t *value);
static uint8_t bmp280_read_calibration_data(void);

static int32_t bmp280_compensate_temperature(int32_t adc_temp, int32_t *fine_temp);
static uint32_t bmp280_compensate_pressure(int32_t adc_press, int32_t fine_temp);


/* ============================================================= ==
   private functions
== ============================================================= */
/** ************************************************************* *
 * @brief       read the data fixed with the calibrated data
 * 
 * @param       temperature 
 * @param       pressure 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t bmp280_read_fixed(int32_t *temperature, uint32_t *pressure)
{
	int32_t adc_pressure;
	int32_t adc_temp;
	int32_t fine_temp;
	uint8_t data[6];

	if(HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR<<1, BMP280_REG_PRESS_MSB, 1, data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

	adc_pressure = data[0] << 12 | data[1] << 4 | data[2] >> 4;
	adc_temp = data[3] << 12 | data[4] << 4 | data[5] >> 4;

	*temperature = bmp280_compensate_temperature( adc_temp, &fine_temp);
	*pressure = bmp280_compensate_pressure( adc_pressure, fine_temp);

	return HAL_OK;
}

/** ************************************************************* *
 * @brief       calculate the right temperature with the calibration
 * 				data.
 * 
 * @param       adc_temp 
 * @param       fine_temp 
 * @return      int32_t 
 * ************************************************************* **/
static int32_t bmp280_compensate_temperature(int32_t adc_temp, int32_t *fine_temp)
{
	int32_t var1, var2;

	var1 = ((((adc_temp >> 3) - ((int32_t) BMP280.calib.dig_T1 << 1))) * (int32_t) BMP280.calib.dig_T2) >> 11;
	var2 = (((((adc_temp >> 4) - (int32_t) BMP280.calib.dig_T1) * ((adc_temp >> 4) - (int32_t) BMP280.calib.dig_T1)) >> 12) * (int32_t) BMP280.calib.dig_T3) >> 14;

	*fine_temp = var1 + var2;
	return (*fine_temp * 5 + 128) >> 8;
}

/** ************************************************************* *
 * @brief       calculate the right pressure with the calibration
 * 				data.
 * 
 * @param       adc_press 
 * @param       fine_temp 
 * @return      uint32_t 
 * ************************************************************* **/
static uint32_t bmp280_compensate_pressure(int32_t adc_press, int32_t fine_temp)
{
	int64_t var1, var2, p;

	var1 = (int64_t) fine_temp - 128000;
	var2 = var1 * var1 * (int64_t)  BMP280.calib.dig_P6;
	var2 = var2 + ((var1 * (int64_t)  BMP280.calib.dig_P5) << 17);
	var2 = var2 + (((int64_t)  BMP280.calib.dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t)  BMP280.calib.dig_P3) >> 8) + ((var1 * (int64_t)  BMP280.calib.dig_P2) << 12);
	var1 = (((int64_t) 1 << 47) + var1) * ((int64_t)  BMP280.calib.dig_P1) >> 33;

	if (var1 == 0) {
		return 0;  // avoid exception caused by division by zero
	}

	p = 1048576 - adc_press;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = ((int64_t)  BMP280.calib.dig_P9 * (p >> 13) * (p >> 13)) >> 25;
	var2 = ((int64_t)  BMP280.calib.dig_P8 * p) >> 19;

	p = ((p + var1 + var2) >> 8) + ((int64_t)  BMP280.calib.dig_P7 << 4);
	return p;
}

/** ************************************************************* *
 * @brief       read the calibration registers of the bmp280
 * 
 * @return      uint8_t 
 * ************************************************************* **/
static uint8_t bmp280_read_calibration_data(void) {

	if(bmp280_read_register16( 0x88, 			  &BMP280.calib.dig_T1)
	&& bmp280_read_register16( 0x8a, (uint16_t *) &BMP280.calib.dig_T2)
	&& bmp280_read_register16( 0x8c, (uint16_t *) &BMP280.calib.dig_T3)
	&& bmp280_read_register16( 0x8e, 			  &BMP280.calib.dig_P1)
	&& bmp280_read_register16( 0x90, (uint16_t *) &BMP280.calib.dig_P2)
	&& bmp280_read_register16( 0x92, (uint16_t *) &BMP280.calib.dig_P3)
	&& bmp280_read_register16( 0x94, (uint16_t *) &BMP280.calib.dig_P4)
	&& bmp280_read_register16( 0x96, (uint16_t *) &BMP280.calib.dig_P5)
	&& bmp280_read_register16( 0x98, (uint16_t *) &BMP280.calib.dig_P6)
	&& bmp280_read_register16( 0x9a, (uint16_t *) &BMP280.calib.dig_P7)
	&& bmp280_read_register16( 0x9c, (uint16_t *) &BMP280.calib.dig_P8)
	&& bmp280_read_register16( 0x9e, (uint16_t *) &BMP280.calib.dig_P9))
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

/** ************************************************************* *
 * @brief       Function used to read 2 bytes of data and return
 * 				it.
 * 
 * @param       addr 
 * @param       value 
 * @return      uint8_t 
 * ************************************************************* **/
static uint8_t bmp280_read_register16(uint8_t addr, uint16_t *value)
{
	uint8_t rx_buff[2];

	if (HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR<<1, addr, 1, rx_buff, 2, 5000) == HAL_OK)
	{
		*value = (uint16_t) ((rx_buff[1] << 8) | rx_buff[0]);
		return true;
	}
	else
	{
		return false;
	}
}

/* ============================================================= ==
   public functions
== ============================================================= */
/** ************************************************************* *
 * @brief       init the BMP280
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t BMP280_Init(void)
{
    uint8_t check;
    uint8_t status;
    uint8_t data;

    /* initialize the temperature and the pressure to 0 */
    BMP280.pressure 	= 0.0;
    BMP280.temperature 	= 0.0;

    /* Structure to configure the BMP280 */
    BMP280_config_t config =
	{
	.mode = 					BMP280_MODE_NORMAL,
	.filter = 					BMP280_FILTER_OFF,
	.oversampling_pressure = 	BMP280_STANDARD,
	.oversampling_temperature = BMP280_STANDARD,
	.standby = 					BMP280_STANDBY_250
	};
	BMP280.config = config;

	/* check device ID WHO_AM_I */
	if(HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR<<1, BMP280_REG_ID, 1, &check, 1, TIMEOUT_I2C)) return HAL_ERROR;
	if(check != BMP280_CHIP_ID) return false;

	/* reset the chip */
	if(HAL_I2C_Mem_Write(&hi2c1, BMP280_ADDR<<1, BMP280_REG_RESET, 1, (uint8_t*)BMP280_RESET_VALUE, 1, TIMEOUT_I2C)) return HAL_ERROR;

	/* Wait until finished copying over the NVP data */
	while (1)
	{
		if(!HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR<<1, BMP280_REG_STATUS, 1, &status, 1, TIMEOUT_I2C) && (status & 1) == 0) break;
	}

	/* read calibration data */
	bmp280_read_calibration_data();

	/* Config register */
	data = (BMP280.config.standby << 5 | BMP280.config.filter << 2);
	if(HAL_I2C_Mem_Write(&hi2c1, BMP280_ADDR<<1, BMP280_REG_CONFIG, 1, &data, 1, TIMEOUT_I2C)) return HAL_ERROR;

	/* Control register */
	data = (BMP280.config.oversampling_temperature << 5 | BMP280.config.oversampling_pressure << 2 | BMP280.config.mode);
	if(HAL_I2C_Mem_Write(&hi2c1, BMP280_ADDR<<1, BMP280_REG_CTRL, 1, &data, 1, TIMEOUT_I2C)) return HAL_ERROR;

	return HAL_OK;
}

/** ************************************************************* *
 * @brief       read the temperature and pressure data
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t BMP280_Read_All(void)
{
	int32_t fixed_temperature;
	uint32_t fixed_pressure;

	if(!bmp280_read_fixed( &fixed_temperature, &fixed_pressure))
	{
		BMP280.temperature = (float) fixed_temperature / 100;
		BMP280.pressure = (float) fixed_pressure / 256;
		return HAL_OK;
	}
	return HAL_ERROR;
}


/* ------------------------------------------------------------- --
   End of file
-- ------------------------------------------------------------- */
