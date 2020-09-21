/*
 * HaberlesmeProtokolu.h
 *
 *  Created on: Sep 15, 2020
 *      Author: yusuf
 */

#ifndef INC_HABERLESMEPROTOKOLU_H_
#define INC_HABERLESMEPROTOKOLU_H_

#include "stm32f1xx_hal.h"

#define TX_BUFFER_BOYUT 512
#define RX_BUFFER_BOYUT 512
#define VERILER_BOYUT	128

enum PaketTuru
{
    BAGLANTI_REQUEST = 0,
    BAGLANTI_OK,

    PROGRAM_REQUEST,
    PROGRAM_OK,

    READ_REQUEST,
    READ_OK
};

enum PaketDurum
{
	SOF1 = 0,
	SOF2,
	PAKET_TURU,
	PAKET_SAYACI,
	PAKET_BOYUTU,
	VERILER,
	EOF1,
	EOF2
};

enum CHECK_STATUS
{
	SOF1_BYTE = 58,
	SOF2_BYTE = 34,
	EOF1_BYTE = 41,
	EOF2_BYTE = 69
};

struct Flag
{
	volatile uint8_t USB_PAKET_HAZIR;

	volatile uint8_t BAGLANTI_REQUEST;
	volatile uint8_t BAGLANTI_OK;
	volatile uint8_t PROGRAM_REQUEST;
	volatile uint8_t PROGRAM_OK;
	volatile uint8_t READ_REQUEST;
	volatile uint8_t READ_OK;

};

struct Hata
{
	volatile uint16_t SOF1_HATA_SAYAC;
	volatile uint16_t SOF2_HATA_SAYAC;
	volatile uint16_t EOF1_HATA_SAYAC;
	volatile uint16_t EOF2_HATA_SAYAC;
};

struct PaketYapisi
{
	uint8_t 	sof1;
	uint8_t 	sof2;
	uint8_t 	paket_turu;
	uint8_t 	paket_sayaci;
	uint8_t 	veri_boyutu;
	uint8_t 	veriler[VERILER_BOYUT];
	uint8_t 	eof1;
	uint8_t 	eof2;
};


typedef struct
{
	struct PaketYapisi 	PaketYapisi;
	struct Flag  		Flag;
	struct Hata			Hata;
	enum   PaketDurum	PaketDurum;
	enum   PaketTuru 	PaketTuru;

	uint8_t rx_buffer[RX_BUFFER_BOYUT];
	uint8_t tx_buffer[TX_BUFFER_BOYUT];

	uint32_t CRC32;
	uint32_t gelen_veri;
}HabPro;

void baglanti_ok_paket_olustur(HabPro* habpro);
void program_ok_paket_olustur(HabPro* habpro);
void USB_PaketGonder(HabPro* habpro_t);
void USB_PaketCoz(HabPro* habpro_t, uint8_t* buffer, uint32_t len);


#endif /* INC_HABERLESMEPROTOKOLU_H_ */
