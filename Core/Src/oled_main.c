/*
 * oled_main.c
 *
 *  Created on: Sep 14, 2020
 *      Author: yusuf
 */


#include "oled_main.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
HabPro habpro_t;

void oled_main(void)
{
	oled_init();

	while(1)
	{

		if(habpro_t.Flag.BAGLANTI_REQUEST)
		{
			baglanti_ok_paket_olustur(&habpro_t);
			USB_PaketGonder(&habpro_t);

			habpro_t.Flag.BAGLANTI_REQUEST = 0;
		}

		if(habpro_t.Flag.BAGLANTI_OK)
		{
			habpro_t.Flag.BAGLANTI_OK = 0;
		}

		if(habpro_t.Flag.PROGRAM_REQUEST)
		{
			program_ok_paket_olustur(&habpro_t);
			USB_PaketGonder(&habpro_t);
			habpro_t.CRC32 = 0;

			oled_UpdateScreen();
			HAL_Delay(1000);

			habpro_t.Flag.PROGRAM_REQUEST = 0;
		}

		if(habpro_t.Flag.PROGRAM_OK)
		{

			habpro_t.Flag.PROGRAM_OK = 0;
		}

		if(habpro_t.Flag.READ_REQUEST)
		{

			habpro_t.Flag.READ_REQUEST = 0;
		}

		if(habpro_t.Flag.READ_OK)
		{

			habpro_t.Flag.READ_OK = 0;
		}



	}
}


void USB_ReadCallback(uint8_t* buffer, uint32_t len)
{
	USB_PaketCoz(&habpro_t, buffer, len);
}
