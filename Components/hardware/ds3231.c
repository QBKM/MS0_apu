/** ************************************************************* *
 * @file       ds3231.c
 * @brief      
 * 
 * @date       2021-04-22
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/


/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "ds3231.h"
#include "config_file.h"
#include "i2c.h"


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
/* I2C timeout */
#ifndef TIMEOUT_I2C
#define TIMEOUT_I2C 10
#endif

/* DS3231 address */
#ifndef DS3231_ADDR
#define DS3231_ADDR  (0x68 << 1)
#endif

/* DS3231 registers */
#define DS3231_REG_TIME         0x00
#define DS3231_REG_ALARM1       0x07
#define DS3231_REG_ALARM2       0x0B
#define DS3231_REG_CONTROL      0x0E
#define DS3231_REG_STATUS       0x0F
#define DS3231_REG_TEMP         0x11

#define DS3231_CON_EOSC         0x80
#define DS3231_CON_BBSQW        0x40
#define DS3231_CON_CONV         0x20
#define DS3231_CON_RS2          0x10
#define DS3231_CON_RS1          0x08
#define DS3231_CON_INTCN        0x04
#define DS3231_CON_A2IE         0x02
#define DS3231_CON_A1IE         0x01

#define DS3231_STA_OSF          0x80
#define DS3231_STA_32KHZ        0x08
#define DS3231_STA_BSY          0x04
#define DS3231_STA_A2F          0x02
#define DS3231_STA_A1F          0x01


/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef enum
{
  SUNDAY = 1,
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY
} DS3231_DaysOfWeek;


/* ------------------------------------------------------------- --
   Variables
-- ------------------------------------------------------------- */
DS3231_t DS3231;


/* ------------------------------------------------------------- --
   private prototypes
-- ------------------------------------------------------------- */
static uint8_t B2D(uint8_t bcd);
static uint8_t D2B(uint8_t decimal);

/* ============================================================= ==
   private functions
== ============================================================= */
/** ************************************************************* *
 * @brief      Convert the bcd to decimal
 * 
 * @param      bcd 
 * @return     uint8_t 
 * ************************************************************* **/
static uint8_t B2D(uint8_t bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}

/** ************************************************************* *
 * @brief      Convert the decimal to bcd
 * 
 * @param      decimal 
 * @return     uint8_t 
 * ************************************************************* **/
static uint8_t D2B(uint8_t decimal)
{
	return (((decimal / 10) << 4) | (decimal % 10));
}


/* ============================================================= ==
   Functions
== ============================================================= */
/** ************************************************************* *
 * @brief      init the time of the ds3231
 * 
 * @return     uint8_t 
 * ************************************************************* **/
uint8_t DS3231_Init()
{
	DS3231.Sec =         0;  /* 0 sec */
	DS3231.Min =         0;  /* 0 mins */
	DS3231.Hour =        0;  /* 0 hours */
	DS3231.DaysOfWeek =  1;  /* 1 = sunday*/
	DS3231.Date =        1;  /* 1st */
	DS3231.Month =       1;  /* January */
	DS3231.Year =        0;  /* 0000 */	
	if(DS3231_Set_Time()) return HAL_ERROR;

	return HAL_OK;
}

/** ************************************************************* *
 * @brief       
 * 
 * @return      DS3231_t 
 * ************************************************************* **/
DS3231_t DS3231_Get_Struct(void)
{
	return DS3231;
}

/** ************************************************************* *
 * @brief      Read the time and the temperature
 * 
 * @return     uint8_t 
 * ************************************************************* **/
uint8_t DS3231_Read_All()
{
	if(DS3231_Read_Time()) return HAL_ERROR;
	if(DS3231_Read_Temperature()) return HAL_ERROR;

	return HAL_OK;
}

/** ************************************************************* *
 * @brief      Read the time
 * 
 * @return     uint8_t 
 * ************************************************************* **/
uint8_t DS3231_Read_Time()
{
	uint8_t data[7];	

	if(HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDR, DS3231_REG_TIME, 1, data, sizeof(data), TIMEOUT_I2C )) return HAL_ERROR;	

	DS3231.Sec = B2D(data[0] & 0x7F);
	DS3231.Min = B2D(data[1] & 0x7F);
	DS3231.Hour = B2D(data[2] & 0x3F);
	DS3231.DaysOfWeek = data[3] & 0x07;
	DS3231.Date = B2D(data[4] & 0x3F);
	DS3231.Month = B2D(data[5] & 0x1F);
	DS3231.Year = B2D(data[6]);	

	return HAL_OK;
}

/** ************************************************************* *
 * @brief      Read the temperature
 * 
 * @return     uint8_t 
 * ************************************************************* **/
uint8_t DS3231_Read_Temperature()
{
	uint8_t data[2];

	if(HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDR, DS3231_REG_TEMP, 1, data, sizeof(data), TIMEOUT_I2C )) return HAL_ERROR;

	int16_t value = (data[0] << 8) | (data[1]);
	value = (value >> 6);
	DS3231.temperature = (value / 4.0f);

	return HAL_OK;
}

/** ************************************************************* *
 * @brief      Set the time
 * 
 * @return     uint8_t 
 * ************************************************************* **/
uint8_t DS3231_Set_Time()
{
	uint8_t startAddr = DS3231_REG_TIME;
	uint8_t data[8] = {startAddr, D2B(DS3231.Sec), D2B(DS3231.Min), D2B(DS3231.Hour), DS3231.DaysOfWeek, D2B(DS3231.Date), D2B(DS3231.Month), D2B(DS3231.Year)};

	if(HAL_I2C_Master_Transmit(&hi2c1, DS3231_ADDR, data, sizeof(data), HAL_MAX_DELAY)) return HAL_ERROR;	
	
	return HAL_OK;
}


/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */