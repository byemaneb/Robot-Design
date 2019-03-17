int r = 1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (1) {
    if (Serial.available() > 0) {
      Serial.println("input");
      r = Serial.read();
      Serial.println(r);
    }
  }
}
