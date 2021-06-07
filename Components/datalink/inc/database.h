/** ************************************************************* *
 * @file        database.h
 * @brief       
 * 
 * @date        2021-06-07
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "bmp280.h"
#include "ds18b20.h"
#include "ds3231.h"
#include "mpu6050.h"

typedef struct
{
    BMP280_t    BMP280;
    DS18B20_t   DS18B20;
    DS3231_t    DS3231;
    MPU6050_t   MPU6050;
}DATABASE_t;

void DATABASE_fill_struct(DATABASE_t);

#endif
