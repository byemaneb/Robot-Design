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

int turnDirection = 0;

/////////////////////////////////////////////////////////////////

// methods used

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


}
