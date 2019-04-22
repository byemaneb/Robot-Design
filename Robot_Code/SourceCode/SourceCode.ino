#include  <Arduino.h>
#include  "SourceCode.h"
void setup() {
  // put your setup code here, to run once:
  connectWifi();
  initializePorts();
  
}

void loop() {
  readUDP();
  //testFunction();
}
