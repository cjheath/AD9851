/*
 * Library for the AD9851 DDS chip
 */
#include "AD9851.h"

#define SPI_SETTING     SPISettings(2000000, LSBFIRST, SPI_MODE0)
#define	W_CLK		13 /* The SPI SCK (clock) signal */

AD9851::AD9851(uint8_t cs)
{
  csPin = cs;			// The FQ_UD pin, AD9851 device select
  SPI.begin();
  pinMode(csPin, OUTPUT);
  pinMode(W_CLK, OUTPUT);
  // Enable serial loading mode (Pulse SCK high/low,FQ_UD high/low)
  digitalWrite(W_CLK, HIGH);
  digitalWrite(W_CLK, LOW);
  digitalWrite(csPin, HIGH);
  setFrequency(1);
}

void AD9851::setFrequency(unsigned long freq)
{
  long tuning_word = (freq * 4294967296LL) / 180000000LL;

  SPI.beginTransaction(SPI_SETTING);
  digitalWrite(csPin, LOW);
  for (int b = 0; b < 4; b++, tuning_word >>= 8)
	  SPI.transfer(tuning_word&0xFF); 
  //SPI.transfer(0x09);
  SPI.transfer(0x01);
  digitalWrite(csPin, HIGH);
  digitalWrite(csPin, LOW);
  digitalWrite(csPin, HIGH);
  SPI.endTransaction();
}
