unsigned long currentMillis = 0;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
currentMillis = millis(); // Store the current time in time 
 // Serial.println("end time");

  Serial.println(currentMillis);
}
