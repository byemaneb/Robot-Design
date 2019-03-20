
void setup() {
  // Open serial communications and wait for port to open:
  Serial1.begin(9600);

}

void loop() { // run over and over
  if (Serial1.available() > 0) {
    Convert();
  }
  delay(1000);
}

void Convert(void) {
  int incomingByte = 0;
  int integerValue = 0;
  int sign = 1;
  while (1) {                                                                   // force into a loop until 'n' is received
    incomingByte = Serial1.read();                                               // byte that was read
    if (incomingByte == '\n') break;                                            // exit the while(1), we're done receiving
    if (incomingByte == 45) {
      sign= -1;
    } else {
      integerValue *= 10;
      integerValue = ((incomingByte - 48) + integerValue);                        // convert ASCII to integer, add, and shift left 1 decimal place
    }

  }
  integerValue = integerValue * sign;
    Serial.println(integerValue);

}
