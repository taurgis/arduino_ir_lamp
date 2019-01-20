
#include <Adafruit_NeoPixel.h>
#include <IRremote.h>

#define PIN 6    // led strand pin
#define NUMLEDS 133

#define receiver 7  // IR receiver pin 
#define receiver_vcc 12 // using signal pins to supple vcc and ground to ir receiver
#define receiver_gnd 11

long lastResult;
int red = 100;
int green = 200;
int BRIGHTNESS = 100;

IRrecv irrecv(receiver); //create a new instance of receiver
decode_results results;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800);
bool onoff; //defines the current state of the LEDs

void setup() {
  Serial.begin(9600);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  pinMode(receiver_vcc, OUTPUT);
  pinMode(receiver_gnd, OUTPUT);
  pinMode(receiver, INPUT_PULLUP);
  digitalWrite(receiver_vcc, HIGH);
  digitalWrite(receiver_gnd, LOW);
  irrecv.enableIRIn(); //start the receiver

  onoff = false;
}

unsigned long lastCommand = millis();
void loop() {
  long result;
  
  if (irrecv.decode(&results)) { //we have received an IR code
    if (( millis() - lastCommand) > 350) {
      lastCommand = millis();
      if (results.value == -1) {
        result = lastResult;
      } else {
        result = results.value;
      }

      Serial.println(result);  // not actually intending to use serial in ISR, just added now to see what is going on with result value.
      switch (result) {
        case 16761405:   // Button 1
          onoff = !onoff;
          break;
        case 16754775:   // Button 2
          if (BRIGHTNESS < 200) {
            BRIGHTNESS += 25;
            strip.setBrightness(BRIGHTNESS);
          }
          // do something
          break;
        case 16769055:   // Button 2
          if (BRIGHTNESS >  25) {
            BRIGHTNESS -= 25;
            strip.setBrightness(BRIGHTNESS);
          }
          // do something
          break;
        case 16753245:
          if (green < 250) {
            red -= 12.5;
            green += 25;
          }
          break;

        case 16769565:
          if (green > 0) {
            red += 12.5;
            green -= 25;
          }

          break;
          // many more cases in switch
      }  // switch

      lastResult = result;
    } // if

    irrecv.resume(); //next value
  }
  
  if (onoff) {
    gradiant();
  } else {
    black();
  }
  
  delay(100);
}

void black() {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor( i, strip.Color(0, 0, 0 ) );

  }

  strip.show();
}

void blueWhiteChase() {
  static int i = 0;
  for (int j = -5; j < NUMLEDS; j = j + 6) {
    if (j + i < 0);
    strip.setPixelColor(j + i, 0, 0, 30);
    strip.setPixelColor(j + i + 1, 0, 0, 100);
    strip.setPixelColor(j + i + 2, 0, 0, 255);
    strip.setPixelColor(j + i + 3, 30, 30, 30);
    strip.setPixelColor(j + i + 4, 90, 90, 90);
    strip.setPixelColor(j + i + 5, 180, 180, 180);
  }
  strip.show();
  i = (i + 1) % 6;
}


int startPixel = 0;

// sp is a local variable that will be the actual first pixel that we write to
void gradiant() {
  int sp = startPixel;

  for ( int i = 0; i < NUMLEDS; i++ ) {
    strip.setPixelColor(sp, red, i, green );

    if ( sp == NUMLEDS )
      sp = 0;
    else
      sp++;
  }

  strip.show();

  startPixel++;
  if ( startPixel == 133 )
    startPixel = 0;
}
