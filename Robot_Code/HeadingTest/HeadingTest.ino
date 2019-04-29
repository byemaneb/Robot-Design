#include <Wire.h>
#include <LSM303.h>

#define LEFT_WHEEL_FORWARD 10
#define LEFT_WHEEL_REVERSE 11
#define RIGHT_WHEEL_FORWARD 12
#define RIGHT_WHEEL_REVERSE 13
#define ANGLE 115
#define STOP 0
#define OFFSET  2

LSM303 compass;


int state = 1;
int desiredAngle = 0;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();

  compass.m_min = (LSM303::vector<int16_t>) {
    -2401, -2064, -2877
  };
  compass.m_max = (LSM303::vector<int16_t>) {
    +3344, +3881, +3574
  };

}

void loop() {

  // read in IMU values
  compass.read();
  float heading = compass.heading();
  Serial.println("heading");
  Serial.println(heading);



  /*
    float heading = compass.heading();
    Serial.println("angle");
    Serial.println(heading);

    snprintf(report, sizeof(report), "A: %6d %6d %6d    M: %6d %6d %6d",compass.a.x, compass.a.y, compass.a.z,compass.m.x, compass.m.y, compass.m.z);
    Serial.println("report");
    Serial.println(report);
  */

  switch (state) {
    case 1:
      // recieve and convert user input
      if (Serial.available() > 0) {
        desiredAngle = Convert();
        state = 2;
      }
      1break;
    case 2:

      if ((int(heading) > (desiredAngle + OFFSET)) || (int(heading) < (desiredAngle - OFFSET))) {
        analogWrite(LEFT_WHEEL_FORWARD, 0);
        analogWrite(RIGHT_WHEEL_FORWARD, 100);

      } else {270
        state = 3;
      }
      break;

    case 3:
      analogWrite(LEFT_WHEEL_FORWARD, 0);
      analogWrite(RIGHT_WHEEL_FORWARD, 0);
      delay(5000);
      state = 1;
      break;
    default:
      // statements
      break;
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
