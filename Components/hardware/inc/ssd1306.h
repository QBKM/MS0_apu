/** ************************************************************* *
 * @file        ssd1306.h
 * @brief       
 * 
 * @date        2021-04-24
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __SSD1306_H__
#define __SSD1306_H__


/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "fonts.h"
#include "stdint.h"

/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef enum {
	SSD1306_COLOR_BLACK = 0x00, /* Black color, no pixel */
	SSD1306_COLOR_WHITE = 0x01  /* White color,  1 pixel */
} SSD1306_COLOR_t;

/* ------------------------------------------------------------- --
   Functions
-- ------------------------------------------------------------- */
/* init */
uint8_t SSD1306_Init(void);

/* screen action */
uint8_t SSD1306_UpdateScreen(void);
uint8_t SSD1306_Clear (void);
void 	SSD1306_Fill(SSD1306_COLOR_t Color);

/* cursor */
void 	SSD1306_GotoXY(uint16_t x, uint16_t y);

/* draw  */
void 	SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
void 	SSD1306_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color);

/* print characters */
uint8_t SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color);
uint8_t SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color);
uint8_t SSD1306_Puts_Num16bits(int16_t number, FontDef_t* Font, SSD1306_COLOR_t color);
uint8_t SSD1306_Puts_float(float number, FontDef_t* Font, SSD1306_COLOR_t color);

/* scroll */
uint8_t SSD1306_ScrollRight(uint8_t start_row, uint8_t end_row);
uint8_t SSD1306_ScrollLeft(uint8_t start_row, uint8_t end_row);
uint8_t SSD1306_Stopscroll(void);

/* i2c */
uint8_t SSD1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);



#endif /* __SSD1306_H__ */
/* ------------------------------------------------------------- --
   End of file
-- ------------------------------------------------------------- */
