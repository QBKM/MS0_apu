/** ************************************************************* *
 * @file        tca6408a.c
 * @brief       
 * 
 * @date        2021-05-01
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#include "tca6408a.h"
#include "config_file.h"
#include "i2c.h"

#ifndef TIMEOUT_I2C
#define TIMEOUT_I2C 10
#endif

/* TCA6408A address */
#ifndef TCA6408A_ADDR
#define TCA6408A_ADDR 0x21
#endif

/*  register map */
#define TCA6408A_INPUT                  0x00
#define TCA6408A_OUTPUT                 0x01
#define TCA6408A_POLARITY               0x02
#define TCA6408A_CONFIG                 0x03

/* TCA6408A masks */
#define TCA6408A_PIN0                   0x01
#define TCA6408A_PIN1                   0x02
#define TCA6408A_PIN2                   0x04
#define TCA6408A_PIN3                   0x08
#define TCA6408A_PIN4                   0x10
#define TCA6408A_PIN5                   0x20
#define TCA6408A_PIN6                   0x40
#define TCA6408A_PIN7                   0x80

typedef enum PIN_STATE_t
{
    HIGH,
    LOW
}PIN_STATE_t;

typedef struct TCA6408A_t
{
    PIN_STATE_t PIN_0;
    PIN_STATE_t PIN_1;
    PIN_STATE_t PIN_2;
    PIN_STATE_t PIN_3;
    PIN_STATE_t PIN_4;
    PIN_STATE_t PIN_5;
    PIN_STATE_t PIN_6;
    PIN_STATE_t PIN_7;
}TCA6408A_t;

TCA6408A_t TCA6408A;

/** ************************************************************* *
 * @brief       
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t TCA6408A_Init(void)
{
    TCA6408A_t temp = 
    {
        .PIN_0 = LOW,
        .PIN_1 = LOW,
        .PIN_2 = LOW,
        .PIN_3 = LOW,
        .PIN_4 = LOW,
        .PIN_5 = LOW,
        .PIN_6 = LOW,
        .PIN_7 = LOW
    };
    TCA6408A = temp;

    //if(HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR<<1, BMP280_REG_ID, 1, &check, 1, TIMEOUT_I2C)) return HAL_ERROR;


    return HAL_OK;
}
