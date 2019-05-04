#include  <Arduino.h>
#include  "SourceCode.h"

int counter = 0;


void setup() {
  // put your setup code here, to run once:
  connectWifi();
  initializePorts();

}

void loop() {

  readUDP();    // open and read in UDP data
  pickUP();     // check if robot has been picked up; if so then set speed = 0
  driveRobot(); // drive robot using speed sent through UDP packet or pickup()

}
