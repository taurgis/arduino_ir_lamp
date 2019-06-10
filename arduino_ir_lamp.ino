//import the necessary libraries
#include "FastLED.h"
#include "IRremote.h"
#include <EEPROM.h>

// The starting brightness of the lamp
int BRIGHTNESS = 50;

// The main variable to store the last set mode of the lamp (which color mode)
long int LAST_MODE = 255;

/**
   The main setup function.
*/
void setup()
{
  //begin serial communication
  Serial.begin(9600);

  //start the receiver
  initIR();

  // load settings from memory
  loadSettings();

  //set up LED strip information
  initLedstrip();
}

/**
   The loop function.
*/
void loop() {
  readIR();

  delay(120);
}


/**
   Loads the settings from EEPROM. We need to make sure we don't
   save to many times to this memory as it is limited to 100.000
   writes (at the minimum, if we're lucky we get more).

   Currently the following settings are saved:
     - The brightness setting
     - The last selected mode (color)
*/
void loadSettings() {
  LAST_MODE = EEPROM.read(0);
  int brightnessFromMemory = EEPROM.read(1);

  Serial.print("Loading mode: ");
  Serial.print(LAST_MODE);

  if (brightnessFromMemory) {
    BRIGHTNESS = brightnessFromMemory;
  }

  Serial.print(" with brightness ");
  Serial.println(BRIGHTNESS);
}
