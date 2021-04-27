/** ************************************************************* *
 * @file        fonts.h
 * @brief       
 * 
 * @date        2021-04-27
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __FONTS_H__
#define __FONTS_H__

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "string.h"
#include "stdint.h"

/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef struct {
	uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} FontDef_t;

typedef struct {
	uint16_t Length;      /*!< String length in units of pixels */
	uint16_t Height;      /*!< String height in units of pixels */
} FONTS_SIZE_t;

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
extern FontDef_t Font_7x10;


#endif
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */