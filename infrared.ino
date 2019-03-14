/**
 * This file contains all functions in relation to the Infrared Receiver.
 */

// The code that is sent when a repetition signal is sent (e.g. holding down the + button)
#define rep 4294967295
// The pin to connect the IR receiver to
#define RECV_PIN  11

IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long lastCommand = millis();
long int lastvalue;

void initIR() {
  irrecv.enableIRIn();
}

void readIR() {
  if (irrecv.decode(&results)) {
    if (( millis() - lastCommand) > 500) {
      lastCommand = millis();
      FastLED.delay(1000 / 120);
      long int result = results.value;

      if (results.value == rep) {
        result = lastvalue;
      }

      lastvalue = result;

      Serial.println(result);

      setLampState(result);

      irrecv.resume(); // Receive the next value
    }
  }
}
