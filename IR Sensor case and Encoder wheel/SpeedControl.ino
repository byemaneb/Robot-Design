
/*Name:_______________________________________Berket Yemaneberhane
   HW:________________________________________ECE-370: ECE-370: Distance Sensor Hookup to uController and "Speed" Encoder Wheel 
   Date:______________________________________3/4/19
   honor Code:_____“On my honor, I have neither given nor received unauthorized aid on this assignment, and I pledge that I am in compliance with the GMU Honor System.”
*/

#define TIMEWINDOWSIZE 10
#define DPT 2.4
#define MOTOR_PIN 10
#define IR_SENSOR_Interuppt_PIN 4
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

static unsigned long currentTime;  // used for setting window

static int i_w;  // increment from time window

static int on;

static int off;

int FREQ_US;

void setup() {
  Serial.begin(9600);
  attachInterrupt(IR_SENSOR_Interuppt_PIN, setTimeWindow, RISING);
  pinMode(MOTOR_PIN, OUTPUT);                     // sets the digital pin as output
  pinMode(IR_SENSOR_Interuppt_PIN, INPUT);
  setRunTime = 10000000;  // runtime
  desiredSpeed = 0 ;
  currentSpeed = 0;
  kp = 1 ; // proportional gain

  FREQ_US =  (MICRO_US / FREQ_HZ); // set up the frequency

  memset (timeWindow, 0, sizeof(timeWindow));

}

void loop() {
  tick = micros();                // get time at the start of the loop
  //Serial.println(tick);           //prints time since program started

  GetSpeed(timeWindow);

  DesiredSpeed();

  error = desiredSpeed - currentSpeed;  // this will be the error for the speed

  correctedSpeed = kp * error ;         // corrected speed
  //Serial.println(correctedSpeed);
  //turnMotor();           // set motor with the corrected speed

  //Serial.println(correctedSpeed);



  tock = micros();                   // get time at the end of the loop
  Serial.println(tock);              //prints time since program started
  sleepTime = setRunTime - (tock - tick); // calculate the total time for the program to run and how much time it has left to run
  delayMicroseconds(sleepTime);      // set time for the program to sleep

}

//get current speed
void GetSpeed(unsigned long timeWindow[]) {
  //Serial.print("im in current speed");

  int i = 0;
  unsigned long averageTime = 0;
  while (i < TIMEWINDOWSIZE) {
    averageTime += timeWindow[i];               // add up the total window time
    i++;
  }
  averageTime = averageTime / TIMEWINDOWSIZE;       // calculate average windowtime
  
  currentSpeed = (int)(DPT * (1 / averageTime));
  Serial.print(currentSpeed);
}


//Desired Speed
void DesiredSpeed() {
  if (Serial.available() > 0) {
    desiredSpeed = Convert();

  }
}

//setTimeWindow
void setTimeWindow() {
  Serial.print("im in the set time window");

  currentTime = micros();           // set cureent time
  timeWindow[i_w] = currentTime;    // insert current time into time window
  i_w ++;                           // increment time window frame


  if (i_w >= TIMEWINDOWSIZE) {
    i_w = 0;                        // reset time window frame it max fram is reached
  }

}

//setMotor
void  turnMotor() {
  while (true) {
    digitalWrite(MOTOR_PIN, HIGH);                                  // set motor on
    delayMicroseconds(500);                                       // keep signal on

    digitalWrite(MOTOR_PIN, LOW);                                   // set motor on
    delayMicroseconds(500);                                        // keep signal off
    Serial.print("motor on");
  }
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
