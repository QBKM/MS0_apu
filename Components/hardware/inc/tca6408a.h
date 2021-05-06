/** ************************************************************* *
 * @file        tca6408a.h
 * @brief       
 * 
 * @date        2021-05-01
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __TCA6408A__
#define __TCA6408A__

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "stdint.h"


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
/* TCA6408A masks */
#define TCA6408A_PIN0                   0x01
#define TCA6408A_PIN1                   0x02
#define TCA6408A_PIN2                   0x04
#define TCA6408A_PIN3                   0x08
#define TCA6408A_PIN4                   0x10
#define TCA6408A_PIN5                   0x20
#define TCA6408A_PIN6                   0x40
#define TCA6408A_PIN7                   0x80


/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef enum PIN_STATE_t
{
    LOW,
    HIGH,
}PIN_STATE_t;

typedef enum MODE_STATE_t
{
    OUTPUT,
    INPUT
}MODE_STATE_t;


/* ------------------------------------------------------------- --
   public prototypes
-- ------------------------------------------------------------- */
uint8_t TCA6408A_Init(void);

uint8_t TCA6408A_Set_Mode(uint8_t PIN, MODE_STATE_t MODE);

uint8_t TCA6408A_Write_Pin(uint8_t PIN,  PIN_STATE_t STATE);
uint8_t TCA6408A_Write_Pin_All(uint8_t PINS);

uint8_t TCA6408A_Read_Pin(uint8_t PIN);
uint8_t TCA6408A_Read_Pin_All(void);


#endif
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */