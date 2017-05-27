/*
 * A Sweep generator to demonstrate the AD9851.
 * 
 * Note the calibration constant below; this is for my board.
 * For a very high step rate, disable the serial printing.
 * 
 * You'll also need to change the definition of AD9851_CS_PIN
 * to reflect the way you have your module wired up.
 */
#include <AD9851.h>
#include <SPI.h>

#define CENTRE        10000000UL  /* Hz */
#define DEVIATION     500000      /* Deviation in Hz (above and below) */
#define STEP          100000      /* Step in Hz. Set to 0 for constant frequency */
#define RATE          5000        /* ms between steps */

#define CALIBRATION   9999941     /* Set this to the frequency when set to 10MHz */

#define AD9851_CS_PIN  2
// And MOSI=11, SCK=13

AD9851 dds(AD9851_CS_PIN);
unsigned long next_update;        /* The millis() value for the next update */

void setup() {
  Serial.begin(38400);
  while (!Serial && (millis() <= 1000));
  next_update = millis();
}

void loop() {
  static  unsigned long freq = CENTRE;

  dds.setFrequency(freq*10000000ULL/CALIBRATION);
  //dds.setFrequency(freq);
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
