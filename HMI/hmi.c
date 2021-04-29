/** ************************************************************* *
 * @file        hmi.c
 * @brief       
 * 
 * @date        2021-04-26
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/


/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "ssd1306.h"
#include "hmi.h"

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
/* OLED lines position */
#define HMI_OLED_LINE_1				(uint8_t)10
#define HMI_OLED_LINE_2				(uint8_t)20
#define HMI_OLED_LINE_3				(uint8_t)30
#define HMI_OLED_LINE_4				(uint8_t)40
#define HMI_OLED_LINE_5				(uint8_t)50

/* OLED columns position */
#define HMI_OLED_INIT_LOG_COLUMN	(uint8_t)(12*7)
#define HMI_OLED_DATA_LOG_COLUMN	(uint8_t)(7*8)

/* OLED columns for time diplay in data line */
#define HMI_OLED_DATA_LOG_COLUMN_HOUR	(uint8_t)(7*8)
#define HMI_OLED_DATA_LOG_COLUMN_DOT1	(uint8_t)(7*10)
#define HMI_OLED_DATA_LOG_COLUMN_MIN	(uint8_t)(7*11)
#define HMI_OLED_DATA_LOG_COLUMN_DOT2	(uint8_t)(7*13)
#define HMI_OLED_DATA_LOG_COLUMN_SEC	(uint8_t)(7*14)

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
uint8_t HMI_save_data_log_screen[SSD1306_WIDTH * SSD1306_HEIGHT / 8];


/* ============================================================= ==
   public functions
== ============================================================= */
/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void HMI_OLED_init(void)
{
	HMI_OLED_setup_data_log_save(HMI_save_data_log_screen);
}


/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void HMI_OLED_setup_data_log_save(uint8_t* save)
{
	SSD1306_Fill (SSD1306_COLOR_BLACK);
	SSD1306_GotoXY(0, 10);
	SSD1306_Puts("==== DATA LOG ====", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, 20);
	SSD1306_Puts("time   : ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, 30);
	SSD1306_Puts("press  : ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, 40);
  	SSD1306_Puts("angleX : ", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(0, 50);
  	SSD1306_Puts("angleY : ", &Font_7x10, SSD1306_COLOR_WHITE);

	HMI_OLED_display_save_screen(save);
	SSD1306_Fill (SSD1306_COLOR_BLACK);
}

/** ************************************************************* *
 * @brief       display a logo on the oled screen for a duration
 * 
 * @param       bitmap 
 * @param       time 
 * ************************************************************* **/
uint8_t HMI_OLED_display_bitmap(const unsigned char* bitmap, uint32_t time, HMI_AfterDisp_t action)
{
    if(SSD1306_Clear()) return HAL_ERROR;
    SSD1306_DrawBitmap(0, 0, bitmap, SSD1306_WIDTH, SSD1306_HEIGHT, SSD1306_COLOR_WHITE);
    if(SSD1306_UpdateScreen()) return HAL_ERROR;

    HAL_Delay(time);
    if(action == ClearAfter) if(SSD1306_Clear()) return HAL_ERROR;

    return HAL_OK;
}

/** ************************************************************* *
 * @brief       display the status of sensor init for a duration
 * 
 * @param       HW_init 
 * @param       time 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t HMI_OLED_display_init_log(HW_status_t HW_init, uint32_t time, HMI_AfterDisp_t action)
{
	SSD1306_GotoXY(0, HMI_OLED_LINE_1);
	SSD1306_Puts("DS3231....",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(0, HMI_OLED_LINE_2);
	SSD1306_Puts("BMP280....",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(0, HMI_OLED_LINE_3);
	SSD1306_Puts("MPU6050...",&Font_7x10, SSD1306_COLOR_WHITE);

    /* DS3231 status */
	HMI_OLED_display_init_log_time(HW_init);

    /* BMP280 status */
	HMI_OLED_display_init_log_press(HW_init);

    /* MPU6050 status */
	HMI_OLED_display_init_log_angle(HW_init);

    /* update */
	if(SSD1306_UpdateScreen()) return HAL_ERROR;

	HAL_Delay(time);
    if(action == ClearAfter) SSD1306_Clear();

    return HAL_OK;
}

/** ************************************************************* *
 * @brief       display the DS3231 status
 * 
 * @param       HW_init 
 * ************************************************************* **/
void HMI_OLED_display_init_log_time(HW_status_t HW_init)
{
	SSD1306_GotoXY(HMI_OLED_INIT_LOG_COLUMN, HMI_OLED_LINE_1);
	if(HW_init.DS3231 == HAL_OK)
	{
		SSD1306_Puts("OK",&Font_7x10, SSD1306_COLOR_WHITE);
	}
	else
	{
		SSD1306_Puts("FAILED",&Font_7x10, SSD1306_COLOR_WHITE);
	}
}


/** ************************************************************* *
 * @brief       diplay the BMP280 status
 * 
 * @param       HW_init 
 * ************************************************************* **/
void HMI_OLED_display_init_log_press(HW_status_t HW_init)
{
	SSD1306_GotoXY(HMI_OLED_INIT_LOG_COLUMN, HMI_OLED_LINE_2);
	if(HW_init.BMP280 == HAL_OK)
	{
		SSD1306_Puts("OK",&Font_7x10, SSD1306_COLOR_WHITE);
	}
	else
	{
		SSD1306_Puts("FAILED",&Font_7x10, SSD1306_COLOR_WHITE);
	}
}


/** ************************************************************* *
 * @brief       display the MPU6050 status
 * 
 * @param       HW_init 
 * ************************************************************* **/
void HMI_OLED_display_init_log_angle(HW_status_t HW_init)
{
	SSD1306_GotoXY(HMI_OLED_INIT_LOG_COLUMN, HMI_OLED_LINE_3);
	if(HW_init.MPU6050 == HAL_OK)
	{
		SSD1306_Puts("OK",&Font_7x10, SSD1306_COLOR_WHITE);
	}
	else
	{
		SSD1306_Puts("FAILED",&Font_7x10, SSD1306_COLOR_WHITE);
	}

}


/** ************************************************************* *
 * @brief       display the menu for data log
 * 
 * @param       time 
 * @param       action 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t HMI_OLED_display_data_log(void)
{
	HMI_OLED_display_restore_screen(HMI_save_data_log_screen);

    /* update */
	if(SSD1306_UpdateScreen()) return HAL_ERROR;

	return HAL_OK;
}

/** ************************************************************* *
 * @brief       display a failed message at a specific height
 * 
 * @param       LINE 
 * ************************************************************* **/
void HMI_OLED_display_data_log_failed(uint8_t LINE)
{
	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, LINE);
	SSD1306_Puts("FAILED  ", &Font_7x10, SSD1306_COLOR_WHITE);
}

/** ************************************************************* *
 * @brief       display the current time
 * 
 * ************************************************************* **/
void HMI_OLED_display_data_log_time(DS3231_t TIME)
{
	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN_DOT1, HMI_OLED_LINE_2);
	SSD1306_Puts(":", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN_DOT2, HMI_OLED_LINE_2);
	SSD1306_Puts(":", &Font_7x10, SSD1306_COLOR_WHITE);


   	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN_HOUR, HMI_OLED_LINE_2);
   	SSD1306_Puts_Num16bits(TIME.Hour, &Font_7x10, SSD1306_COLOR_WHITE);

   	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN_MIN, HMI_OLED_LINE_2);
   	SSD1306_Puts_Num16bits(TIME.Min, &Font_7x10, SSD1306_COLOR_WHITE);

   	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN_SEC, HMI_OLED_LINE_2);
   	SSD1306_Puts_Num16bits(TIME.Sec, &Font_7x10, SSD1306_COLOR_WHITE);
}


/** ************************************************************* *
 * @brief       display the current pressure
 * 
 * ************************************************************* **/
void HMI_OLED_display_data_log_press(BMP280_t PRESS)
{
	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_LINE_3);
	SSD1306_Puts("        ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_LINE_3);
	SSD1306_Puts_float((PRESS.pressure/100), &Font_7x10, SSD1306_COLOR_WHITE);
}


/** ************************************************************* *
 * @brief       display the current angle
 * 
 * ************************************************************* **/
void HMI_OLED_display_data_log_angle(MPU6050_t ANGLE)
{
	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_LINE_4);
	SSD1306_Puts("        ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_LINE_4);
	SSD1306_Puts_float(ANGLE.KalmanAngleX, &Font_7x10, SSD1306_COLOR_WHITE);


	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_LINE_5);
	SSD1306_Puts("        ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_LINE_5);
	SSD1306_Puts_float(ANGLE.KalmanAngleY, &Font_7x10, SSD1306_COLOR_WHITE);
}

/** ************************************************************* *
 * @brief       save the current buffer
 * 
 * @return      uint8_t* 
 * ************************************************************* **/
void HMI_OLED_display_save_screen(uint8_t* save)
{
	memcpy(save, SSD1306_Buffer, sizeof(SSD1306_Buffer));
}

void HMI_OLED_display_restore_screen(uint8_t* save)
{
	memcpy(SSD1306_Buffer, save, sizeof(SSD1306_Buffer));
}
