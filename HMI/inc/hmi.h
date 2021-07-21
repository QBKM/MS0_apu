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
#include "bmp280.h"
#include "mpu6050.h"
#include "ds18b20.h"

#include "config_file.h"

/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef enum
{
    KeepAfter,
    ClearAfter
}HMI_AfterDisp_t;

typedef enum
{
    NO_FRAME,
    MENU_FRAME,
    INIT_LOG_FRAME,
    DATA_LOG_FRAME,
	STATUS_FRAME
}HMI_FrameSelector_t;

typedef enum
{
    NO_PUSH,
    PUSH_UP,
    PUSH_MIDDLE,
    PUSH_BOTTOM
}HMI_PushStatus_t;

typedef struct
{
    HMI_PushStatus_t    Button;
    uint8_t             Line;
}HMI_Menu_t;

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
HMI_FrameSelector_t Frame;
HW_status_t HW_status;


/* ------------------------------------------------------------- --
   function prototypes
-- ------------------------------------------------------------- */
void        HMI_OLED_init(void);

/* diplay the main menu */
void     HMI_OLED_display_menu(void);
void     HMI_OLED_display_menu_selector(void);
void     HMI_OLED_check_menu(void);
void 		HMI_OLED_IT_btn_up(void);
void 		HMI_OLED_IT_btn_middle(void);
void 		HMI_OLED_IT_btn_bottom(void);

/* display a bit map */
uint8_t  HMI_OLED_display_bitmap(const unsigned char* bitmap, uint32_t time, HMI_AfterDisp_t action);


/* **************************************** **
 * Menu frame
 * **************************************** */
/* display the running menu (default and time saving screen) */
void        HMI_OLED_display_running(void);

/* display init log screen */
uint8_t     HMI_OLED_display_init_log(HW_status_t HW_init, uint32_t time, HMI_AfterDisp_t action);
void        HMI_OLED_display_init_log_time(HW_status_t HW_init);
void        HMI_OLED_display_init_log_press(HW_status_t HW_init);
void        HMI_OLED_display_init_log_angle(HW_status_t HW_init);
void        HMI_OLED_display_init_log_temp(HW_status_t HW_init);
void        HMI_OLED_display_init_log_sd(HW_status_t HW_init);

/* display data log screen */
uint8_t     HMI_OLED_display_data_log(void);
void        HMI_OLED_display_data_log_failed(uint8_t LINE);
void        HMI_OLED_display_data_log_time(DS3231_t TIME);
void        HMI_OLED_display_data_log_press(BMP280_t PRESS);
void        HMI_OLED_display_data_log_angle(MPU6050_t ANGLE);
void        HMI_OLED_display_data_log_temp(DS18B20_t TEMP);


/* display status screen */
uint8_t		HMI_OLED_display_status(void);
void		   HMI_OLED_display_status_phase(void);
void		   HMI_OLED_display_status_jack(void);


/* **************************************** **
 * Optional features
 * **************************************** */
void        HMI_OLED_display_save_screen(uint8_t* save);
void        HMI_OLED_display_restore_screen(uint8_t* save);


#endif
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
