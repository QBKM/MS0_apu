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

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
/* SSD1306 size in pixels */
#define SSD1306_WIDTH            			   128
#define SSD1306_HEIGHT           			   64

/* register addresses*/
#define SSD1306_REG_CMD				            0x00

/* list of commands */
#define SSD1306_RIGHT_HORIZONTAL_SCROLL      0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL       0x27
#define SSD1306_DEACTIVATE_SCROLL            0x2E 
#define SSD1306_ACTIVATE_SCROLL              0x2F 
#define SSD1306_SET_VERTICAL_SCROLL_AREA     0xA3 

/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef enum {
	SSD1306_COLOR_BLACK = 0x00, /* Black color, no pixel */
	SSD1306_COLOR_WHITE = 0x01  /* White color,  1 pixel */
} SSD1306_COLOR_t;

typedef struct
{
	uint16_t CurrentX;
	uint16_t CurrentY;

	uint8_t Initialized;
}SSD1306_t;

/* ------------------------------------------------------------- --
   Functions
-- ------------------------------------------------------------- */
/* init */
void SSD1306_Init(void);

/* screen action */
void SSD1306_UpdateScreen(void);
void SSD1306_Fill(SSD1306_COLOR_t Color);
void SSD1306_Clear (void);

/* cursor */
void SSD1306_GotoXY(uint16_t x, uint16_t y);

/* draw  */
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
uint8_t SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c);
void SSD1306_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color);

/* print characters */
uint8_t SSD1306_Putc(uint8_t ch, FontDef_t* Font, SSD1306_COLOR_t color);
uint8_t SSD1306_Puts(uint8_t* str, FontDef_t* Font, SSD1306_COLOR_t color);
uint8_t SSD1306_Puts_Num16bits(int16_t number, FontDef_t* Font, SSD1306_COLOR_t color);
uint8_t SSD1306_Puts_float(float number, FontDef_t* Font, SSD1306_COLOR_t color);

/* scroll */
void SSD1306_ScrollRight(uint8_t start_row, uint8_t end_row);
void SSD1306_ScrollLeft(uint8_t start_row, uint8_t end_row);
void SSD1306_Stopscroll(void);

/* i2c */
void SSD1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);



#endif /* __SSD1306_H__ */
/* ------------------------------------------------------------- --
   End of file
-- ------------------------------------------------------------- */
