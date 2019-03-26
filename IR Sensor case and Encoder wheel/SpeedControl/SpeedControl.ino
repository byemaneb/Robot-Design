
/*Name:_______________________________________Berket Yemaneberhane
   HW:________________________________________ECE-370: ECE-370: Distance Sensor Hookup to uController and "Speed" Encoder Wheel
   Date:______________________________________3/4/19
   honor Code:_____“On my honor, I have neither given nor received unauthorized aid on this assignment, and I pledge that I am in compliance with the GMU Honor System.”
*/

#define TIMEWINDOWSIZE 20
#define DPT 2.4                       //degree per turn
#define MOTOR_PIN 10
#define IR_SENSOR_Interuppt_PIN 15
#define FREQ_HZ   1000              // Desired PWM freq(Hz)
#define MICRO_US  1000000             // Number of micro seconds in 1 second

// variables
unsigned long tick;
unsigned long tock;
unsigned long setRunTime;
unsigned long sleepTime;

static int currentSpeed;  // current speed
static int desiredSpeed;  // desired speed
int  error;    // error in speed

int kp;       // proportional gain- set best to manage speed

int correctedSpeed;      //corrected speed

static unsigned long timeWindow[TIMEWINDOWSIZE]; // Time window array

static unsigned long previousTime;  // used for setting window
static unsigned long currentTime;  // used for setting window
static unsigned long loggedTime;  // used for setting window



static int i_w;  // increment from time window

static int on;

static int off;

int FREQ_US;

int out;
int PWM;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(IR_SENSOR_Interuppt_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IR_SENSOR_Interuppt_PIN), setTimeWindow, RISING);

  pinMode(MOTOR_PIN, OUTPUT);                                            // sets the digital pin as output


  setRunTime =350000;  // runtime
  desiredSpeed = 0 ;
  currentSpeed = 0;
  kp = 1 ; // proportional gain

  FREQ_US =  (MICRO_US / FREQ_HZ); // set up the frequency

  memset (timeWindow, 0, sizeof(timeWindow));

  // set time counters
  previousTime = 0;
  currentTime = 0;

}

void loop() {
  tick = micros();                                              // get time at the start of the loop

  //Serial.println("start time");
  //Serial.println(tick);

  GetSpeed(timeWindow);

  DesiredSpeed();

  error = desiredSpeed - currentSpeed;                                // this will be the error for the speed
  //Serial.println(error);

  out = kp * error ;                                       // corrected speed
  //Serial.println(out);
  setMotor(error);
  turnMotor();                                                        // set motor with the corrected speed

  Serial.println("desiredSpeed");

  Serial.println(desiredSpeed);
  
  Serial.println("currentSpeed");

  Serial.println(currentSpeed);

  Serial.println("error");

  Serial.println(error);

  tock = micros();                                                 // get time at the end of the loop

  //Serial.println("finish time");
  // Serial.println(tock);

  sleepTime = setRunTime - (tock - tick);                               // calculate the total time for the program to run and how much time it has left to run

  //Serial.println("sleep time");
  //Serial.println(sleepTime);

  delayMicroseconds(sleepTime);                                                  //prints sleep time
}


//setTimeWindow
void setTimeWindow() {

  currentTime = micros() ;           // set cureent time

  timeWindow[i_w] = (currentTime - previousTime) ;    // insert current time into time window
  previousTime = currentTime;                      // save previous time

  //Serial.println("change of time: ");
  loggedTime = timeWindow[i_w];
  

  i_w ++;                           // increment time window frame

  if (i_w >= TIMEWINDOWSIZE) {
    i_w = 0;                        // reset time window frame it max fram is reached
  }

}

//get current speed
void GetSpeed(unsigned long timeWindow[]) {
  //Serial.println("im in current speed");

  int i = 0;
  unsigned long averageTime = 0;
  while (i < TIMEWINDOWSIZE) {
    averageTime += timeWindow[i];               // add up the total window time
    i++;
  }
  averageTime = averageTime / TIMEWINDOWSIZE;       // calculate average windowtime


  if (averageTime > 0) {
    currentSpeed = (int)(DPT * (MICRO_US / averageTime));
  } else {
    currentSpeed = 0 ;
  }


}

//Desired Speed
void DesiredSpeed() {
  if (Serial.available() > 0) {
    desiredSpeed = Convert();
  }
}

void setMotor(int error ) {
  int newSpeed = 0;
  if(error < 0 && PWM >= 0 ){
    PWM-- ;
  }
  if(error > 0 && PWM < 255 ){
    PWM++;
  }
  Serial.println("PWM");

  Serial.println(PWM);


}

//setMotor
void  turnMotor() {
  analogWrite(MOTOR_PIN, PWM); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
}

// convert inputed serial data as a integer value
int Convert(void) {
  int incomingByte = 0;
  int integerValue = 0;
  while (1) {                                                                   // force into a loop until 'n' is received
    incomingByte = Serial.read();                                               // byte that was read
    if (incomingByte == '\n') break;                                            // exit the while(1), we're done receiving
    integerValue *= 10;                                                         // shift left 1 decimal place
    integerValue = ((incomingByte - 48) + integerValue);                        // convert ASCII to integer, add, and shift left 1 decimal place
  }
  return integerValue;
}

void convertDutyCycle(int duty_cycle) {

  //Serial.println("i made it to the convertor");
  on = int((duty_cycle * FREQ_US) / 100) ;                      // time to keep motor on
  off = int(FREQ_US - on);                                        // time to keep motor off
}
