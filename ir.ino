#define rep 4294967295
decode_results results;
unsigned long lastCommand = millis();
long int lastvalue;

void readIr() {
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
