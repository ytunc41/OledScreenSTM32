/*
 * HaberlesmeProtokolu.c
 *
 *  Created on: Sep 15, 2020
 *      Author: yusuf
 */

#include "oled_main.h"

void baglanti_ok_paket_olustur(HabPro* habpro)
{
	uint8_t index = 0;

	habpro->PaketYapisi.veri_boyutu = index;
	habpro->PaketYapisi.paket_turu = BAGLANTI_OK;
}

void program_ok_paket_olustur(HabPro* habpro)
{
	uint8_t index = 0;

	UINT32_ayir(habpro->PaketYapisi.veriler, &index, habpro->CRC32);

	habpro->PaketYapisi.veri_boyutu = index;
	habpro->PaketYapisi.paket_turu = PROGRAM_OK;
}

void USB_PaketGonder(HabPro* habpro_t)
{
	uint8_t index = 0;
	habpro_t->tx_buffer[index++] = SOF1_BYTE;
	habpro_t->tx_buffer[index++] = SOF2_BYTE;
	habpro_t->tx_buffer[index++] = habpro_t->PaketYapisi.paket_turu;
	habpro_t->tx_buffer[index++] = ++habpro_t->PaketYapisi.paket_sayaci;
	habpro_t->tx_buffer[index++] = habpro_t->PaketYapisi.veri_boyutu;
	for(uint8_t i=0; i < habpro_t->PaketYapisi.veri_boyutu; i++)
		habpro_t->tx_buffer[index++] = habpro_t->PaketYapisi.veriler[i];
	habpro_t->tx_buffer[index++] = EOF1_BYTE;
	habpro_t->tx_buffer[index++] = EOF2_BYTE;

	while(CDC_Transmit_FS(habpro_t->tx_buffer, index) != USBD_OK);
}


void USB_PaketCoz(HabPro* habpro_t, uint8_t* buffer, uint32_t len)
{
	static uint8_t VERILER_SAYAC = 0;

	for(uint8_t i = 0; i < len; i++)
	{
		switch (habpro_t->PaketDurum)
		{
			case SOF1:
			{
				if(buffer[i] == SOF1_BYTE)
				{
					habpro_t->PaketYapisi.sof1 = buffer[i];
					habpro_t->PaketDurum = SOF2;
				}
				else
				{
					habpro_t->Hata.SOF1_HATA_SAYAC++;
				}
				break;
			}

			case SOF2:
			{
				if(buffer[i] == SOF2_BYTE)
				{
					habpro_t->PaketYapisi.sof2 = buffer[i];
					habpro_t->PaketDurum = PAKET_TURU;
				}
				else
				{
					habpro_t->PaketDurum = SOF1;
					habpro_t->Hata.SOF2_HATA_SAYAC++;
				}
				break;
			}

			case PAKET_TURU:
			{
				habpro_t->PaketYapisi.paket_turu = buffer[i];
				habpro_t->PaketTuru = buffer[i];
				habpro_t->PaketDurum = PAKET_SAYACI;
				break;
			}

			case PAKET_SAYACI:
			{
				habpro_t->PaketYapisi.paket_sayaci = buffer[i];
				habpro_t->PaketDurum = PAKET_BOYUTU;
				break;
			}

			case PAKET_BOYUTU:
			{
				habpro_t->PaketYapisi.veri_boyutu = buffer[i];

				if(habpro_t->PaketYapisi.veri_boyutu == 0)
				{
					habpro_t->PaketDurum = EOF1;
					break;
				}
				habpro_t->PaketDurum = VERILER;
				break;
			}

			case VERILER:
			{
				habpro_t->PaketYapisi.veriler[VERILER_SAYAC++] = buffer[i];

				if(VERILER_SAYAC == habpro_t->PaketYapisi.veri_boyutu)
				{
					habpro_t->PaketDurum = EOF1;
					VERILER_SAYAC = 0;
				}
				break;
			}

			case EOF1:
			{
				if(buffer[i] == EOF1_BYTE)
				{
					habpro_t->PaketYapisi.eof1 = buffer[i];
					habpro_t->PaketDurum = EOF2;
				}
				else
				{
					VERILER_SAYAC = 0;
					habpro_t->Hata.EOF1_HATA_SAYAC++;
					habpro_t->PaketDurum = SOF1;
				}
				break;
			}

			case EOF2:
			{
				if(buffer[i] == EOF2_BYTE)
				{
					habpro_t->PaketYapisi.eof2 = buffer[i];
					habpro_t->PaketDurum = SOF1;
					habpro_t->Flag.USB_PAKET_HAZIR = 1;
				}
				else
				{
					VERILER_SAYAC = 0;
					habpro_t->Hata.EOF2_HATA_SAYAC++;
					habpro_t->PaketDurum = SOF1;
				}
				break;
			}

			default:
			{
				habpro_t->PaketDurum = SOF1;
				break;
			}

		}

		if(habpro_t->Flag.USB_PAKET_HAZIR)
		{
			switch(habpro_t->PaketTuru)
			{
				case BAGLANTI_REQUEST:
				{

					habpro_t->Flag.BAGLANTI_REQUEST = 1;
					break;
				}

				case BAGLANTI_OK:
				{

					habpro_t->Flag.BAGLANTI_OK = 1;
					break;
				}

				case PROGRAM_REQUEST:
				{
					static uint8_t pageNum = 0;

					for(uint8_t i = 0; i < OLED_WIDTH; i++)
					{
						OLED_Buffer[i + pageNum * OLED_WIDTH] = habpro_t->PaketYapisi.veriler[i];
						habpro_t->CRC32 += habpro_t->PaketYapisi.veriler[i];
					}

					pageNum++;
					if(pageNum == OLED_PAGESIZE)
					{
						pageNum = 0;
						habpro_t->Flag.PROGRAM_REQUEST = 1;
					}

					break;
				}

				case PROGRAM_OK:
				{

					habpro_t->Flag.PROGRAM_OK = 1;
					break;
				}

				case READ_REQUEST:
				{

					habpro_t->Flag.READ_REQUEST = 1;
					break;
				}

				case READ_OK:
				{

					habpro_t->Flag.READ_OK = 1;
					break;
				}

			}
			habpro_t->Flag.USB_PAKET_HAZIR = 0;
		}
	}

}

