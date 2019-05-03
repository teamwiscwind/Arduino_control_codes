
#include "Arduino.h"

// use digital pin8 as input pin for push button
#define BUTTON_PIN 8

// use A0 as input pin for current sensing
#define CURRENT_SENSING A0

// use digital pin9, pin10 as output pin for sending breaking pulses
#define PULSE_SIG_ACT 9
#define PULSE_SIG_DEACT 10

#define PULSE_TIME 500

static unsigned int state;
int button;

// the voltage at current sensing pin
int v_current_sensing;

// the flag that the current is zero
int zero_current;

// set the threshold voltage at current sensing pin to be 0.004v (close to 0v)
const int v_current_threshold = 0.004;

void setup() {
  state = 1;
  pinMode(BUTTON_PIN, INPUT);
  pinMode(PULSE_SIG_ACT, OUTPUT);
  pinMode(PULSE_SIG_DEACT, OUTPUT);
}

void loop() {

  button = digitalRead(BUTTON_PIN);
  
  // check voltage of current sensing here and determine the boolean value of zero_current; 
  // based on Vout = RL/10K * I(Amps) on current sensor, RL = 10k, Vout = I in amps, I resolution is about 0.004888
  v_current_sensing = 5.0/1023.0*analogRead(CURRENT_SENSING);
  if (v_current_sensing < v_current_threshold) {
    zero_current = 1;
  }
  else {
    zero_current = 0;
  }

  //state 1: normal operation
  //state 2: breaking due to pressing button
  //state 3: breaking due to current sensing = 0 (disconnect of the load)
  switch (state)
  {
  case 1:
    if(button == HIGH) {
      state = 2;
      
      // send pulse signal of breaking here, pulse has voltage 3.3v/5v

      // start the pulse
      digitalWrite(PULSE_SIG_ACT, HIGH);

      // assume the pulse duration is 0.5s here
      delay(PULSE_TIME);

      // end the pulse
      digitalWrite(PULSE_SIG_ACT, LOW);

      break;
    }
    
    if(zero_current == 1) {
      state = 3;
      
      // send pulse signal of breaking here, pulse has voltage 3.3v/5v

      // start the pulse
      digitalWrite(PULSE_SIG_ACT, HIGH);

      // assume the pulse duration is 0.5s here
      delay(PULSE_TIME);

      // end the pulse
      digitalWrite(PULSE_SIG_ACT, LOW);

      break;
    }

    break;
 
  case 2:
    if(button == HIGH ) {
      state = 1;
      
      // send pulse signal of breaking here, pulse has voltage 3.3v/5v

      // start the pulse
      digitalWrite(PULSE_SIG_DEACT, HIGH);

      // assume the pulse duration is 0.5s here
      delay(PULSE_TIME);

      // end the pulse
      digitalWrite(PULSE_SIG_DEACT, LOW);

    }

    break;

  case 3:
    if(zero_current == 0) {
      state = 1;
      
      // send pulse signal of breaking here, pulse has voltage 3.3v/5v

      // start the pulse
      digitalWrite(PULSE_SIG_DEACT, HIGH);

      // assume the pulse duration is 0.5s here
      delay(PULSE_TIME);

      // end the pulse
      digitalWrite(PULSE_SIG_DEACT, LOW);

    }

    break;
  }

}
