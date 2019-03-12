//import the necessary libraries
#include "FastLED.h"
#include "IRremote.h"
#include <EEPROM.h>

//define some variables
#define LEDPIN     8
#define RECV_PIN  11
#define LED_TYPE     WS2811
#define NUM_LEDS    133

int BRIGHTNESS = 50;
CRGB leds[NUM_LEDS];

boolean onoff;

long int lastMode = 0;
//set up IR receiver information
IRrecv irrecv(RECV_PIN);

void setup()
{
  //begin serial communication
  Serial.begin(9600);
  //sanity delay
  delay(3000);
  //start the receiver
  irrecv.enableIRIn();

  // load settings from memory
  loadSettings();

  //set up LED strip information
  initLedstrip();
}

void loop() {
  readIr();
  delay(120);
}

void loadSettings() {
  int readBrightness = EEPROM.read(1);

  lastMode = EEPROM.read(0);
  Serial.print("Loading mode: ");
  Serial.println(lastMode);

  if (readBrightness) {
    BRIGHTNESS = readBrightness;
  }
}

void initLedstrip() {
  FastLED.addLeds<LED_TYPE, LEDPIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  onoff = false;
}
