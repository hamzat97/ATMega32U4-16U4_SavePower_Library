#include <SavePower.h>

#define INTPIN 3

void ISR_function() {
  // Put your ISR instructions here
}

void setup()
{ 
  // Configure the interrupt pin 3 as input with pull-up resistor to read the digital input on pin 3
  pinMode(INTPIN, INPUT_PULLUP);
}

void loop() 
{ 
  // Allow wake up pin to trigger interrupt whenever the pin changes value
  attachInterrupt(digitalPinToInterrupt(INTPIN), ISR_function, HIGH);
  
  // Enter the MCU into a sleep mode forever   
  SavePower.IdleMode(SLEEP_FOREVER);
  // SavePower.ADCNoiseReductionMode(SLEEP_FOREVER);
  // SavePower.PowerDownMode(SLEEP_FOREVER);
  // SavePower.PowerSaveMode(SLEEP_FOREVER);
  // SavePower.StandbyMode(SLEEP_FOREVER);
  // SavePower.ExtendedStandbyMode(SLEEP_FOREVER);

  // Turn off the interrupt 
  detachInterrupt(digitalPinToInterrupt(INTPIN));
}
