/** ************************************************************* *
 * @file        ds18b20.c
 * @brief       
 * 
 * @date        2021-04-30
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "ds18b20.h"
#include "gpio.h"


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
#define DS18B20_PORT    GPIOF
#define DS18B20_PIN     GPIO_PIN_0

/* ------------------------------------------------------------- --
   private prototypes
-- ------------------------------------------------------------- */
static void Set_Pin_Output(void);
static void Set_Pin_Input(void);
static uint8_t DS18B20_Write(uint8_t data);
static uint8_t DS18B20_Read(void);

DS18B20_t DS18B20;

/* ============================================================= ==
   private functions
== ============================================================= */
/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
static void Set_Pin_Output(void)
{
    GPIOF->MODER 	|= (1<<0);                 	//PF0 output
    GPIOF->OTYPER 	&= ~(1<<0);               	//PF0 push-pull
    GPIOF->OSPEEDR 	|= (1<<0);               	//PF0 fast mode
    GPIOF->PUPDR 	&= ~((1<<0) | (1<<1));     	//PF0 no pull up/down
}

/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
static void Set_Pin_Input(void)
{
    GPIOF->MODER 	&= ~((1<<0) | (1<<1));     //PF0 input
    GPIOF->PUPDR	&= ~((1<<0) | (1<<1));     //PF0 no pull up/down
}

/** ************************************************************* *
 * @brief       
 * 
 * @param       data 
 * @return      uint8_t 
 * ************************************************************* **/
static uint8_t DS18B20_Write(uint8_t data)
{
    Set_Pin_Output();  // set as output

    	for (int i=0; i<8; i++)
    	{

    		if ((data & (1<<i))!=0)  // if the bit is high
    		{
    			// write 1

    			Set_Pin_Output();  // set as output
    			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin LOW
    			delay_us (1);  // wait for 1 us

    			Set_Pin_Input();  // set as input
    			delay_us (50);  // wait for 60 us
    		}

    		else  // if the bit is low
    		{
    			// write 0

    			Set_Pin_Output();
    			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin LOW
    			delay_us (50);  // wait for 60 us

    			Set_Pin_Input();
    		}
    	}

    return HAL_OK;
}

/** ************************************************************* *
 * @brief       
 * 
 * @return      uint8_t 
 * ************************************************************* **/
static uint8_t DS18B20_Read(void)
{
    uint8_t value=0;
    Set_Pin_Input ();

	for (int i=0;i<8;i++)
	{
		Set_Pin_Output ();   // set as output

		HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the data pin LOW
		delay_us (2);  // wait for 2 us

		Set_Pin_Input ();  // set as input
		if (HAL_GPIO_ReadPin (DS18B20_PORT, DS18B20_PIN))  // if the pin is HIGH
		{
			value |= 1<<i;  // read = 1
		}
		delay_us (60);  // wait for 60 us
	}
	return value;
}

/* ============================================================= ==
   public functions
== ============================================================= */
/** ************************************************************* *
 * @brief       
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t DS18B20_Init(void)
{
	/* delay_init */
	delay_init();

	Set_Pin_Output();   // set the pin as output
	HAL_GPIO_WritePin (DS18B20_PORT, GPIO_PIN_0, 0);  // pull the pin low
	delay_us (480);   // delay according to datasheet

	Set_Pin_Input();    // set the pin as input
	delay_us (80);    // delay according to datasheet

	if( ! HAL_GPIO_ReadPin (DS18B20_PORT, DS18B20_PIN)) return HAL_ERROR;    // if the pin is low i.e the presence pulse is detected

	delay_us (400); // 480 us delay totally.

	return HAL_OK;
}


/** ************************************************************* *
 * @brief       
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t DS18B20_Get_Temp(void)
{
    uint8_t Presence;

    Presence = DS18B20_Init ();
    HAL_Delay (1);
    DS18B20_Write (0xCC);  // skip ROM
    DS18B20_Write (0x44);  // convert t
    HAL_Delay (800);

    Presence = DS18B20_Init ();
    HAL_Delay(1);
    DS18B20_Write (0xCC);  // skip ROM
    DS18B20_Write (0xBE);  // Read Scratch-pad

    DS18B20.LSB = DS18B20_Read();
    DS18B20.MSB = DS18B20_Read();

    DS18B20.temperature = ((DS18B20.LSB<<8 | DS18B20.MSB)/16);

    return Presence;
}

DS18B20_t DS18B20_Get_Struct()
{
	return DS18B20;
}
