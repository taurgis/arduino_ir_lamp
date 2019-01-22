//import the necessary libraries
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include "IRremote.h"

//define some variables
#define LEDPIN     6
#define RECV_PIN  2
#define LED_TYPE     NEOPIXEL
#define NUM_LEDS    133
#define BRIGHTNESS  50
CRGB leds[NUM_LEDS];
CRGB endclr;
CRGB midclr;

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
  FastLED.setBrightness(BRIGHTNESS);
}
uint8_t gHue = 0;
int lastResult;
//<—————————————————————————LOOP———————————————————————————————>
void loop() {
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;
  }
  if (irrecv.decode(&results)) {
     int result;
    //rotating base colour used by many of the patterns

    FastLED.delay(1000 / 120);
    if(results.value == 4294967295) {
      result = lastResult;
    } else {
      result = results.value;
    }
    
    lastResult = result;
    
    Serial.println(results.value);  // not

    //<——————————————————BUTTON FUNCTIONS—————————————————————————>
    //power button = plain white
    if (results.value == 16761405) {
      fill_solid(leds, NUM_LEDS, CRGB::White);
      FastLED.show();
    }

    //vol+ = brightness up
    if (results.value == 16754775) {
      FastLED.setBrightness(255);
      FastLED.show();
    }

    //vol- = brightness down
    if (results.value == 16769055) {
      fadeLightBy(leds, NUM_LEDS, 30);
      FastLED.show();
    }

    //play/pause = cycles through colours
    if (results.value == 16736925) {
      uint8_t i = beatsin8(130, 0, 150);
      fill_solid(leds, NUM_LEDS, CHSV(i, 255, 150));
      FastLED.show();
    }

    //left = Custom gradient blend
    if (results.value == 0xFD20DF) {
      uint8_t speed = beatsin8(10, 0, 255);
      endclr = blend(CHSV(160, 185, 255), CRGB::Pink, speed);
      midclr = blend(CRGB::Purple, CRGB::Green, speed);

      fill_gradient_RGB(leds, 0, endclr, NUM_LEDS / 2, midclr);
      fill_gradient_RGB(leds, NUM_LEDS / 2 + 1, midclr, NUM_LEDS, endclr);
      FastLED.show();
    }

    //right = Custom solid colour
    if (results.value == 0xFD609F) {
      fill_solid(leds, NUM_LEDS, CRGB::Purple);
      FastLED.show();
    }


    //func/stop = xmas
    if (results.value == 0xFD40BF) {
      uint8_t dot = beatsin8(50, 0, NUM_LEDS);
      leds[dot] = CRGB::Red;
      leds[dot + 1] = CRGB::Green;
      FastLED.show();
    }


    //up = bpm
    if (results.value == 16769565) {
      CRGBPalette16 palette = PartyColors_p;
      uint8_t beat = beatsin8( 70, 64, 255);
      for ( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
      }
      FastLED.show();
    }

    //down = fire
    if (results.value == 0xFD10EF) {
      CRGBPalette16 palette = HeatColors_p;
      uint8_t beat = beatsin8( 50, 64, 255);
      for ( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
      }
      FastLED.show();
    }

    //eq = rainbow
    if (results.value == 0xFDB04F) {
      fill_rainbow( leds, NUM_LEDS - 3, gHue, 7);
      leds[12] = CRGB::Blue;   //adjust the rainbow slightly towards the purple end
      leds[13] = CRGB::Purple;
      leds[14] = CRGB::Purple;
      FastLED.show();
    }

    //st/rept = rainbow w stars
    if (results.value == 0xFD708F) {
      fill_rainbow( leds, NUM_LEDS - 3, gHue, 7);
      leds[12] = CRGB::Blue;
      leds[13] = CRGB::Purple;
      leds[14] = CRGB::Purple;
      addGlitter(70);
      FastLED.show();
    }

    // 0 = day
    if (results.value == 0xFD30CF) {
      uint8_t speed = beatsin8(8, 0, 255);
      endclr = blend(CRGB::Yellow, CHSV(0, 0, 170), speed);
      midclr = blend(CHSV(0, 0, 150), CRGB::Yellow, speed);

      fill_gradient_RGB(leds, 0, endclr, NUM_LEDS / 2, midclr);
      fill_gradient_RGB(leds, NUM_LEDS / 2 + 1, midclr, NUM_LEDS, endclr);
      FastLED.show();
    }

    // 1 = sunrise (pink & blue)
    if (results.value == 16724175) {
      fill_gradient(leds, 0, CHSV(160, 185, 210), NUM_LEDS - 1, CHSV(237, 141, 255));
      FastLED.show();
    }

    // 2 = sunset (orange & blue)
    if (results.value == 16718055) {
      fill_gradient_RGB(leds, 0, CHSV(150, 255, 200), NUM_LEDS - 1, CHSV(25, 255, 255));
      FastLED.show();
    }

    // 3 = sunrise to sunset
    if (results.value == 16743045) {
      uint8_t speed = beatsin8(10, 0, 255);
      endclr = blend(CHSV(150, 255, 210), CHSV(160, 185, 200), speed);
      midclr = blend(CHSV(25, 255, 255), CHSV(237, 141, 255), speed);

      fill_gradient_RGB(leds, 0, endclr, NUM_LEDS / 2, midclr);
      fill_gradient_RGB(leds, NUM_LEDS / 2 + 1, midclr, NUM_LEDS, endclr);
      FastLED.show();
    }

    // 4 = night
    if (results.value == 16716015) {
      fill_gradient_RGB(leds, 0, CHSV(160, 255, 150), NUM_LEDS - 1, CHSV(180, 230, 80));
      FastLED.show();
    }

    // 5 = starry night
    if (results.value == 16726215) {
      fill_gradient_RGB(leds, 0, CHSV(160, 255, 150), NUM_LEDS - 1, CHSV(180, 230, 80));
      addGlitter(80);
      FastLED.show();
    }

    // 6 = rain
    if (results.value == 16734885) {
      fadeToBlackBy( leds, NUM_LEDS, 30);
      int pos = random16(NUM_LEDS);
      leds[pos] = CHSV(135, 80, 200);
      FastLED.show();
    }

    // 7 = acid rain
    if (results.value == 851901943) {
      fadeToBlackBy( leds, NUM_LEDS, 130);
      int pos = random16(NUM_LEDS);
      leds[pos] = CRGB::Green;
      FastLED.show();
    }

    // 8 = snow
    if (results.value == 16730805) {
      fadeToBlackBy( leds, NUM_LEDS, 30);
      addGlitter(50);
      FastLED.show();
    }

    // 9 = storm
    if (results.value == 16732845) {
      fadeToBlackBy( leds, NUM_LEDS, 30);
      fill_solid(leds, NUM_LEDS, CHSV(0, 0, 50));
      addGlitter(60);
      lightning();
      FastLED.show();
    }

    irrecv.resume(); // Receive the next value
  }
}

//<——————————————————————OTHER FUNCTIONS————————————————————>

void addGlitter( fract8 chanceOfGlitter)
{ if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void lightning ()
{ EVERY_N_MILLISECONDS (4000) {
    uint8_t i = beatsin8(6, 0, 255);
    fill_solid(leds, NUM_LEDS, CHSV(160, 10, i));
  }
}
