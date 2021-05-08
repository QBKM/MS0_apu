/** ************************************************************* *
 * @file       broadcast.c
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
#include "broadcast.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "config_file.h"

#include "msg_list.h"
#include "msg_log.h"


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
#ifndef TIMEOUT_UART
#define TIMEOUT_UART 10
#endif

/* uart peripheral */
#define HUART  huart2

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
char OUT_MSG[10];
char IN_MSG[10];

/* ------------------------------------------------------------- --
   Public functions
-- ------------------------------------------------------------- */
/** ************************************************************* *
 * @brief      send on uart a short ID for other boards
 * 
 * @param      message 
 * ************************************************************* **/
void broadcast_uart_send(const uint8_t message)
{
	sprintf(OUT_MSG, "%c", message);
   HAL_UART_Transmit(&HUART, (uint8_t*)OUT_MSG, strlen(OUT_MSG), TIMEOUT_UART);
}

/** ************************************************************* *
 * @brief       receive on uart a short ID from other boards
 * 
 * @param       message 
 * ************************************************************* **/
void broadcast_uart_receive(void)
{
   IN_MSG[0] = HUART.Instance->RDR;
   MSG_LOG_push(IN_MSG[0]);
   MSG_LOG_pop();
}
