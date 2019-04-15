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

/////////////////////////////////////////////////////////////////

//variables

int desiredAngle = 0;
int currentAngle = 0;

int currentTick;
int desiredTick = 0;
int tick = 0;

int error = 0;

int BOTTOM_IR_SENSOR = 0;
int TOP_IR_SENSOR = 0;

int currentState = 0;
int previousState = 0;

<<<<<<< HEAD
<<<<<<< HEAD
int motorPin = FORWARD_MOTOR_PIN;
int turnDirection = 0;

int error = 0;

int temp = 0;
int degreeMoved = 0;
long degree = DEGREES_PER_TICK;
=======
int turnDirection = 0;
>>>>>>> parent of a01fea9... Update Servo.ino
=======
int turnDirection = 0;
>>>>>>> parent of a01fea9... Update Servo.ino

/////////////////////////////////////////////////////////////////

// methods used

<<<<<<< HEAD
<<<<<<< HEAD
int Convert(void) {
  int incomingByte = 0;
  int integerValue = 0;
  int sign = 1;
  while (1) {                                                                   // force into a loop until 'n' is received
    incomingByte = Serial1.read();                                               // byte that was read
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

=======
>>>>>>> parent of a01fea9... Update Servo.ino
=======
>>>>>>> parent of a01fea9... Update Servo.ino
//motor direction
void motorDirection(void) {
  if ((currentState == 1) && (previousState == 3)) {
    turnDirection = FORWARD;
    
  }

  if ((currentState == 2) && (previousState == 3)) {
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

void counter(void) {
  tick++;
}

/////////////////////////////////////////////////////////////////

//put your setup code
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial1.begin(9600);

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

  // case where
  encoderState();
  motorDirection();
  previousState  = currentState;

<<<<<<< HEAD
<<<<<<< HEAD
  while (1) {
    //encoderState();
    //Serial.println(currentState);
    
    //motorDirection();
    //Serial.println(turnDirection);

    // Input serial data from Pi
    if (Serial1.available() > 0) {
      desiredAngle = Convert();
      changeAngle = desiredAngle - currentAngle;
      Serial.println("desiredAngle");
      Serial.println(desiredAngle);
    }
/*




    if (currentAngle < desiredAngle) {
      //pController();
      // degree = DEGREES_PER_TICK;
      motorPin = FORWARD_MOTOR_PIN;
    } else if (currentAngle > desiredAngle) {
      //pController();
      //degree = -DEGREES_PER_TICK;
      motorPin = REVERSE_MOTOR_PIN;
    } else {
      break;
    }


    if ((motorPin == FORWARD_MOTOR_PIN) && (turnDirection == FORWARD) && (currentAngle < desiredAngle)) {
      degree = DEGREES_PER_TICK;
      //delay(500);
      //Serial.println("im am going forward");
    } else if ((motorPin == REVERSE_MOTOR_PIN) && (turnDirection == REVERSE) && (currentAngle > desiredAngle)) {
      degree = -DEGREES_PER_TICK;
      //delay(500);
      //Serial.println("im am going reverse");
    } else {
      degree = 0;
      //Serial.println("nothing is happening");
    }





    //turnMotor(110);
    //Serial.println(currentAngle);
    */
  }
  turnMotor(0);


  //Serial.println(currentAngle);
  delay(500);
=======

>>>>>>> parent of a01fea9... Update Servo.ino
=======

>>>>>>> parent of a01fea9... Update Servo.ino
}
