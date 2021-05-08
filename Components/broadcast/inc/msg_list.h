/** ************************************************************* *
 * @file        msg_list.h
 * @brief       
 * 
 * @date        2021-05-08
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __MSG_LIST__
#define __MSG_LIST__

/* ------------------------------------------------------------- --
   constant
-- ------------------------------------------------------------- */
/* phases ID */
#define MSG_ID_phase_wait           (uint8_t)0xA1
#define MSG_ID_phase_ascend         (uint8_t)0xA2
#define MSG_ID_phase_deploy         (uint8_t)0xA3
#define MSG_ID_phase_descend        (uint8_t)0xA4
#define MSG_ID_phase_landed         (uint8_t)0xA5

/* hard ID */
#define MSG_ID_HW_jack_plugged      (uint8_t)0xB1
#define MSG_ID_HW_jack_unplugged    (uint8_t)0xB2

/* temporal window ID */
#define MSG_ID_unlock_window_IT     (uint8_t)0xF1
#define MSG_ID_unlock_window_POOL   (uint8_t)0xF2
#define MSG_ID_relock_window_IT     (uint8_t)0xF3
#define MSG_ID_relock_window_POOL   (uint8_t)0xF4

#endif