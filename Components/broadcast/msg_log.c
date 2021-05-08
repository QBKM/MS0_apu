/** ************************************************************* *
 * @file        msg_log.c
 * @brief       
 * 
 * @date        2021-05-08
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "msg_log.h"
#include "msg_list.h"

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
#define BUFF_SIZE 64

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
uint8_t MSG_LOG[BUFF_SIZE] = {0};
uint8_t buff_index = 0;

/* ============================================================= ==
   private functions
== ============================================================= */
void MSG_LOG_dispatch(const uint8_t message)
{
    switch (message)
    {
        case 0xA1: phase = PHASE_WAIT;              break;
        case 0xA2: phase = PHASE_ASCEND;            break;
        case 0xA3: phase = PHASE_DEPLOY;            break;
        case 0xA4: phase = PHASE_DESCEND;           break;
        case 0xA5: phase = PHASE_LANDED;            break;

        case 0xB1: jack = JACK_PLUGGED;             break;
        case 0xB2: jack = JACK_UNPLUGGED;           break;
        
        case 0xF1: window_IT    = WINDOW_UNLOCK;    break;
        case 0xF2: window_POOL  = WINDOW_UNLOCK;    break;
        case 0xF3: window_IT    = WINDOW_RELOCK;    break;
        case 0xF4: window_POOL  = WINDOW_RELOCK;    break;
        default:                                    break;
    }
}

/* ============================================================= ==
   public functions
== ============================================================= */
/** ************************************************************* *
 * @brief       init the msg log
 * 
 * ************************************************************* **/
void MSG_LOG_init(void)
{
    phase       = PHASE_WAIT;
    jack        = JACK_UNPLUGGED;
    window_IT   = WINDOW_LOCK;
    window_POOL = WINDOW_LOCK;

    buff_index = 0;
}

/** ************************************************************* *
 * @brief       push the message into the message buffer
 * 
 * @param       message 
 * ************************************************************* **/
void MSG_LOG_push(const uint8_t message)
{
    MSG_LOG[buff_index] = message;
    buff_index +=1;
}

/** ************************************************************* *
 * @brief       pop the message from the message buffer
 * 
 * ************************************************************* **/
void MSG_LOG_pop(void)
{
    while(buff_index > 0)
    {
        MSG_LOG_dispatch(MSG_LOG[0]);
        buff_index -=1;
    }
}
