//import the necessary libraries
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include "IRremote.h"

//define some variables
#define LEDPIN     6
#define RECV_PIN  2
#define LED_TYPE     NEOPIXEL
#define NUM_LEDS    133
#define rep 4294967295

long int lastvalue;

int BRIGHTNESS = 50;
CRGB leds[NUM_LEDS];
CRGB endclr;
CRGB midclr;

boolean onoff;
unsigned long lastCommand = millis();

//set up IR receiver information
IRrecv irrecv(RECV_PIN);
decode_results results;

//<————————————————————————————SETUP————————————————————————————>
void setup()
{
  //begin serial communication
  Serial.begin(9600);
  //sanity delay
  delay(3000);
  //start the receiver
  irrecv.enableIRIn();

  //set up LED strip information
  FastLED.addLeds<LED_TYPE, LEDPIN>(leds, NUM_LEDS).setCorrection(UncorrectedColor );
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  onoff = false;
}


//<—————————————————————————LOOP———————————————————————————————>
void loop() {
  if (irrecv.decode(&results)) {
    if (( millis() - lastCommand) > 350) {
      lastCommand = millis();
      FastLED.delay(1000 / 120);
      long int result = results.value;

      if (results.value == rep) {
        result = lastvalue;
      }

      lastvalue = result;

      Serial.println(result);

      //<——————————————————BUTTON FUNCTIONS—————————————————————————>
      //power button = plain white
      if (result == 16761405) {
        onoff = !onoff;
        if (onoff) {
          fill_solid(leds, NUM_LEDS, CRGB::AliceBlue);
        } else {
          fill_solid(leds, NUM_LEDS, CRGB::Black);
        }

        FastLED.show();
      }

      //vol+ = brightness up
      if (result == 16754775) {
        if (BRIGHTNESS < 250) {
          BRIGHTNESS += 10;
        }
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.show();
      }

      //vol- = brightness down
      if (result == 16769055) {
        if (BRIGHTNESS > 10) {
          BRIGHTNESS -= 10;
        }
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.show();
      }

      //play/pause = cycles through colours
      if (result == 16736925) {
        uint8_t i = beatsin8(130, 0, 150);
        fill_solid(leds, NUM_LEDS, CHSV(i, 255, 150));
        FastLED.show();
      }

      if (result == 16769565) {
        uint8_t i = beatsin8(130, 0, 150);
        uint8_t j = beatsin8(180, 0, 150);
        fill_gradient (leds, NUM_LEDS,  CHSV(i, 255, 150), CHSV(j, 255, 150), SHORTEST_HUES);
        FastLED.show();
      }


      if (result == 16753245) {

        fill_gradient (leds, NUM_LEDS,  CHSV(beatsin8(180, 0, 150), 255, 150), CHSV( beatsin8(130, 0, 150), 255, 150), LONGEST_HUES);
        FastLED.show();
      }

      if (result == 16738455) {
        fill_solid(leds, NUM_LEDS, CRGB::White);
        FastLED.show();
      }

      if (result == 16724175) {
        fill_solid(leds, NUM_LEDS, CRGB::Blue);
        FastLED.show();
      }

      // 2 = sunset (orange & blue)
      if (result == 16718055) {
        fill_solid(leds, NUM_LEDS, CRGB::Green);
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.show();
      }

      if (result == 16743045) {
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
      }

      if (result == 16716015) {
        fill_solid(leds, NUM_LEDS, CRGB::Pink);
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.show();
      }

      if (result == 16726215) {
        fill_solid(leds, NUM_LEDS, CRGB::Purple);
        FastLED.show();
      }

      // 6 = rain
      if (result == 16734885) {
        fill_solid(leds, NUM_LEDS, CRGB::LightBlue);
      }

      // 7 = acid rain
      if (result == 16728765) {
        fill_solid(leds, NUM_LEDS, CRGB::FairyLight);
      }

      // 8 = snow
      if (result == 16730805) {
        fill_solid(leds, NUM_LEDS, CRGB::ForestGreen);
      }

      // 9 = storm
      if (result == 16732845) {
        fill_solid(leds, NUM_LEDS, CRGB::FairyLightNCC );
      }

      irrecv.resume(); // Receive the next value
    }
  }

  delay(75);
}

//<——————————————————————OTHER FUNCTIONS————————————————————>
