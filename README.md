# AD9851 Direct Digital Synthesis Arduino Library

AD9851 is a library for controlling the AD9851 chip for direct digital sythesis of radio frequency signals

![AD9851 Example Program](doc/AD9851.jpg)

## Setup Functions

First, create an instance of the library for your module.
You must provide the digital pin number to be used for chip select.
The normal MISO, MOSI and SCK pins will be used automatically.

    #define AD9851_CS_PIN  2
    AD9851 dds(CS_PIN);

## Setting the frequency

The setFrequency() function tells the module what frequency to generate.

      dds.setFrequency(7140000UL);
