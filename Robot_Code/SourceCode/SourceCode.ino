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
 // while (counter < 190) {
    readUDP();
    testFunction();
    
    //counter++;
    //Serial.println ("counter " );
    //Serial.println (counter );
    delay(100);
  //}
}
