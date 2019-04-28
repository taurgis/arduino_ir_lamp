// define the LED variables
#define LEDPIN     8

#define LED_TYPE     WS2811
#define NUM_LEDS    133

// All leds in the strip as a FastLED object.
CRGB leds[NUM_LEDS];
CRGB endclr;
CRGB midclr;

// The main state of the lamp (true = on, false = off)
boolean onoff;

/**
   Initialize the LED strip as a typical LED strip with type WS2811.

   Always start the lamp with color "Black" to turn it off.
*/
void initLedstrip() {
  //sanity delay
  delay(3000);
  FastLED.addLeds<LED_TYPE, LEDPIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  if (LAST_MODE > 0 && LAST_MODE < 13) {
    onoff = true;
    setLampState(LAST_MODE);
  } else {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    onoff = false;

  }

  FastLED.show();

}

/**
   Sets the lamp state based on the received IR code.

   @param {long int} result - The received IR code
*/
void setLampState(long int result) {
  FastLED.delay(1000 / 120);

  if (result == 16761405) {
    onoff = !onoff;
    if (onoff) {
      if (LAST_MODE > 0 && LAST_MODE < 13) {
        result = LAST_MODE;
        Serial.print("Overwriting mode: ");
        Serial.println(result);
      } else {
        fill_solid(leds, NUM_LEDS, CRGB::White);
        FastLED.show();
      }
    } else {
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      EEPROM.write(0, 255);
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

    if ((result != 16769055) && (result != 16754775) && (result != 16761405) && (result > 12) && (shortResult != LAST_MODE)) {
      Serial.print("Saving mode: ");
      Serial.println(shortResult);

      LAST_MODE = shortResult;
      EEPROM.write(0, shortResult);
    }
  }
}
