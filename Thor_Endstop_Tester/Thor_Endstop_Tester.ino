// states
#define SETTING  0
#define START    1
#define PRINTOUT 2

// devices
#define PHOTOELECTRIC 0
#define MECHANICAL    1

// for mechanical switch sensor
#define INVERTED 0

// variables
byte state;
char decision;
byte device;
byte pin; // signal
bool sign; // signal output

void setup() {
  state = SETTING;

  Serial.begin(9600);
  Serial.println("******************");
  Serial.println("THOR ENDSTOP TESTS");
  Serial.println("WITH ARDUINO UNUO");
  Serial.println("******************");
  Serial.println("\nAT ANY POINT WHERE THE PROGRAM EXPECTS AN INPUT");
  Serial.println("YOU CAN TYPE 'r' TO SET THE ENDSTOP YOU WANT TO TEST!");
}

void waitForInput() {
  while( !Serial.available() ) {
    // waiting 4 serial input
  }
}

void loop() {

  switch(state) {

    /*========== SETTING ==========*/
    case SETTING:
      Serial.println("Enter signal pin (2 - 13):");
      waitForInput(); // expecting the pins (2 - 13), any other result will go to this SETTING STATE again
      decision = Serial.readString(); // reads the next valid input
      if(2 <= decision && decision <= 13) {
        pin = decision; // setting the pin for the output
        pinMode(pin, INPUT);
        state = START;
      }
      break;

    /*========== START STATE ==========*/
    case START:
      Serial.println("\nWhich kind of endstop is it?");
      Serial.println("Photoelectric sensor: 0");
      Serial.println("Mechanical switch   : 1");
      waitForInput(); // expecting either 0 or 1, any other result will go to this START STATE again
      decision = Serial.read(); // reads next single char in buffer
      if(decision == 1) { // no 'if(decision)' because e.g. '2' is not a valid input
        // decision '1' for testing mechanical switch sensor
        device = MECHANICAL;
      else if(!decision) { // equivalent to 'decision == 0'
        // decision '0' for testing photoelectric sensor
        device = PHOTOELECTRIC;
      state = PRINTOUT;
      break;


    /*========== PRINTOUT STATE ==========*/
    case PRINTOUT:
      // in case for stopping the output & go back to START STATE
      if(Serial.available() > 0) {
        decision = Serial.read();
        if(decision == 'r')
          state = START;
      }

      // outputs for the according sensoric devices
      switch(device) {
        sign = digitalRead(pin); // reads pin signal

        /*---------- output for photoelectric sensors ----------*/
        case PHOTOELECTRIC:
          if(sign == 1)
            Serial.println("blocked");
          else if(!sign) // equivalent to 'sign == 0'
            Serial.println("unblocked");
          break;

        /*---------- output for mechanical switch sensors ----------*/
        case MECHANICAL:
          // in case of 'NC' (Normally Closed) otherwise 'NO' (Normally Open)
          if(INVERTED)
            sign = !sign;

          if(sign == 1)
            Serial.println("open");
          else if(!sign) // equivalent to 'sign == 0'
            Serial.println("closed");
          else
            Serial.println("No idea...");
          break;
      }
      delay(200);
      break; // state break
  }
  // flushes the buffer after every input
  Serial.flush();
}

// Deprecated
/*
 * Orange Thor:
 *   Base-Art1: Tested
 *     Red  : Signal
 *     Brown: 5V
 *     Black: Ground
 *   Art1-Art2: Tested
 *     Orange: Signal.
 *     Yellow: 5V
 *     Red   : Ground
 *   Art2-Art3: Tested -> unwanted unblocked at both ends
 *     Blue  : Signal
 *     Green : 5V
 *     Purple: Ground
 *   Art3-Art4:- Tested
 *     Orange: 5V     ???
 *     Yellow: Signal ???
 *     Green : Ground
 *   Endeff.-Sensor: Tested
 *     Green : Normally Closed (NC) | current when triggered
 *     Blue  : Normally Open   (NO) | no current when triggered
 *     Purple: Ground          (C)
 */
