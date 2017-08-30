/*
 * A Sweep generator to demonstrate the AD9851.
 * 
 * Note the calibration constant below; this is for my board.
 * For a very high step rate, disable the serial printing.
 * 
 * You'll also need to change the definition of AD9851_FQ_UD_PIN
 * to reflect the way you have your module wired up.
 */
#include <AD9851.h>
#include <SPI.h>

/* Sweep details */
#define CENTRE        10000000UL  /* Hz */
#define DEVIATION     500000      /* Deviation in Hz (above and below) */
#define STEP          100000      /* Step in Hz. Set to 0 for constant frequency */
#define RATE          5000        /* ms between steps */

//#define CALIBRATION   0         /* Parts-per billion to adjust the core clock from the reference */
#define CALIBRATION   5700        /* Calibration for my test module; it runs fast */

#define AD9851_FQ_UD_PIN      2
#define AD9851_RESET_PIN      3
// And MOSI=11, SCK=13

class MyAD9851 : public AD9851<AD9851_RESET_PIN, AD9851_FQ_UD_PIN> {};
MyAD9851 dds;
unsigned long next_update;        /* The millis() value for the next update */

void setup() {
  Serial.begin(38400);
  while (!Serial && (millis() <= 1000));
  next_update = millis();
}

void loop() {
  static  unsigned long freq = CENTRE;

  dds.setClock(CALIBRATION);
  dds.setFrequency(freq);
  Serial.println(freq);

  if ((freq += STEP) > CENTRE+DEVIATION)
    freq = CENTRE-DEVIATION;    // Start again at the bottom of the sweep
  next_update += RATE;
  unsigned long wait = next_update-millis();
  if (wait < 100000)            // Wait until RATE ms have elapsed since the last update
    delay(wait);                // Unless the update took longer than RATE
  else
    next_update = millis()+RATE;  // Resynchronise if possible, things exploded
}
