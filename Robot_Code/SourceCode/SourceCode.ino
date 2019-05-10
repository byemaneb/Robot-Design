#include  <Arduino.h>
#include  "SourceCode.h"

int counter = 0;


void setup() {
  // put your setup code here, to run once:
  connectWifi();
  initializePorts();

}

void loop() {
  
  /*
    timeTick = millis();
    while (timeTock < REFRESH_TIME ) {
    timeCurrent = millis();
    timeTock = timeCurrent - timeTick;
    }
  */
  orientation();
  readUDP();
  pickUP();
     
  
  if(((dataIn->robotAngle) == 90)||((dataIn->robotAngle) == 180)||((dataIn->robotAngle) == 270)||((dataIn->robotAngle) == 360)){
    rotate();
  }else{
    driveRobot();
  }


}
