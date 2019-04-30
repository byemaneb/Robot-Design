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
    -2087, -1879, -2513
  };
  compass.m_max = (LSM303::vector<int16_t>) {
    +3111, +3651, +3437
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
    case 0:

      analogWrite(LEFT_WHEEL_FORWARD, 0);
      analogWrite(RIGHT_WHEEL_FORWARD, 0);
      delay(5000);
      state++;
      break;
    case 1:
      desiredAngle = 270;
      if ((int(heading) > (desiredAngle + OFFSET)) || (int(heading) < (desiredAngle - OFFSET))) {
        analogWrite(LEFT_WHEEL_FORWARD, 0);
        analogWrite(RIGHT_WHEEL_FORWARD, 100);

      } else {
        analogWrite(LEFT_WHEEL_FORWARD, 0);
        analogWrite(RIGHT_WHEEL_FORWARD, 0);
        delay(5000);
        state++;
      }
      break;
    case 2:
      desiredAngle = 90;
      if ((int(heading) > (desiredAngle + OFFSET)) || (int(heading) < (desiredAngle - OFFSET))) {
        analogWrite(LEFT_WHEEL_FORWARD, 0);
        analogWrite(RIGHT_WHEEL_FORWARD, 100);

      } else {
        analogWrite(LEFT_WHEEL_FORWARD, 0);
        analogWrite(RIGHT_WHEEL_FORWARD, 0);
        delay(5000);
        state++;
      }
      break;
    case 3:
      desiredAngle = 0;
      if ((int(heading) > (desiredAngle + OFFSET)) || (int(heading) < (desiredAngle - OFFSET))) {
        analogWrite(LEFT_WHEEL_FORWARD, 0);
        analogWrite(RIGHT_WHEEL_FORWARD, 100);

      } else {
        analogWrite(LEFT_WHEEL_FORWARD, 0);
        analogWrite(RIGHT_WHEEL_FORWARD, 0);
        delay(5000);
        state++;
      }
      break;
    case 4:
      desiredAngle = 180;
      if ((int(heading) > (desiredAngle + OFFSET)) || (int(heading) < (desiredAngle - OFFSET))) {
        analogWrite(LEFT_WHEEL_FORWARD, 0);
        analogWrite(RIGHT_WHEEL_FORWARD, 100);

      } else {
        analogWrite(LEFT_WHEEL_FORWARD, 0);
        analogWrite(RIGHT_WHEEL_FORWARD, 0);
        delay(5000);
        state++;
      }
      break;
    case 5:
      desiredAngle = 270;
      if ((int(heading) > (desiredAngle + OFFSET)) || (int(heading) < (desiredAngle - OFFSET))) {
        analogWrite(LEFT_WHEEL_FORWARD, 0);
        analogWrite(RIGHT_WHEEL_FORWARD, 100);

      } else {
        state = 0;
      }
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
