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
#include "tca6408a.h"

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */


/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */


/* ============================================================= ==
   private functions
== ============================================================= */
void MSG_LOG_dispatch(const uint8_t message)
{
    switch (message)
    {
        case 0xA1: phase = PHASE_WAIT;        MSG_SEQ_PHASE = message;   TCA6408A_Write_Pin(TCA6408A_PIN4, LOW);   break;
        case 0xA2: phase = PHASE_ASCEND;      MSG_SEQ_PHASE = message;   TCA6408A_Write_Pin(TCA6408A_PIN4, HIGH);  break;
        case 0xA3: phase = PHASE_DEPLOY;      MSG_SEQ_PHASE = message;      break;
        case 0xA4: phase = PHASE_DESCEND;     MSG_SEQ_PHASE = message;      break;
        case 0xA5: phase = PHASE_LANDED;      MSG_SEQ_PHASE = message;      break; 

        case 0xB1: jack = JACK_PLUGGED;             break;
        case 0xB2: jack = JACK_UNPLUGGED;           break;

        case 0xC6: MSG_SEQ = message;        MSG_SEQ_MOTOR = message;       break;  /* motor start */
        case 0xC7: MSG_SEQ = message;        MSG_SEQ_MOTOR = message;       break;  /* motor stop */
        
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
    phase         = PHASE_WAIT;
    jack          = JACK_PLUGGED;
    window_IT     = WINDOW_LOCK;
    window_POOL   = WINDOW_LOCK;
    MSG_SEQ       = 0;
    MSG_SEQ_MOTOR = 0;
    MSG_SEQ_PHASE = 0;
}

