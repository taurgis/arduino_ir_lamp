//import the necessary libraries
#include "FastLED.h"
#include "IRremote.h"
#include <EEPROM.h>

//define some variables
#define LEDPIN     8
#define RECV_PIN  11
#define LED_TYPE     WS2811
#define NUM_LEDS    133
#define rep 4294967295

long int lastvalue;

int BRIGHTNESS = 50;
CRGB leds[NUM_LEDS];
CRGB endclr;
CRGB midclr;

boolean onoff;
unsigned long lastCommand = millis();
long int lastMode = 0;
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
  FastLED.addLeds<LED_TYPE, LEDPIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  int readBrightness = EEPROM.read(1);

  lastMode = EEPROM.read(0);
  Serial.print("Loading mode: ");
  Serial.println(lastMode);

  if (readBrightness) {
    BRIGHTNESS = readBrightness;
  }

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
          if (lastMode > 0 && lastMode < 13) {
            result = lastMode;
            Serial.print("Overwriting mode: ");
            Serial.println(result);
          } else {
            fill_solid(leds, NUM_LEDS, CRGB::White);
            FastLED.show();
          }
        } else {
          fill_solid(leds, NUM_LEDS, CRGB::Black);
          FastLED.show();
        }
      }

      if (onoff) {
        int shortResult;
        //vol+ = brightness up
        if (result == 16754775) {
          if (BRIGHTNESS < 250) {
            BRIGHTNESS += 10;
          }
          FastLED.setBrightness(BRIGHTNESS);
          EEPROM.write(1, BRIGHTNESS);
          FastLED.show();
        }

        //vol- = brightness down
        if (result == 16769055) {
          if (BRIGHTNESS > 10) {
            BRIGHTNESS -= 10;
          }
          FastLED.setBrightness(BRIGHTNESS);
          EEPROM.write(1, BRIGHTNESS);
          FastLED.show();
        }



        //play/pause = cycles through colours
        if (result == 16736925 || result == 0) {
          uint8_t i = beatsin8(130, 0, 150);
          fill_solid(leds, NUM_LEDS, CHSV(i, 255, 150));
          FastLED.show();

          shortResult = 0;
        }

        if (result == 16769565 || result == 1) {
          uint8_t i = beatsin8(130, 0, 150);
          uint8_t j = beatsin8(180, 0, 150);
          fill_gradient (leds, NUM_LEDS,  CHSV(i, 255, 150), CHSV(j, 255, 150), SHORTEST_HUES);
          FastLED.show();

          shortResult = 1;
        }


        if (result == 16753245 || result == 2) {
          fill_gradient (leds, NUM_LEDS,  CHSV(beatsin8(180, 0, 150), 255, 150), CHSV( beatsin8(130, 0, 150), 255, 150), LONGEST_HUES);
          FastLED.show();

          shortResult = 2;
        }

        if (result == 16738455 || result == 3) {
          fill_solid(leds, NUM_LEDS, CRGB::White);
          FastLED.show();

          shortResult = 3;
        }

        if (result == 16724175 || result == 4) {
          fill_solid(leds, NUM_LEDS, CRGB::Blue);
          FastLED.show();

          shortResult = 4;
        }

        // 2 = sunset (orange & blue)
        if (result == 16718055 || result == 5) {
          fill_solid(leds, NUM_LEDS, CRGB::Green);
          FastLED.setBrightness(BRIGHTNESS);
          FastLED.show();

          shortResult = 5;
        }

        if (result == 16743045 || result == 6) {
          fill_solid(leds, NUM_LEDS, CRGB::Red);
          FastLED.show();

          shortResult = 6;
        }

        if (result == 16716015 || result == 7) {
          fill_solid(leds, NUM_LEDS, CRGB::Pink);
          FastLED.setBrightness(BRIGHTNESS);
          FastLED.show();

          shortResult = 7;
        }

        if (result == 16726215 || result == 8) {
          fill_solid(leds, NUM_LEDS, CRGB::Purple);
          FastLED.show();

          shortResult = 8;
        }

        if (result == 16734885 || result == 9) {
          fill_solid(leds, NUM_LEDS, CRGB::LightBlue);
          FastLED.show();
          shortResult = 9;
        }

        if (result == 16728765 || result == 10) {
          fill_solid(leds, NUM_LEDS, CRGB::FairyLight);
          FastLED.show();
          shortResult = 10;
        }


        if (result == 16730805 || result == 11) {
          fill_solid(leds, NUM_LEDS, CRGB::ForestGreen);
          FastLED.show();
          shortResult = 11;
        }

        if (result == 16732845 || result == 12) {
          fill_solid(leds, NUM_LEDS, CRGB::FairyLightNCC );
          FastLED.show();
          shortResult = 12;
        }

        if ((result != 16769055) && (result != 16754775) && (result != 16761405) && (result > 12)) {
          Serial.print("Saving mode: ");
          Serial.println(shortResult);
          lastMode = shortResult;
          EEPROM.write(0, shortResult);
        }
      }

      irrecv.resume(); // Receive the next value
    }
  }

  delay(75);
}

//<——————————————————————OTHER FUNCTIONS————————————————————>
