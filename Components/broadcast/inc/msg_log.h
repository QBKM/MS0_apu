/** ************************************************************* *
 * @file        msg_log.h
 * @brief       
 * 
 * @date        2021-05-08
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __MSG_LOG__
#define __MSG_LOG__

/* ------------------------------------------------------------- --
   includeq
-- ------------------------------------------------------------- */
#include "stdint.h"


/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef enum
{
  PHASE_WAIT,
  PHASE_ASCEND,
  PHASE_DEPLOY,
  PHASE_DESCEND,
  PHASE_LANDED
}phase_t;

typedef enum
{
    JACK_PLUGGED,
    JACK_UNPLUGGED
}jack_t;

typedef enum
{
    WINDOW_LOCK,
    WINDOW_UNLOCK,
    WINDOW_RELOCK
}window_t;

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
phase_t phase;
jack_t jack;
window_t window_IT;
window_t window_POOL;

/* ------------------------------------------------------------- --
   function prototypes
-- ------------------------------------------------------------- */
void MSG_LOG_init(void);
void MSG_LOG_push(const uint8_t message);
void MSG_LOG_pop(void);
void MSG_LOG_dispatch(const uint8_t message);


#endif