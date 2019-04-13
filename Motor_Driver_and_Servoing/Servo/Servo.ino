/*


*/
/////////////////////////////////////////////////////////////////

// Global variables

#define MAX_ANGLE 720
#define MIN_ANGLE -720

#define FORWARD_MOTOR_PIN 9
#define REVERSE_MOTOR_PIN 10

#define BOTTOM_IR_SENSOR_INTERUPPT_PIN 16
#define TOP_IR_SENSOR_INTERUPPT_PIN 15

#define FORWARD 1
#define REVERSE -1

#define DEGREES_PER_TICK  2.4

#define ALLOWED_ERROR 5

/////////////////////////////////////////////////////////////////

//variables

int desiredAngle = 0;
long currentAngle = 0;
int changeAngle = 0;
int angleTemp = 0;

int currentTick = 0;
int previousTick = 0;
int desiredTick = 0;
int tick = 0;

int BOTTOM_IR_SENSOR = 0;
int TOP_IR_SENSOR = 0;

int currentState = 0;
int previousState = 0;

int motorPin = FORWARD_MOTOR_PIN;
int turnDirection = 1;

int error = 0;

int temp = 0;
int degreeMoved = 0;
long degree = DEGREES_PER_TICK;

/////////////////////////////////////////////////////////////////

// methods used

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
  integerValue = integerValue * sign;
  return integerValue;
}

//motor direction
void motorDirection(void) {
  if ((currentState == 1) && (previousState == 3)) {
    //motorPin = FORWARD_MOTOR_PIN;
    turnDirection = FORWARD;
  }

  if ((currentState == 2) && (previousState == 3)) {
    //motorPin = REVERSE_MOTOR_PIN;
    turnDirection = REVERSE;
  }
}

// state of the encoder wheel
void encoderState(void) {
  TOP_IR_SENSOR = digitalRead(TOP_IR_SENSOR_INTERUPPT_PIN);
  BOTTOM_IR_SENSOR = digitalRead(BOTTOM_IR_SENSOR_INTERUPPT_PIN);

  if ( (BOTTOM_IR_SENSOR == 0) && (TOP_IR_SENSOR == 0)) {
    currentState = 0;
  } else if ( (BOTTOM_IR_SENSOR == 1) && (TOP_IR_SENSOR == 0) ) {
    currentState = 1;
  } else if ( (BOTTOM_IR_SENSOR == 0) && (TOP_IR_SENSOR == 1) ) {
    currentState = 2;
  } else if ( (BOTTOM_IR_SENSOR == 1) && (TOP_IR_SENSOR == 1) ) {
    currentState = 3;
  }
}

//setMotor
void  turnMotor(int PWM) {
  analogWrite(motorPin, PWM); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
}


// converts the given angle into ticks
int convertAngleToTick(int angle) {
  int tick = angle / DEGREES_PER_TICK;
  return tick;
}

// converts the given tick into an angle
int convertTickToAngle(int tick) {
  int angle = tick * DEGREES_PER_TICK;
  return angle;
}

void counter(void) {
  currentAngle = currentAngle + degree ;
  //currentTick++;
  //currentTick = currentTick + (currentTick * turnDirection);
  // Serial.println("currentTick");
  // Serial.println(currentTick);
}

// p controller for error
void pController() {
  if (abs(changeAngle) < 90) {
    degree = DEGREES_PER_TICK;
  } else if (abs(changeAngle) > 90 && abs(changeAngle) < 180) {
    degree = DEGREES_PER_TICK;
  } else if (abs(changeAngle) > 180 && abs(changeAngle) < 270) {
    degree = 10;
  } else if (abs(changeAngle) > 270 && abs(changeAngle) < 360) {
    degree = DEGREES_PER_TICK;
  } else {
    degree = DEGREES_PER_TICK;
  }
}
/////////////////////////////////////////////////////////////////

//put your setup code
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  //initialize pins
  pinMode(FORWARD_MOTOR_PIN, OUTPUT);
  pinMode(REVERSE_MOTOR_PIN, OUTPUT);

  //initialize pins at input
  pinMode(TOP_IR_SENSOR_INTERUPPT_PIN, INPUT);
  pinMode(BOTTOM_IR_SENSOR_INTERUPPT_PIN, INPUT);
  //pinMode(BOTTOM_IR_SENSOR_INTERUPPT_PIN, INPUT_PULLUP);

  // attach interupt to a function
  attachInterrupt(digitalPinToInterrupt(TOP_IR_SENSOR_INTERUPPT_PIN), counter, RISING);
  //attachInterrupt(digitalPinToInterrupt(TOP_IR_SENSOR_INTERUPPT_PIN), topSensorTrigger, RISING);

}
/////////////////////////////////////////////////////////////////

// main code
void loop() {
  //encoderState();
  // motorDirection();
  // previousState = currentState;
  // Input serial data from Pi



  while (1) {
    encoderState();
    motorDirection();

    // Input serial data from Pi
    if (Serial.available() > 0) {
      desiredAngle = Convert();
      changeAngle = desiredAngle - currentAngle;

    }

    if (currentAngle < desiredAngle) {
      //pController();
       degree = DEGREES_PER_TICK;
      motorPin = FORWARD_MOTOR_PIN;
    } else if (desiredAngle < currentAngle) {
      //pController();
      degree = -DEGREES_PER_TICK;
      motorPin = REVERSE_MOTOR_PIN;
    } else {
      break;
    }


    turnMotor(110);
    Serial.println(currentAngle);
  }
  turnMotor(0);

  Serial.println(currentAngle);
  delay(500);
}
