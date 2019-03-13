//import the necessary libraries
#include "FastLED.h"
#include "IRremote.h"
#include <EEPROM.h>

// define the LED variables
#define LEDPIN     8
#define RECV_PIN  11
#define LED_TYPE     WS2811
#define NUM_LEDS    133

// The starting brightness of the lamp
int BRIGHTNESS = 50;

// All leds in the strip as a FastLED object.
CRGB leds[NUM_LEDS];

// The main state of the lamp (true = on, false = off)
boolean onoff;

// The main variable to store the last set mode of the lamp (which color mode)
long int lastMode = 0;

//set up IR receiver information
IRrecv irrecv(RECV_PIN);


/**
 * The main setup function.
 */
void setup()
{
  //begin serial communication
  Serial.begin(9600);
  
  //start the receiver
  irrecv.enableIRIn();

  // load settings from memory
  loadSettings();

  //set up LED strip information
  initLedstrip();
}

/**
 * The loop function.
 */
void loop() {
  readIr();
  delay(120);
}


/**
 * Loads the settings from EEPROM. We need to make sure we don't
 * save to many times to this memory as it is limited to 100.000
 * writes (at the minimum, if we're lucky we get more).
 * 
 * Currently the following settings are saved:
 *   - The brightness setting
 *   - The last selected mode (color)
 */
void loadSettings() {
  int readBrightness = EEPROM.read(1);

  lastMode = EEPROM.read(0);
  Serial.print("Loading mode: ");
  Serial.println(lastMode);

  if (readBrightness) {
    BRIGHTNESS = readBrightness;
  }
}

/**
 * Initialize the LED strip as a typical LED strip with type WS2811.
 * 
 * Always start the lamp with color "Black" to turn it off.
 */
void initLedstrip() {
  //sanity delay
  delay(3000);
  FastLED.addLeds<LED_TYPE, LEDPIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  onoff = false;
}
