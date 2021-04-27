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

/* ============================================================= ==
   public functions
== ============================================================= */
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
uint8_t HMI_OLED_display_init_log(HW_init_t HW_init, uint32_t time, HMI_AfterDisp_t action)
{
	SSD1306_GotoXY(0, 10);
	SSD1306_Puts("BMP280....",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(0, 20);
	SSD1306_Puts("DS3231....",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(0, 30);
	SSD1306_Puts("MPU6050...",&Font_7x10, SSD1306_COLOR_WHITE);

    /* BMP280 status */
	SSD1306_GotoXY(12*7, 10);
	if(HW_init.BMP280 == HAL_OK)
	{
		SSD1306_Puts("OK",&Font_7x10, SSD1306_COLOR_WHITE);
	}
	else
	{
		SSD1306_Puts("FAILED",&Font_7x10, SSD1306_COLOR_WHITE);
	}

    /* DS3231 status */
	SSD1306_GotoXY(12*7, 20);
	if(HW_init.DS3231 == HAL_OK)
	{
		SSD1306_Puts("OK",&Font_7x10, SSD1306_COLOR_WHITE);
	}
	else
	{
		SSD1306_Puts("FAILED",&Font_7x10, SSD1306_COLOR_WHITE);
	}

    /* MPU6050 status */
	SSD1306_GotoXY(12*7, 30);
	if(HW_init.MPU6050 == HAL_OK)
	{
		SSD1306_Puts("OK",&Font_7x10, SSD1306_COLOR_WHITE);
	}
	else
	{
		SSD1306_Puts("FAILED",&Font_7x10, SSD1306_COLOR_WHITE);
	}

    /* update */
	if(SSD1306_UpdateScreen()) return HAL_ERROR;

	HAL_Delay(5000);
    if(action == ClearAfter) SSD1306_Clear();

    return HAL_OK;
}