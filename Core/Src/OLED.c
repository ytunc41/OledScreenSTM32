/*
 * OLED.c
 *
 *  Created on: Sep 9, 2020
 *      Author: yusuf
 */

#include "OLED.h"
extern I2C_HandleTypeDef hi2c2;

void oled_init(void)
{
	HAL_Delay(100);

#if defined(OLED_128x32)

#elif defined(OLED_128x64)

#elif defined(OLED_128x128)

#endif

	// Set Display Off
	oled_write(OLED_COMMAND, 0xAE, 1);

	oled_write(OLED_COMMAND, 0x20, 1);
	oled_write(OLED_COMMAND, 0x00, 1);

	oled_write(OLED_COMMAND, 0xB0, 1);

	oled_write(OLED_COMMAND, 0xC8, 1);

	oled_write(OLED_COMMAND, 0x00, 1);
	oled_write(OLED_COMMAND, 0x10, 1);

	oled_write(OLED_COMMAND, 0x40, 1);

	oled_write(OLED_COMMAND, 0x81, 1);
	oled_write(OLED_COMMAND, 0xFF, 1);

	oled_write(OLED_COMMAND, 0xA1, 1);

	oled_write(OLED_COMMAND, 0xA6, 1);	// set normal color

#if defined(OLED_128x32)
	oled_write(OLED_COMMAND, 0xA8, 1);
#elif defined(OLED_128x64)
	oled_write(OLED_COMMAND, 0xA8, 1);
#elif defined(OLED_128x128)
	oled_write(OLED_COMMAND, 0xFF, 1);
#endif

#if defined(OLED_128x32)
	oled_write(OLED_COMMAND, 0x1F, 1);
#else
	oled_write(OLED_COMMAND, 0x3F, 1);
#endif

	oled_write(OLED_COMMAND, 0xA4, 1);

	oled_write(OLED_COMMAND, 0xD3, 1);
	oled_write(OLED_COMMAND, 0x00, 1);

	oled_write(OLED_COMMAND, 0xD5, 1);
	oled_write(OLED_COMMAND, 0xF0, 1);

	oled_write(OLED_COMMAND, 0xD9, 1);
	oled_write(OLED_COMMAND, 0x22, 1);

	oled_write(OLED_COMMAND, 0xDA, 1);
#if defined(OLED_128x32)
	oled_write(OLED_COMMAND, 0x02, 1);
#else
	oled_write(OLED_COMMAND, 0x12, 1);
#endif

	oled_write(OLED_COMMAND, 0xDB, 1);
	oled_write(OLED_COMMAND, 0x20, 1);

	oled_write(OLED_COMMAND, 0x8D, 1);
	oled_write(OLED_COMMAND, 0x14, 1);

	oled_write(OLED_COMMAND, 0xAF, 1);

	oled_Fill(0);
	oled_UpdateScreen();
	HAL_Delay(1000);

	oled_Fill(1);
	oled_UpdateScreen();
	HAL_Delay(2000);

	oled_Fill(0);
	oled_UpdateScreen();
}

void oled_write(uint8_t addr, uint8_t txdata, uint8_t data_size)
{
	HAL_I2C_Mem_Write(&hi2c2, OLED_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &txdata, data_size, HAL_MAX_DELAY);
}

void oled_writeData(uint8_t addr, uint8_t* txdata, uint8_t data_size)
{
	HAL_I2C_Mem_Write(&hi2c2, OLED_ADDR, addr, I2C_MEMADD_SIZE_8BIT, txdata, data_size, HAL_MAX_DELAY);
}

void oled_Fill(uint8_t black_or_white)
{
    uint8_t x;
    if(black_or_white)
    	x = 0xFF;
    else
    	x = 0x00;
    uint32_t i;
    for(i = 0; i < sizeof(OLED_Buffer); i++)
    	OLED_Buffer[i] = x;
}

void oled_UpdateScreen(void)
{
    for(uint8_t i = 0; i < OLED_HEIGHT/8; i++)
	{
		oled_write(OLED_COMMAND, 0xB0 + i, 1); // Set the current RAM page address.
		oled_write(OLED_COMMAND, 0x00, 1);
		oled_write(OLED_COMMAND, 0x10, 1);
		oled_writeData(OLED_DATA, &OLED_Buffer[OLED_WIDTH*i], OLED_WIDTH);
	}
}

void oled_DrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
	if(x >= OLED_WIDTH || y >= OLED_HEIGHT)
		return;

	// Draw in the right color
	if(color == 1)
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] |= 1 << (y % 8);
	else
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
}

void oled_TestRectangle(void)
{
	oled_Fill(0);

	uint32_t start = HAL_GetTick();
	uint32_t end = start;
	uint8_t x = 0;
	uint8_t y = 0;
	do {
//		oled_DrawPixel(x,y,0);

	  if((y == 0) && (x < 127))
		  x++;
	  else if((x == 127) && (y < 31))
		  y++;
	  else if((y == 31) && (x > 0))
		  x--;
	  else
		  y--;

	  oled_DrawPixel(x,y,1);
	  oled_UpdateScreen();

	  end = HAL_GetTick();
	} while((end - start) < 15900);
}

void oled_Rectangle(RectType RectType, uint8_t speed)
{
	oled_Fill(0);
//	oled_UpdateScreen();
//	HAL_Delay(50);

	uint8_t xMin=0, xMax = OLED_WIDTH - 1;
	uint8_t yMin=0, yMax = OLED_HEIGHT - 1;
	uint8_t x=0,xf=0,y=0,yf=0;
	uint16_t i, iMax = OLED_WIDTH * OLED_HEIGHT;

	for(i=0; i<iMax+1; i++)
	{
		if(i == (2*(xMax-xMin) + 2*(yMax-yMin)))
		{
			if(RectType == CIZGISEL)
			{
				oled_UpdateScreen();
				HAL_Delay(100-speed*5);
			}

			i = 0;
			xMin+=1; xMax-=1;
			yMin+=1; yMax-=1;
			x = ++xf; y = ++yf;
			if((yMax-yMin < 0) || (xMax-xMin < 0))
				break;
		}

		oled_DrawPixel(x,y,1);
		if(RectType == NOKTASAL)
			if (!(i%speed))
				oled_UpdateScreen();

		if((y == yMin) && (x < xMax))
			x++;
		else if((x == xMax) && (y < yMax))
			y++;
		else if((y == yMax) && (x > xMin))
			x--;
		else if((x == xMin) && (y > yMin))
			y--;
	}

	oled_Fill(1);
	oled_UpdateScreen();
}
