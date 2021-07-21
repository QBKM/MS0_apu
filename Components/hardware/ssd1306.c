/** ************************************************************* *
 * @file        ssd1306.c
 * @brief       
 * 
 * @date        2021-04-24
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/


/* ------------------------------------------------------------- --
   Includes
-- ------------------------------------------------------------- */
#include "ssd1306.h"
#include "i2c.h"
#include "stdio.h"
#include "config_file.h"


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
/* register addresses*/
#define SSD1306_REG_CMD						0x00

/* list of commands */
#define SSD1306_RIGHT_HORIZONTAL_SCROLL		0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL		0x27
#define SSD1306_DEACTIVATE_SCROLL			0x2E 
#define SSD1306_ACTIVATE_SCROLL				0x2F 
#define SSD1306_SET_VERTICAL_SCROLL_AREA	0xA3 

/* I2C timeout*/
#ifndef TIMEOUT_I2C
#define TIMEOUT_I2C 10
#endif


/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef struct
{
	uint16_t CurrentX;
	uint16_t CurrentY;

	uint8_t Initialized;
}SSD1306_t;


/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
SSD1306_t SSD1306;

/* ------------------------------------------------------------- --
   Private prototypes
-- ------------------------------------------------------------- */
uint8_t SSD1306_WriteCommand(uint8_t cmd);


/* ============================================================= ==
   private functions
== ============================================================= */
/** ************************************************************* *
 * @brief       send the commands to the SSD1306
 * 
 * @param       cmd 
 * ************************************************************* **/
uint8_t SSD1306_WriteCommand(uint8_t cmd)
{
	uint8_t data[2] = {SSD1306_REG_CMD, cmd};

	if(HAL_I2C_Master_Transmit(&hi2c1, SSD1306_ADDR, data, 2, TIMEOUT_I2C)) return HAL_ERROR;
	return HAL_OK;
}	


/* ============================================================= ==
   private functions
== ============================================================= */
/** ************************************************************* *
 * @brief       initialize the SSD1306
 * 
 * ************************************************************* **/
uint8_t SSD1306_Init(void)
{
	/* Init LCD */
	if(	SSD1306_WriteCommand(0xAE) 		//display off
	||	SSD1306_WriteCommand(0x20) 		//Set Memory Addressing Mode   
	||	SSD1306_WriteCommand(0x10) 		//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	||	SSD1306_WriteCommand(0xB0) 		//Set Page Start Address for Page Addressing Mode,0-7
	||	SSD1306_WriteCommand(0xC8) 		//Set COM Output Scan Direction
	||	SSD1306_WriteCommand(0x00) 		//---set low column address
	||	SSD1306_WriteCommand(0x10) 		//---set high column address
	||	SSD1306_WriteCommand(0x40) 		//--set start line address
	||	SSD1306_WriteCommand(0x81) 		//--set contrast control register
	||	SSD1306_WriteCommand(0xFF)
	||	SSD1306_WriteCommand(0xA1) 		//--set segment re-map 0 to 127
	||	SSD1306_WriteCommand(0xA6) 		//--set normal display
	||	SSD1306_WriteCommand(0xA8) 		//--set multiplex ratio(1 to 64)
	||	SSD1306_WriteCommand(0x3F) 		//
	||	SSD1306_WriteCommand(0xA4) 		//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	||	SSD1306_WriteCommand(0xD3) 		//-set display offset
	||	SSD1306_WriteCommand(0x00) 		//-not offset
	||	SSD1306_WriteCommand(0xD5) 		//--set display clock divide ratio/oscillator frequency
	||	SSD1306_WriteCommand(0xF0) 		//--set divide ratio
	||	SSD1306_WriteCommand(0xD9) 		//--set pre-charge period
	||	SSD1306_WriteCommand(0x22) 		//
	||	SSD1306_WriteCommand(0xDA) 		//--set com pins hardware configuration
	||	SSD1306_WriteCommand(0x12)
	||	SSD1306_WriteCommand(0xDB) 		//--set vcomh
	||	SSD1306_WriteCommand(0x20) 		//0x20,0.77xVcc
	||	SSD1306_WriteCommand(0x8D) 		//--set DC-DC enable
	||	SSD1306_WriteCommand(0x14) 		//
	||	SSD1306_WriteCommand(0xAF) 		//--turn on SSD1306 panel
	||	SSD1306_WriteCommand(SSD1306_DEACTIVATE_SCROLL))	
	{
		return HAL_ERROR;
	}	

	/* Clear screen */
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	
	/* Update screen */
	if(SSD1306_UpdateScreen()) return HAL_ERROR;
	
	/* Set default values */
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	
	/* Initialized OK */
	SSD1306.Initialized = 1;

	return HAL_OK;
}

/** ************************************************************* *
 * @brief       fill the screen with the arg color
 * 
 * @param       color 
 * ************************************************************* **/
void SSD1306_Fill(SSD1306_COLOR_t color)
{
	/* Set memory */
	if(color == SSD1306_COLOR_BLACK)
	memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

/** ************************************************************* *
 * @brief       update the screen with the screen buffer
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t SSD1306_UpdateScreen(void) {
	uint8_t m;
	
	for (m = 0; m < 8; m++) {
		if(	SSD1306_WriteCommand(0xB0 + m)
		||	SSD1306_WriteCommand(0x00)
		||	SSD1306_WriteCommand(0x10))
		{
			return HAL_ERROR;
		}
		
		if(SSD1306_I2C_WriteMulti(SSD1306_ADDR, 0x40, &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH)) return HAL_ERROR;
	}

	return HAL_OK;
}

/** ************************************************************* *
 * @brief       send multi messages on i2c
 * 
 * @param       address 
 * @param       reg 
 * @param       data 
 * @param       count 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t SSD1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) 
{
	uint8_t dt[256];
	dt[0] = reg;
	uint8_t i;
	for(i = 0; i < count; i++) dt[i+1] = data[i];

	if(HAL_I2C_Master_Transmit(&hi2c1, address, dt, count+1, 10)) return HAL_ERROR;

	return HAL_OK;
}

/** ************************************************************* *
 * @brief       clear the screen
 * 
 * ************************************************************* **/
uint8_t SSD1306_Clear(void)
{
	SSD1306_Fill (SSD1306_COLOR_BLACK);
	if(SSD1306_UpdateScreen()) return HAL_ERROR;

	return HAL_OK;
}

/** ************************************************************* *
 * @brief       move the cursor on the screen
 * 
 * @param       x 
 * @param       y 
 * ************************************************************* **/
void SSD1306_GotoXY(uint16_t x, uint16_t y)
{
	/* Set write pointers */
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

/** ************************************************************* *
 * @brief       draw a pixel at arg
 * 
 * @param       x 
 * @param       y 
 * @param       color 
 * ************************************************************* **/
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color)
{
	if (x < SSD1306_WIDTH || y < SSD1306_HEIGHT)
	{
		/* Set color */
		if (color == SSD1306_COLOR_WHITE) {
			SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
		} else {
			SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
		}
	}
}

/** ************************************************************* *
 * @brief       draw a bitmap
 * 
 * @param       x 
 * @param       y 
 * @param       bitmap 
 * @param       w 
 * @param       h 
 * @param       color 
 * ************************************************************* **/
void SSD1306_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color)
{

    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    for(int16_t j=0; j<h; j++, y++)
    {
        for(int16_t i=0; i<w; i++)
        {
            if(i & 7)
            {
               byte <<= 1;
            }
            else
            {
               byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
            }
            if(byte & 0x80) SSD1306_DrawPixel(x+i, y, color);
        }
    }
}

/** ************************************************************* *
 * @brief       print a character
 * 
 * @param       ch 
 * @param       Font 
 * @param       color 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color) {
	uint32_t i, b, j;
	
	/* Check available space in LCD */
	if (
		SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)
	) {
		/* Error */
		return 0;
	}
	
	/* Go through font */
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t) color);
			} else {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
			}
		}
	}
	
	/* Increase pointer */
	SSD1306.CurrentX += Font->FontWidth;
	
	/* Return character written */
	return ch;
}

/** ************************************************************* *
 * @brief       print a string
 * 
 * @param       str 
 * @param       Font 
 * @param       color 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (SSD1306_Putc(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}
		
		/* Increase string pointer */
		str++;
	}
	
	/* Everything OK, zero should be returned */
	return *str;
}

/** ************************************************************* *
 * @brief       print a number
 * 
 * @param       number 
 * @param       Font 
 * @param       color 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t SSD1306_Puts_Num16bits(int16_t number, uint8_t printsize, FontDef_t* Font, SSD1306_COLOR_t color)
{
	char buffer[10];

	if(number < 10)
	{
		snprintf(buffer, printsize, "0%d", number);	
	}
	else
	{
		snprintf(buffer, printsize, "%d", number);
	}
	
	SSD1306_Puts(buffer, Font, color);

	return *buffer;
}

/** ************************************************************* *
 * @brief       print a number
 * 
 * @param       number 
 * @param       Font 
 * @param       color 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t SSD1306_Puts_float(float number, uint8_t printsize, FontDef_t* Font, SSD1306_COLOR_t color)
{
	char buffer[10];

	snprintf(buffer, printsize, "%f", number);
	SSD1306_Puts(buffer, Font, color);

	return *buffer;
}

///** ************************************************************* *
// * @brief       scroll text to the right
// * 
// * @param       start_row 
// * @param       end_row 
// * ************************************************************* **/
//uint8_t SSD1306_ScrollRight(uint8_t start_row, uint8_t end_row)
//{
//	if(	SSD1306_WriteCommand(SSD1306_RIGHT_HORIZONTAL_SCROLL)	// send 0x26
//	||	SSD1306_WriteCommand(0x00)  							// send dummy
//	||	SSD1306_WriteCommand(start_row)  						// start page address
//	||	SSD1306_WriteCommand(0X00)  							// time interval 5 frames
//	||	SSD1306_WriteCommand(end_row)  							// end page address
//	||	SSD1306_WriteCommand(0X00)
//	||	SSD1306_WriteCommand(0XFF)
//	||	SSD1306_WriteCommand(SSD1306_ACTIVATE_SCROLL)) 			// start scroll
//	{
//		return HAL_ERROR;
//	}
//
//	return HAL_OK;
//}
//
///** ************************************************************* *
// * @brief       scroll text to the left
// * 
// * @param       start_row 
// * @param       end_row 
// * ************************************************************* **/
//uint8_t SSD1306_ScrollLeft(uint8_t start_row, uint8_t end_row)
//{
//	if(	SSD1306_WriteCommand(SSD1306_LEFT_HORIZONTAL_SCROLL)  	// send 0x26
//	||	SSD1306_WriteCommand(0x00)  							// send dummy
//	||	SSD1306_WriteCommand(start_row)  						// start page address
//	||	SSD1306_WriteCommand(0X00)  							// time interval 5 frames
//	||	SSD1306_WriteCommand(end_row)  							// end page address
//	||	SSD1306_WriteCommand(0X00)
//	||	SSD1306_WriteCommand(0XFF)
//	||	SSD1306_WriteCommand(SSD1306_ACTIVATE_SCROLL)) 			// start scroll
//	{
//		return HAL_ERROR;
//	}
//
//	return HAL_OK;
//}

///** ************************************************************* *
// * @brief       stop the scroll
// * 
// * ************************************************************* **/
//uint8_t SSD1306_Stopscroll(void)
//{
//	if(SSD1306_WriteCommand(SSD1306_DEACTIVATE_SCROLL)) return HAL_ERROR;
//
//	return HAL_OK;
//}
