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

  readUDP();
  driveRobot();
  /*
  systemData->robotSpeed = 1;
  systemData->robotTurn = 2;
  systemData->robotAngle = 3;

  Serial.println ("robotSpeed" );
  Serial.println(systemData->robotSpeed);
    Serial.println ("robotTurn" );
  Serial.println(systemData->robotTurn);
    Serial.println ("robotAngle" );
  Serial.println(systemData->robotAngle);
  */
  //updateData();
  //outputUDP();
  //Serial.println ("netTransformMatrix" );
  //Serial.println(systemData->robotSpeed);
  //printMatrix(transformMatrix);
  delay(100);

}
