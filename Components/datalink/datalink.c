/** ************************************************************* *
 * @file        datalink.c
 * @brief       
 * 
 * @date        2021-05-16
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "datalink.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "config_file.h"


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
#ifndef TIMEOUT_UART
#define TIMEOUT_UART 10
#endif

/* uart peripheral */
#define HUART  huart1

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
char OUT_MSG[64];
char IN_MSG[64];

/* ------------------------------------------------------------- --
   Public functions
-- ------------------------------------------------------------- */
/** ************************************************************* *
 * @brief      send on uart a short ID for other boards
 * 
 * @param      message 
 * ************************************************************* **/
void datalink_uart_send(const uint8_t message)
{
    sprintf(OUT_MSG, "%c", message);
    HAL_UART_Transmit(&HUART, (uint8_t*)OUT_MSG, strlen(OUT_MSG), TIMEOUT_UART);
}

/** ************************************************************* *
 * @brief       receive on uart a short ID from other boards
 * 
 * @param       message 
 * ************************************************************* **/
void datalink_uart_receive(void)
{
    IN_MSG[0] = HUART.Instance->RDR;
    //MSG_LOG_push(IN_MSG[0]);
    //MSG_LOG_pop();
}
