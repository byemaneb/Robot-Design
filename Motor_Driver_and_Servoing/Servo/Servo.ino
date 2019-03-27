
#define FORWARD_MOTOR_PIN 9
#define REVERSE_MOTOR_PIN 10

#define BOTTOM_IR_SENSOR_INTERUPPT_PIN 16
#define TOP_IR_SENSOR_INTERUPPT_PIN 15


// variables
int tick;
int desiredPosition;
int currentPosition;
int   sensorA;
int   sensorB;
int state;
int currentState;
int previousState;
int motorDirection;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(FORWARD_MOTOR_PIN, OUTPUT);                                            // init the digital pin as output for motor control
  pinMode(REVERSE_MOTOR_PIN, OUTPUT);                                            // init the digital pin as output for motor control

  pinMode(BOTTOM_IR_SENSOR_INTERUPPT_PIN, INPUT_PULLUP);                        // init inturrupt pin
  attachInterrupt(digitalPinToInterrupt(TOP_IR_SENSOR_INTERUPPT_PIN), topSensorOn, RISING); // attach interupt to a function
  //attachInterrupt(digitalPinToInterrupt(BOTTOM_IR_SENSOR_INTERUPPT_PIN), bottomSensorOn, RISING); // attach interupt to a function

  //attachInterrupt(digitalPinToInterrupt(TOP_IR_SENSOR_INTERUPPT_PIN), topSensorOff, LOW); // attach interupt to a function
  //attachInterrupt(digitalPinToInterrupt(TOP_IR_SENSOR_INTERUPPT_PIN), bottomSensorOff, FALLING); // attach interupt to a function

  //desiredPosition = 0;                                                   // init desiredPosition as 0 ; until user defines another one
  //tick = 0;                                                             // intit the tick counter for the IR sensor

  sensorA = 0;
  sensorB = 0;
  motorDirection = FORWARD_MOTOR_PIN;
  state = 0;
}



void loop() { // run over and over
  // if there is a value in the
  //Serial.println("desiredPosition");

  if (Serial.available() > 0) {
    desiredPosition = Convert();
    Serial.println("desiredPosition");
    Serial.println(desiredPosition);
    tick = 0;

  }
  
    while(tick < desiredPosition ){
      turnMotor(100);
      Serial.println(tick);
    }
  




  turnMotor(0);
  delay(500);
}

// convert inputed serial data as a integer value
int Convert(void) {
  int incomingByte = 0;
  int integerValue = 0;
  int sign = 1;
  while (1) {                                                                   // force into a loop until 'n' is received
    incomingByte = Serial.read();                                               // byte that was read
    if (incomingByte == '\n') break;                                            // exit the while(1), we're done receiving
    if (incomingByte == 45) {
      sign = -1;                                                                // make value negative
    } else {
      integerValue *= 10;
      integerValue = ((incomingByte - 48) + integerValue);                        // convert ASCII to integer, add, and shift left 1 decimal place
    }

  }
  integerValue = integerValue * sign;                                           //
  return integerValue;
  //Serial.println(integerValue);
}

//
void topSensorOn(void) {
  tick++;
 // sensorA = !sensorA;
  //Serial.println("top");
  //Serial.println(sensorA);

}
void topSensorOff(void) {
  Serial.println("top off");
}
void bottomSensorOn(void) {

  sensorB = !sensorB;
  //Serial.println("bottom");
  //Serial.println(sensorB);

}
void bottomSensorOff(void) {
  Serial.println("bottom off");
}

//setMotor
void  turnMotor(int angle) {
  analogWrite(motorDirection, angle); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255

}
