#define PIN_A     10                // pin for forward motor direction
#define PIN_B     6               // pin for reverse motor direction

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_A, OUTPUT);                     // sets the digital pin as output
  pinMode(PIN_B, OUTPUT);                     // sets the digital pin as output
  
}

void loop() {
  // put your main code here, to run repeatedly:
while (true) {
    digitalWrite(PIN_A, HIGH);                                  // set motor on
    delayMicroseconds(500);                                       // keep signal on

    digitalWrite(PIN_A, LOW);                                   // set motor on
    delayMicroseconds(500);                                        // keep signal off
  }
}
