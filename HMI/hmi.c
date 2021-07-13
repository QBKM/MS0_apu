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

#include "ds3231.h"
#include "bmp280.h"
#include "mpu6050.h"

#include "msg_log.h"
#include "err_manager.h"

#include "broadcast.h"

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */


/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
HMI_Menu_t Menu;


/* ============================================================= ==
   public functions
== ============================================================= */
/** ************************************************************* *
 * @brief       init the HMI with OLED
 * 
 * ************************************************************* **/
void HMI_OLED_init(void)
{
	SSD1306_Clear();
	Frame 		= NO_FRAME;
	Menu.Button	= NO_PUSH;
	Menu.Line 	= HMI_OLED_LINE_2;
}

/** ************************************************************* *
 * @brief       display the frame for running mode
 * 
 * ************************************************************* **/
void HMI_OLED_display_running(void)
{
	SSD1306_Clear();
	SSD1306_GotoXY(0, HMI_OLED_LINE_1);
	SSD1306_Puts("The SW is running",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(0, HMI_OLED_LINE_3);
	SSD1306_Puts(">Press middle btn ",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(0, HMI_OLED_LINE_4);
	SSD1306_Puts("   to open menu   ",&Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_UpdateScreen();
}

/** ************************************************************* *
 * @brief       check if the menu must be displayed and display it
 * 
 * ************************************************************* **/
void HMI_OLED_check_menu(void)
{
	if(Menu.Button == PUSH_MIDDLE) 
	{
		if(Frame != MENU_FRAME)
		{
			Menu.Button = NO_PUSH;
			Frame = MENU_FRAME;
			HMI_OLED_display_menu();
		};
	}
}

/** ************************************************************* *
 * @brief       display the menu frame
 * 
 * ************************************************************* **/
void HMI_OLED_display_menu(void)
{
	SSD1306_Clear();

	SSD1306_GotoXY(0, HMI_OLED_MENU_LINE_HEADER);
	SSD1306_Puts("====== MENU ======",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(14, HMI_OLED_MENU_LINE_DATA);
	SSD1306_Puts("Display DATA",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(14, HMI_OLED_MENU_LINE_STATUS);
	SSD1306_Puts("Display STATUS",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(14, HMI_OLED_MENU_LINE_QUIT);
	SSD1306_Puts("Quit menu",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(14, HMI_OLED_MENU_LINE_RESTART);
	SSD1306_Puts("Restart",&Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_UpdateScreen();
}


/** ************************************************************* *
 * @brief       display the selector in menu
 * 
 * ************************************************************* **/
void HMI_OLED_display_menu_selector(void)
{
	SSD1306_GotoXY(0, Menu.Line);
	SSD1306_Puts(">",&Font_7x10, SSD1306_COLOR_WHITE);

	/* UP button */
	if(Menu.Button == PUSH_UP)
	{
		Menu.Button = NO_PUSH;
		SSD1306_GotoXY(0, Menu.Line);
		if(Menu.Line < HMI_OLED_LINE_5) Menu.Line += HMI_OLED_LINE_NEXT;
		SSD1306_Puts(" ",&Font_7x10, SSD1306_COLOR_WHITE);
		SSD1306_GotoXY(0, Menu.Line);
		SSD1306_Puts(">",&Font_7x10, SSD1306_COLOR_WHITE);
	}

	/* MIDDLE button */
	if(Menu.Button == PUSH_BOTTOM)
	{
		Menu.Button = NO_PUSH;
		SSD1306_GotoXY(0, Menu.Line);
		if(Menu.Line > HMI_OLED_LINE_2) Menu.Line -= HMI_OLED_LINE_NEXT;
		SSD1306_Puts(" ",&Font_7x10, SSD1306_COLOR_WHITE);
		SSD1306_GotoXY(0, Menu.Line);
		SSD1306_Puts(">",&Font_7x10, SSD1306_COLOR_WHITE);
	}

	/* BOTTOM button */
	if(Menu.Button == PUSH_MIDDLE)
	{
		Menu.Button = NO_PUSH;

		/* select data log */
		if(Menu.Line == HMI_OLED_MENU_LINE_DATA)
		{
			HMI_OLED_display_data_log();
			Frame = DATA_LOG_FRAME;
		}

		/* select status frame */
		else if(Menu.Line == HMI_OLED_MENU_LINE_STATUS)
		{
			HMI_OLED_display_status();
			Frame = STATUS_FRAME;
		}

		/* select running frame */
		else if(Menu.Line == HMI_OLED_MENU_LINE_QUIT)
		{
			HMI_OLED_display_running();
			Frame = NO_FRAME;
		}

		/* select reset SW */
		else if(Menu.Line == HMI_OLED_MENU_LINE_RESTART)
		{
			/* restart the software */
			SCB->AIRCR = 0x05fa0004;
		}
	}

}

/** ************************************************************* *
 * @brief       set the flag for up button
 * 
 * ************************************************************* **/
void HMI_OLED_IT_btn_up(void)
{
	Menu.Button = PUSH_UP;
}

/** ************************************************************* *
 * @brief       set the flag for middle button
 * 
 * ************************************************************* **/
void HMI_OLED_IT_btn_middle(void)
{
	Menu.Button = PUSH_MIDDLE;
}

/** ************************************************************* *
 * @brief       set the flag for bottom button
 * 
 * ************************************************************* **/
void HMI_OLED_IT_btn_bottom(void)
{
	Menu.Button = PUSH_BOTTOM;
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
	SSD1306_GotoXY(0, HMI_OLED_INIT_LINE_DS3231);
	SSD1306_Puts("DS3231....",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(0, HMI_OLED_INIT_LINE_BMP280);
	SSD1306_Puts("BMP280....",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(0, HMI_OLED_INIT_LINE_MPU6050);
	SSD1306_Puts("MPU6050...",&Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(0, HMI_OLED_INIT_LINE_DS18B20);
	SSD1306_Puts("DS18B20...",&Font_7x10, SSD1306_COLOR_WHITE);

    /* DS3231 status */
	HMI_OLED_display_init_log_time(HW_init);

    /* BMP280 status */
	HMI_OLED_display_init_log_press(HW_init);

    /* MPU6050 status */
	HMI_OLED_display_init_log_angle(HW_init);

    /* DS18B20 status */
	HMI_OLED_display_init_log_temp(HW_init);

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
	SSD1306_GotoXY(HMI_OLED_INIT_LOG_COLUMN, HMI_OLED_INIT_LINE_DS3231);
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
	SSD1306_GotoXY(HMI_OLED_INIT_LOG_COLUMN, HMI_OLED_INIT_LINE_BMP280);
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
	SSD1306_GotoXY(HMI_OLED_INIT_LOG_COLUMN, HMI_OLED_INIT_LINE_MPU6050);
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
 * @brief       display the DS18B20 status
 * 
 * @param       HW_init 
 * ************************************************************* **/
void HMI_OLED_display_init_log_temp(HW_status_t HW_init)
{
	SSD1306_GotoXY(HMI_OLED_INIT_LOG_COLUMN, HMI_OLED_INIT_LINE_DS18B20);
	if(HW_init.DS18B20 == HAL_OK)
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
	SSD1306_Clear();
	SSD1306_GotoXY(0, HMI_OLED_DATA_LINE_HEADER);
	SSD1306_Puts("==== DATA LOG ====", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, HMI_OLED_DATA_LINE_TIME);
	SSD1306_Puts("time  : ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, HMI_OLED_DATA_LINE_PRESS);
	SSD1306_Puts("press : ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, HMI_OLED_DATA_LINE_ANGLEX);
  	SSD1306_Puts("angXY : ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, HMI_OLED_LINE_5);
  	SSD1306_Puts("temp  : ", &Font_7x10, SSD1306_COLOR_WHITE);

	//SSD1306_GotoXY(0, HMI_OLED_DATA_LINE_ANGLEY);
  	//SSD1306_Puts("angleY : ", &Font_7x10, SSD1306_COLOR_WHITE);

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
	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN_DOT1, HMI_OLED_DATA_LINE_TIME);
	SSD1306_Puts(":", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN_DOT2, HMI_OLED_DATA_LINE_TIME);
	SSD1306_Puts(":", &Font_7x10, SSD1306_COLOR_WHITE);


   	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN_HOUR, HMI_OLED_DATA_LINE_TIME);
   	SSD1306_Puts_Num16bits(TIME.Hour, 3, &Font_7x10, SSD1306_COLOR_WHITE);

   	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN_MIN, HMI_OLED_DATA_LINE_TIME);
   	SSD1306_Puts_Num16bits(TIME.Min, 3, &Font_7x10, SSD1306_COLOR_WHITE);

   	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN_SEC, HMI_OLED_DATA_LINE_TIME);
   	SSD1306_Puts_Num16bits(TIME.Sec, 3, &Font_7x10, SSD1306_COLOR_WHITE);
}


/** ************************************************************* *
 * @brief       display the current pressure
 * 
 * ************************************************************* **/
void HMI_OLED_display_data_log_press(BMP280_t PRESS)
{
	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_DATA_LINE_PRESS);
	SSD1306_Puts("        ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_DATA_LINE_PRESS);
	SSD1306_Puts_float((PRESS.pressure/100), 9, &Font_7x10, SSD1306_COLOR_WHITE);
}


/** ************************************************************* *
 * @brief       display the current angle
 * 
 * ************************************************************* **/
void HMI_OLED_display_data_log_angle(MPU6050_t ANGLE)
{
	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_DATA_LINE_ANGLEX);
	SSD1306_Puts("        ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_DATA_LINE_ANGLEX);
	SSD1306_Puts_float(ANGLE.KalmanAngleX, 6, &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(13*7, HMI_OLED_DATA_LINE_ANGLEX);
	SSD1306_Puts_float(ANGLE.KalmanAngleY, 6, &Font_7x10, SSD1306_COLOR_WHITE);


	//SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_DATA_LINE_ANGLEY);
	//SSD1306_Puts("        ", &Font_7x10, SSD1306_COLOR_WHITE);

	//SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_DATA_LINE_ANGLEY);
	//SSD1306_Puts_float(ANGLE.KalmanAngleY, 4, &Font_7x10, SSD1306_COLOR_WHITE);
}

/** ************************************************************* *
 * @brief       
 * 
 * @param       TEMP 
 * ************************************************************* **/
void HMI_OLED_display_data_log_temp(DS18B20_t TEMP)
{
	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_LINE_5);
	SSD1306_Puts("        ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_DATA_LOG_COLUMN, HMI_OLED_LINE_5);
	SSD1306_Puts_float(TEMP.temperature, 6, &Font_7x10, SSD1306_COLOR_WHITE);
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

/** ************************************************************* *
 * @brief       display the status menu
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t HMI_OLED_display_status(void)
{
	SSD1306_Clear();
	SSD1306_GotoXY(0, HMI_OLED_STATUS_LINE_HEADER);
	SSD1306_Puts("===== STATUS =====", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, HMI_OLED_STATUS_LINE_PHASE);
	SSD1306_Puts("phase   : ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, HMI_OLED_STATUS_LINE_JACK);
	SSD1306_Puts("jack    : ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, HMI_OLED_STATUS_LINE_MSG_ID);
	SSD1306_Puts("msg ID  : ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, HMI_OLED_STATUS_LINE_ERRORS);
	SSD1306_Puts("nb errs : ", &Font_7x10, SSD1306_COLOR_WHITE);

	/* update */
	if(SSD1306_UpdateScreen()) return HAL_ERROR;

	return HAL_OK;
}

/** ************************************************************* *
 * @brief       display the current phase
 * 
 * ************************************************************* **/
void HMI_OLED_display_status_phase(void)
{
	SSD1306_GotoXY(HMI_OLED_STATUS_COLUMN, HMI_OLED_STATUS_LINE_PHASE);
	SSD1306_Puts("        ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_STATUS_COLUMN, HMI_OLED_STATUS_LINE_PHASE);
	switch (phase)
	{
	case PHASE_WAIT: 	SSD1306_Puts("wait", &Font_7x10, SSD1306_COLOR_WHITE); 		break;
	case PHASE_ASCEND: 	SSD1306_Puts("ascend", &Font_7x10, SSD1306_COLOR_WHITE);	break;
	case PHASE_DEPLOY: 	SSD1306_Puts("apogee", &Font_7x10, SSD1306_COLOR_WHITE); 	break;
	case PHASE_DESCEND: SSD1306_Puts("descend", &Font_7x10, SSD1306_COLOR_WHITE); 	break;
	case PHASE_LANDED: 	SSD1306_Puts("landed", &Font_7x10, SSD1306_COLOR_WHITE); 	break;
	default:																		break;
	}
}

void HMI_OLED_display_status_jack(void)
{
	SSD1306_GotoXY(HMI_OLED_STATUS_COLUMN, HMI_OLED_STATUS_LINE_JACK);
	SSD1306_Puts("          ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_STATUS_COLUMN, HMI_OLED_STATUS_LINE_JACK);
	switch (jack)
	{
	case JACK_PLUGGED:		SSD1306_Puts("plugged", &Font_7x10, SSD1306_COLOR_WHITE); 	break;
	case JACK_UNPLUGGED: 	SSD1306_Puts("unplugged", &Font_7x10, SSD1306_COLOR_WHITE);	break;
	default:																			break;
	}
}

void HMI_OLED_display_status_msg_id(void)
{
	SSD1306_GotoXY(HMI_OLED_STATUS_COLUMN, HMI_OLED_STATUS_LINE_MSG_ID);
	SSD1306_Puts("          ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_STATUS_COLUMN, HMI_OLED_STATUS_LINE_MSG_ID);
	SSD1306_Puts(msg_id, &Font_7x10, SSD1306_COLOR_WHITE);

}

void HMI_OLED_display_status_errors_number(void)
{
	SSD1306_GotoXY(HMI_OLED_STATUS_COLUMN, HMI_OLED_STATUS_LINE_ERRORS);
	SSD1306_Puts("        ", &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(HMI_OLED_STATUS_COLUMN, HMI_OLED_STATUS_LINE_ERRORS);
	SSD1306_Puts_Num16bits(err_counter, 9, &Font_7x10, SSD1306_COLOR_WHITE);
}
