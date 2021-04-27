/** ************************************************************* *
 * @file        hmi.h
 * @brief       
 * 
 * @date        2021-04-26
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __HMI_H__
#define __HMI_H__

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "err_manager.h"


/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef enum
{
    KeepAfter,
    ClearAfter
}HMI_AfterDisp_t;


/* ------------------------------------------------------------- --
   function prototypes
-- ------------------------------------------------------------- */
uint8_t HMI_OLED_display_bitmap(const unsigned char* bitmap, uint32_t time, HMI_AfterDisp_t action);
uint8_t HMI_OLED_display_init_log(HW_init_t HW_init, uint32_t time, HMI_AfterDisp_t action);


#endif
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */