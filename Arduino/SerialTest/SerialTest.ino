
void setup() {
  // Open serial communications and wait for port to open:
  Serial1.begin(9600);

}

void loop() { // run over and over

  Serial1.write("Hi Yohanes");
  delay(2000);
}
