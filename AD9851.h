/*
 * Library for the AD9851 DDS chip
 *
 * Connect:
 * SPI CLK (Uno Pin 13) to AD9851 W_CLK
 * SPI MOSI (Uno Pin 11) to AD9851 DATA (D7)
 * Your chosen CS pin to AD9851 FQ_UD (device select)
 */

#ifndef _AD9851_h_
#define _AD9851_h_

#include "Arduino.h"
#include <SPI.h>

#if ARDUINO < 10600
#error "Arduino 1.6.0 or later (SPI library) is required"
#endif

class AD9851 {
public:
        AD9851(uint8_t cspin);
        void setFrequency(unsigned long freq);

private:
        uint8_t csPin;
};

#endif  /* _AD9851_h_ */
