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

#include "ds3231.h"
#include "mpu6050.h"
#include "bmp280.h"

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
uint8_t HMI_OLED_display_init_log(HW_status_t HW_init, uint32_t time, HMI_AfterDisp_t action);

uint8_t HMI_OLED_display_data_log(void);
void    HMI_OLED_display_data_log_failed(uint8_t HEIGHT);
void    HMI_OLED_display_data_log_time(DS3231_t TIME);
void    HMI_OLED_display_data_log_press(BMP280_t PRESS);
void    HMI_OLED_display_data_log_angle(MPU6050_t ANGLE);

#endif
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
