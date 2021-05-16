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

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
/* OLED lines position */
#define HMI_OLED_LINE_NEXT			(uint8_t)10
#define HMI_OLED_LINE_1				(uint8_t)10
#define HMI_OLED_LINE_2				(uint8_t)20
#define HMI_OLED_LINE_3				(uint8_t)30
#define HMI_OLED_LINE_4				(uint8_t)40
#define HMI_OLED_LINE_5				(uint8_t)50

/* OLED columns position */
#define HMI_OLED_INIT_LOG_COLUMN	(uint8_t)(12*7)
#define HMI_OLED_DATA_LOG_COLUMN	(uint8_t)(7*8)
#define HMI_OLED_STATUS_COLUMN		(uint8_t)(7*9)

/* OLED columns for time display in data line */
#define HMI_OLED_DATA_LOG_COLUMN_HOUR	(uint8_t)(7*8)
#define HMI_OLED_DATA_LOG_COLUMN_DOT1	(uint8_t)(7*10)
#define HMI_OLED_DATA_LOG_COLUMN_MIN	(uint8_t)(7*11)
#define HMI_OLED_DATA_LOG_COLUMN_DOT2	(uint8_t)(7*13)
#define HMI_OLED_DATA_LOG_COLUMN_SEC	(uint8_t)(7*14)


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

/* display data log screen */
uint8_t     HMI_OLED_display_data_log(void);
void        HMI_OLED_display_data_log_failed(uint8_t LINE);
void        HMI_OLED_display_data_log_time(DS3231_t TIME, uint8_t LINE);
void        HMI_OLED_display_data_log_press(BMP280_t PRESS, uint8_t LINE);
void        HMI_OLED_display_data_log_angle(MPU6050_t ANGLE, uint8_t LINEx, uint8_t LINEy);

/* display status screen */
uint8_t		HMI_OLED_display_status(void);
void		   HMI_OLED_display_status_phase(uint8_t LINE);
void		   HMI_OLED_display_status_jack(uint8_t LINE);
void		   HMI_OLED_display_status_errors_number(uint8_t LINE);

/* display errors list */
void		   HMI_OLED_display_error_list(void);


/* **************************************** **
 * Optional features
 * **************************************** */
void        HMI_OLED_display_save_screen(uint8_t* save);
void        HMI_OLED_display_restore_screen(uint8_t* save);


#endif
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
