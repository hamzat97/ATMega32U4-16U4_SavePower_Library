#include <SavePower.h>

void setup()
{
  // Setup is not required for this example
}

void loop() 
{
  // You can put your code everywhere you want in this exemple.  
  
  // Take note that the available durations are : 15ms, 30ms, 60ms, 120ms, 250ms, 500ms, 1s, 2s, 4s, 8s
  // Whatever the chosen duration, you can increase it by an iterative loop    

  // Lowest consumption for 8 seconds
  SavePower.LowestConsumption(WDTO_8S);
}
