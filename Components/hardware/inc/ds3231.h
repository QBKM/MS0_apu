/** ************************************************************* *
 * @file       ds3231.h
 * @brief      
 * 
 * @date       2021-04-22
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __DS3231_H__
#define __DS3231_H__


/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "stdint.h"


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
typedef struct
{
   uint8_t Year;
   uint8_t Month;
   uint8_t Date;
   uint8_t DaysOfWeek;
   uint8_t Hour;
   uint8_t Min;
   uint8_t Sec;

   float temperature;
}DS3231_t;


/* ------------------------------------------------------------- --
   functions
-- ------------------------------------------------------------- */
uint8_t  DS3231_Init(void);
DS3231_t DS3231_Get_Struct(void);
uint8_t  DS3231_Read_All(void);
uint8_t  DS3231_Read_Time(void);
uint8_t  DS3231_Read_Temperature(void);
uint8_t  DS3231_Set_Time(void);
DS3231_t DS3231_Get_Struct(void);

#endif /* __DS3231_H__ */
/* ------------------------------------------------------------- --
   end of files
-- ------------------------------------------------------------- */
