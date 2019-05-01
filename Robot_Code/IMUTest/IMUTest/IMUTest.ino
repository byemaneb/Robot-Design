#include <Wire.h>
#include <LSM303.h>
#define TIMEWINDOWSIZE  50
#define THRESHOLD 150


#define LEFT_WHEEL_FORWARD 10
#define LEFT_WHEEL_REVERSE 11
#define RIGHT_WHEEL_FORWARD 12
#define RIGHT_WHEEL_REVERSE 13
#define ANGLE 115
#define STOP 0

LSM303 compass;

char report[80];

int long timeWindow[TIMEWINDOWSIZE]; // Time window array
int long previousAcceleration = 0;  // used for setting window
int long currentAcceleration = 0;  // used for setting window
int long loggedTime = 0;  // used for setting window
int i_w = 0;  // increment from time window
int previousAverageAcceleration = 0;  // increment from time window
int state =0;

bool pickup = false;
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

  memset (timeWindow, 0, sizeof(timeWindow));
}

void loop() {

  // read in IMU values
  compass.read();
  
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
    analogWrite(LEFT_WHEEL_FORWARD, 100);
    analogWrite(RIGHT_WHEEL_FORWARD, 100);
    break;
  case 2:
    analogWrite(LEFT_WHEEL_FORWARD, 0);
    analogWrite(RIGHT_WHEEL_FORWARD, 0);
    
    delay(5000);
    memset (timeWindow, 0, sizeof(timeWindow));
    state = 1;
    break;
  default:
    // statements
    break;
}

  //check if there is change in the z direction
  getAcceleration();
}

void getAcceleration() {
  
  int i = 0;
  int changeInAcceleration = 0;
  unsigned long averageAcceleration = 0;

  currentAcceleration = compass.a.z ;           // set cureent time
  timeWindow[i_w] = (currentAcceleration) ;    // insert current time into time window
  
  previousAcceleration = currentAcceleration;                      // save previous time

  while (i < TIMEWINDOWSIZE) {
    averageAcceleration += timeWindow[i];               // add up the total window time
    i++;

  }
  averageAcceleration = averageAcceleration / TIMEWINDOWSIZE;
    
 i_w ++;                           // increment time window frame

  if (i_w >= TIMEWINDOWSIZE) {
    i_w = 0;                        // reset time window frame it max fram is reached
  }
  changeInAcceleration = averageAcceleration - previousAverageAcceleration ;
 Serial.println(changeInAcceleration); 
  if (changeInAcceleration > THRESHOLD) {

    state = 2;
  }
    
  previousAverageAcceleration = averageAcceleration;
}
