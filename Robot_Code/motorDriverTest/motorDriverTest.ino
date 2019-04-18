#define LEFT_WHEEL_FORWARD 13
#define LEFT_WHEEL_REVERSE 10
#define RIGHT_WHEEL_FORWARD 12
#define RIGHT_WHEEL_REVERSE 11
#define ANGLE 115
#define STOP 0

int robotDirection = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LEFT_WHEEL_FORWARD, OUTPUT);
  pinMode(LEFT_WHEEL_REVERSE, OUTPUT);
  pinMode(RIGHT_WHEEL_FORWARD, OUTPUT);
  pinMode(RIGHT_WHEEL_REVERSE, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    robotDirection = Convert();
  }
  Serial.println(robotDirection);


  if (robotDirection == 1) {
    analogWrite(LEFT_WHEEL_REVERSE, STOP);
    analogWrite(RIGHT_WHEEL_REVERSE, STOP);
    analogWrite(LEFT_WHEEL_FORWARD, ANGLE);
    analogWrite(RIGHT_WHEEL_FORWARD, ANGLE);
  } else if (robotDirection == -1) {
    analogWrite(LEFT_WHEEL_FORWARD, STOP);
    analogWrite(RIGHT_WHEEL_FORWARD, STOP);
    analogWrite(LEFT_WHEEL_REVERSE, ANGLE);
    analogWrite(RIGHT_WHEEL_REVERSE, ANGLE);
  } else {
    analogWrite(LEFT_WHEEL_REVERSE, STOP);
    analogWrite(RIGHT_WHEEL_REVERSE, STOP);
    analogWrite(LEFT_WHEEL_FORWARD, STOP);
    analogWrite(RIGHT_WHEEL_FORWARD, STOP);
  }

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
