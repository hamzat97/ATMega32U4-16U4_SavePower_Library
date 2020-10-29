#include <SavePower.h>

void setup()
{
  // Setup is not required for this example
}

void loop() 
{
  // The possible durations in any of the sleep modes are : 15ms, 30ms, 60ms, 120ms, 250ms, 500ms, 1s, 2s, 4s, 8s
  // Whatever the chosen duration, we can increase it by an iterative loop     
  
  SavePower.IdleMode(WDTO_8S);
  // SavePower.ADCNoiseReductionMode(SLEEP_8S);
  // SavePower.PowerDownMode(SLEEP_8S);
  // SavePower.PowerSaveMode(SLEEP_8S);
  // SavePower.StandbyMode(SLEEP_8S);
  // SavePower.ExtendedStandbyMode(SLEEP_8S);
  
  // Put your code here
}
