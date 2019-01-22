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
  if (irrecv.decode(&results)) {


    FastLED.delay(1000 / 120);

    
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


    //func/stop = xmas
    if (results.value == 16750695) {
      uint8_t dot = beatsin8(50, 0, NUM_LEDS);
      leds[dot] = CRGB::Red;
      leds[dot + 1] = CRGB::Green;
      FastLED.show();
    }


    //eq = rainbow
    if (results.value == 16756815) {
      fill_rainbow( leds, NUM_LEDS - 3, gHue, 7);
      leds[12] = CRGB::Blue;   //adjust the rainbow slightly towards the purple end
      leds[13] = CRGB::Purple;
      leds[14] = CRGB::Purple;
      FastLED.show();
    }
    
    if (results.value == 16738455) {
        fill_solid(leds, NUM_LEDS, CRGB::White);
    }

    if (results.value == 16724175) {
       fill_solid(leds, NUM_LEDS, CRGB::Blue);
    }

    // 2 = sunset (orange & blue)
    if (results.value == 16718055) {
       fill_solid(leds, NUM_LEDS, CRGB::Green);
    }

    // 3 = sunrise to sunset
    if (results.value == 16743045) {
       fill_solid(leds, NUM_LEDS, CRGB::Red);
    }

    // 4 = night
    if (results.value == 16716015) {
      fill_solid(leds, NUM_LEDS, CRGB::Pink);
    }

    // 5 = starry night
    if (results.value == 16726215) {
      fill_solid(leds, NUM_LEDS, CRGB::Purple);
    }

    // 6 = rain
    if (results.value == 16734885) {
      fill_solid(leds, NUM_LEDS, CRGB::LightBlue);
    }

    // 7 = acid rain
    if (results.value == 851901943) {
    
    }

    // 8 = snow
    if (results.value == 16730805) {
    
    }

    // 9 = storm
    if (results.value == 16732845) {
   ;
    }

    irrecv.resume(); // Receive the next value
  }
}

//<——————————————————————OTHER FUNCTIONS————————————————————>
