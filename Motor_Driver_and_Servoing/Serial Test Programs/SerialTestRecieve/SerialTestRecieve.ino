int r = 1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (1) {
    if (Serial1.available() > 0) {
      Serial.println("input");
      r = Serial1.read();
      Serial.println(r);
    }
  }
}
