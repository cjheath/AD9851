/*
 * Library for the AD9851 DDS chip
 *
 * Connect:
 * SPI CLK (Uno Pin 13) to AD9851 W_CLK
 * SPI MOSI (Uno Pin 11) to AD9851 DATA (D7)
 * Your chosen CS pin to AD9851 FQ_UD (device select)
 * Your chosen Reset pin to AD9851 ResetPin (device reset)
 *
 * To calibrate, use the default calibration value (10MHz)
 * and measure the generated frequency with a frequency counter.
 * Then provide that value as the calibration constant instead.
 */
#ifndef _AD9851_h_
#define _AD9851_h_

#include "Arduino.h"
#include <SPI.h>

#if ARDUINO < 10600
#error "Arduino 1.6.0 or later (SPI library) is required"
#endif

template <
	uint8_t ResetPin,		// Reset pin (active = high)
	uint8_t FQ_UDPin,		// Data Load pin (active = pulse high)
	unsigned long calibration = 10000000,	// Use your actual frequency when set to 10MHz
	long SPIRate = 2000000,		// 2MBit/s
	uint8_t SPIClkPin = 13,		// Note: do not change the SPI pins, they're currently ignored.
	uint8_t SPIMOSIPin = 11
>
class AD9851 {
public:
  AD9851()
  {
    pinMode(ResetPin, OUTPUT);	// Ensure we can reset the AD9851
    pinMode(FQ_UDPin, OUTPUT);	// This control signal applies the loaded values
    // Ensure that the SPI device is initialised
    // "setting SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high"
    SPI.begin();

    reset();
  }

  void reset()
  {
    pulse(ResetPin);		// (minimum 5 cycles of the 30MHz clock)
    pulse(SPIClkPin);		// Enable serial loading mode:
    pulse(FQ_UDPin);

    // To avoid possible false configuration settings, immediately set the frequency
    setFrequency(1);
  }

  void setFrequency(unsigned long freq)
  {
    unsigned long tuning_word = freq * 10000000ULL / calibration * 4294967296ULL / 180000000ULL;

    SPI.beginTransaction(SPISettings(SPIRate, LSBFIRST, SPI_MODE0));
    for (int b = 0; b < 4; b++, tuning_word >>= 8)
	    SPI.transfer(tuning_word&0xFF); 
    // The last byte contains configuration settings including phase:
    SPI.transfer(0x01);

    pulse(FQ_UDPin);		// Transfer the 40-bit control word into the DDS core
    SPI.endTransaction();
  }

protected:
  void pulse(uint8_t pin)
  {
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);
  }
};

#endif  /* _AD9851_h_ */
