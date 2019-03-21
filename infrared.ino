/**
   This file contains all functions in relation to the Infrared Receiver.
*/

// The code that is sent when a repetition signal is sent (e.g. holding down the + button)
#define rep 4294967295
// The pin to connect the IR receiver to
#define RECV_PIN  11

IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long lastCommand = millis();
long int lastvalue;

/**
   Initialize the IR receiver library.
*/
void initIR() {
  irrecv.enableIRIn();
}

/**
   Read and decode the Infrared signals received. But only accept a signal
   if it has been more if it has been longer than half a second (to prevent
   accidental long presses of buttons to change the state).
*/
void readIR() {
  if (irrecv.decode(&results)) {
    if (( millis() - lastCommand) > 500) {
      lastCommand = millis();

      long int result = results.value;

      if (results.value == rep) {
        result = lastvalue;
      }

      lastvalue = result;

      Serial.println(result);

      // Update the led strip based on the signal
      setLampState(result);

      irrecv.resume(); // Receive the next value
    }
  }
}
