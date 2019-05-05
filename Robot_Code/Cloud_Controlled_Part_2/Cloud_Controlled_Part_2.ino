#include  <Arduino.h>
#include  "SourceCode.h"

int counter = 0;


void setup() {
  // put your setup code here, to run once:
  connectWifi();
  initializePorts();

}

void loop() {
  
  orientation();  // read in heading data
  readUDP();      // open UDP port and read in UDP data

  pickUP();       // pickUp = TRUE if thrshhold is reached
  
  // check UDP angle data
  if(((dataIn->robotAngle) == 90)||((dataIn->robotAngle) == 180)||((dataIn->robotAngle) == 270)||((dataIn->robotAngle) == 360)){
    rotate();     // rotate robot until wanted orientation is reached
  }else{
    driveRobot();     // drive robot to UDP provided speed
  }


}
