
void setup() {
  // Open serial communications and wait for port to open:
  Serial1.begin(9600);

}

void loop() { // run over and over

  Convert();
  delay(2000);
}

void Convert(void) {
  int incomingByte = 0;
  int integerValue = 0;
  while (1) {                                                                   // force into a loop until 'n' is received
    incomingByte = Serial1.read();                                               // byte that was read
    if (incomingByte == '\n') break;                                            // exit the while(1), we're done receiving
    integerValue *= 10;                                                         // shift left 1 decimal place
    integerValue = ((incomingByte - 48) + integerValue);                        // convert ASCII to integer, add, and shift left 1 decimal place
    
  }
  Serial.println(integerValue);
}
