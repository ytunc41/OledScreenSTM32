/*
 * OLED.h
 *
 *  Created on: Sep 9, 2020
 *      Author: yusuf
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "stm32f1xx_hal.h"
#include "stdio.h"

//#define OLED_128x32
#define OLED_128x64
//#define OLED_128x128


#define OLED_WIDTH 		128

#if defined(OLED_128x32)
#define OLED_HEIGHT 	32
#elif defined(OLED_128x64)
#define OLED_HEIGHT 	64
#elif defined(OLED_128x128)
#define OLED_HEIGHT 	128
#endif

#define OLED_PAGESIZE	(OLED_HEIGHT / 8)

#define OLED_COMMAND		0X00
#define OLED_DATA			0X40

#define OLED_ADDR			0x78
#define OLED_ID				0X3C
#define OLED_SETCONTRAST	0x81
#define OLED_RESET			0x7F
#define OLED_SETDISPLAY		0xAF
#define OLED_SETMUXRATIO	0xA8

#define OLED_BUFFER_SIZE   OLED_WIDTH * OLED_PAGESIZE
uint8_t OLED_Buffer[OLED_BUFFER_SIZE];

typedef struct
{
	uint8_t Buffer[OLED_BUFFER_SIZE];
	uint8_t CursorX;
	uint8_t CursorY;
}OLED_TypeDef;
OLED_TypeDef OLED;

typedef enum
{
	NOKTASAL = 0,
	CIZGISEL,
}RectType;

void oled_main(void);
void oled_init(void);
void oled_write(uint8_t addr, uint8_t txdata, uint8_t length);
uint8_t* oled_read(uint8_t addr, uint8_t length);
void oled_Fill(uint8_t black_or_white);
void oled_UpdateScreen(void);
void oled_writeData(uint8_t addr, uint8_t* txdata, uint8_t data_size);
void oled_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
void oled_TestRectangle(void);
void oled_Rectangle(RectType RectType, uint8_t speed);


/* ifdef_example.inp */
#define TEST1  100 // define a variable.
#define TEST2
/* process the input based on the what was defined above */
#ifdef TEST1
//TEST1 (test1) is defined
#ifdef TEST2   // another check (nested within the first)
//The Value TEST2 (test2) is defined
#else          // else for #ifdef TEST2
TEST1 (test1) is defined and TEST2 is not defined
#endif        // #endif for #ifdef TEST2
#elif TEST2
//TEST1 is NOT Defined,  and TEST2 (test2) is defined
#else
//TEST1 and TEST2 are not defined
#endif




#endif /* INC_OLED_H_ */
