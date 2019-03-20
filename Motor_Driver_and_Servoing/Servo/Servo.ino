
#define MOTOR_PIN 10
#define IR_SENSOR_Interuppt_PIN 9

// variables
int tick;
int desiredPosition;
int currentPosition;

void setup() {
  // Open serial communications and wait for port to open:
  Serial1.begin(9600);
  pinMode(MOTOR_PIN, OUTPUT);                                            // init the digital pin as output for motor control
  pinMode(IR_SENSOR_Interuppt_PIN, INPUT_PULLUP);                        // init inturrupt pin
  attachInterrupt(digitalPinToInterrupt(IR_SENSOR_Interuppt_PIN), count, RISING); // attach interupt to a function
  
  desiredPosition =0;                                                   // init desiredPosition as 0 ; until user defines another one
  tick = 0;                                                             // intit the tick counter for the IR sensor
}



void loop() { // run over and over
  // if there is a value in the 
  if (Serial1.available() > 0) {
    desiredPosition = Convert();
      Serial.print(desiredPosition);
      tick =0;

  }

  while(tick < desiredPosition ){
    turnMotor();
    Serial.println(tick);
  }
  delay(1000);
}

// convert inputed serial data as a integer value
int Convert(void) {
  int incomingByte = 0;
  int integerValue = 0;
  int sign = 1;
  while (1) {                                                                   // force into a loop until 'n' is received
    incomingByte = Serial1.read();                                               // byte that was read
    if (incomingByte == '\n') break;                                            // exit the while(1), we're done receiving
    if (incomingByte == 45) {
      sign= -1;                                                                 // make value negative
    } else {
      integerValue *= 10;
      integerValue = ((incomingByte - 48) + integerValue);                        // convert ASCII to integer, add, and shift left 1 decimal place
    }

  }
  integerValue = integerValue * sign;                                           //
  return integerValue;  
   // Serial.println(integerValue);
}

//
void count(void){
  tick++;
  Serial.println(tick);
}

//setMotor
void  turnMotor(void) {
  // while (true) {
  digitalWrite(MOTOR_PIN, HIGH);                                  // set motor on
  delayMicroseconds(550);                                       // keep signal on

  digitalWrite(MOTOR_PIN, LOW);                                   // set motor on
  delayMicroseconds(450);                                        // keep signal off
  //Serial.print("motor on");
  // }
}
