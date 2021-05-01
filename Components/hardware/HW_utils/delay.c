/** ************************************************************* *
 * @file        delay.c
 * @brief       
 * 
 * @date        2021-04-30
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#include "delay.h"
#include "tim.h"
#include "stdint.h"

/** ************************************************************* *
 * @brief       init the timer for delay
 * 
 * ************************************************************* **/
void delay_init(void)
{
    HAL_TIM_Base_Start(&htim3);
}

/** ************************************************************* *
 * @brief       delay for us
 * 
 * @param       us 
 * ************************************************************* **/
void delay_us(uint32_t us)
{
    __HAL_TIM_SET_COUNTER(&htim3,0);
    while ((__HAL_TIM_GET_COUNTER(&htim3))<us);
}
