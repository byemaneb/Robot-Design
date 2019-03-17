/*Name:_______________________________________Berket Yemaneberhane
   HW:________________________________________ECE-370: Motor Driver open loop speed control, one direction PWM
   Date:______________________________________2/12/19
   honor Code:_____“On my honor, I have neither given nor received unauthorized aid on this assignment, and I pledge that I am in compliance with the GMU Honor System.”
*/

#define SERIAL_BAUD 9600              // serial baud rate       
#define PIN_A     10                // pin for forward motor direction
#define PIN_B     9               // pin for reverse motor direction
#define INNER_IR  6
#define OUTER_IR  5
#define FREQ_HZ   1000              // Desired PWM freq(Hz)
#define MAX_INPUT 100               // Max user input allowed
#define MICRO_US  1000000             // Number of micro seconds in 1 second
#define MIN_INPUT 0               // Min user input allowed
#define EXIT_CODE 555               // Code used to send user to main menu
#define ERROR   999               // Code that there was an invalid user input

static int ledPin ;                                 // Motor driver signal connected to digital pin 13
static int on ;                     // time motor on
static int off ;                            // time motor on
static int Duty_Cycle;                // duty cycle in percent
static int MOTOR_DIRECTION;             // motor direction
static int OPTION;                  // menu option




const unsigned int INITAL_INPUT = -1;

int FREQ_US = 0;
int QUES = 0;

int SPEED_DONE = 0;
int PWM_Q = 0;
int DIR_Q = 0;
int DIR_DONE = 0;

int temp;
int temp2;



// this is the added new variables
static int INPUT_VALUE;
int QUESTIONS;
int isObstacle = HIGH;  // HIGH MEANS NO OBSTACLE
int TIC = 0;
int PREVIOUS = 0;
int CURRENT = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);          // initialize baud rate for serial

  pinMode(PIN_A, OUTPUT);                     // sets the digital pin as output
  pinMode(PIN_B, OUTPUT);                     // sets the digital pin as output
  pinMode(INNER_IR, INPUT);
  pinMode(OUTER_IR, INPUT);

  Duty_Cycle = 0;
  MOTOR_DIRECTION = 0;
  OPTION = 0;
  SPEED_DONE = 0;
  PWM_Q = 0;
  DIR_Q = 0;
  DIR_DONE = 0;
  temp = 0;
  temp2 = 0;

  FREQ_US =  (MICRO_US / FREQ_HZ);

  // this is the added new variables
  INPUT_VALUE = 0 ;
  QUESTIONS = 1;
}

void loop() {
  switch (OPTION) {
    // case 1: change the PWM duty cycle
    case 1:

      // ask question
      if (QUESTIONS == 3) {
        QUESTIONS = 1;
        Serial.println("Enter duty cycle 0-100%");
      }


      // wait for input from serial
      if (Serial.available() > 0) {
        Duty_Cycle = Convert();
        convertDutyCycle(Duty_Cycle);
        // test outputs
        Serial.println("on");
        Serial.println(on);
        Serial.println("off");
        Serial.println(off);

        OPTION = 0;     // exit from speed section
      }

      break;

    // case 1: change motor direction
    case 2:
      //Serial.println("change direction");
      if (QUESTIONS == 4) {
        QUESTIONS = 1;
        Serial.println("Enter 1 to turn off motor: 2 to forward: or 3 to go reverse");
      }

      if (Serial.available() > 0) {
        MOTOR_DIRECTION = Convert();

        if (MOTOR_DIRECTION == 1 || MOTOR_DIRECTION == 2 || MOTOR_DIRECTION == 3) {
          changeDirection(MOTOR_DIRECTION);
          // test outputs
          Serial.println("change direction");
          Serial.println(MOTOR_DIRECTION);
          OPTION = 0;     // exit from speed section
          QUESTIONS = 1;
        } else {
          QUESTIONS = 4;
        }

      }

      break;

    // case 1: Run motor
    case 3:

      break;
    // ask questions
    case 4:

      break;

    // case default: main menu to ask questions
    default:
      // ask question - add delay to be able to print to the serial command
      if (QUESTIONS == 1) {
        delay(5000);
        QUESTIONS = 0;
        Serial.println("\nEnter 1 to change speed or enter 2 to change direction");
      } else if (QUESTIONS == 2) {
        //delay(5000);
        //QUESTIONS = 0;
        Serial.println("\nplease enter either 1 to change speed or 2 to change direction");
      }

      // if there is no no input to the serial then run PWM signal for the motor
      if (Serial.available() > 0) {
        INPUT_VALUE = Convert();                    // store the input as a new value

        if (INPUT_VALUE == 1) {
          OPTION = 1;
          QUESTIONS = 3;
        } else if (INPUT_VALUE == 2) {
          OPTION = 2;
          QUESTIONS = 4;
        } else {
          QUESTIONS = 2;
        }


        // test outputs
        Serial.println("Pin ");
        Serial.println(ledPin);

      } else {

        // turn motor on and off
        if (MOTOR_DIRECTION != 1) {
          digitalWrite(ledPin, HIGH);                                  // set motor on
          delayMicroseconds(on);                                       // keep signal on

          digitalWrite(ledPin, LOW);                                   // set motor on
          delayMicroseconds(off);                                        // keep signal off
        }


      }

      PREVIOUS = isObstacle;

      isObstacle = digitalRead(INNER_IR);

      CURRENT = isObstacle;




      break;

  } // end switch
}// end loop

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

//change direction of the motor to either off, forward or reverse
void  changeDirection (int myDirection) {
  if (myDirection == 1) {
    digitalWrite(PIN_B, LOW);                          //set PIN_B off
    digitalWrite(PIN_A, LOW);                          //set PIN_A off
  } else if (myDirection == 2) {
    digitalWrite(PIN_B, LOW);                          //set PIN_B off
    ledPin = PIN_A;                                //set PIN_A on
  } else if (myDirection == 3) {
    digitalWrite(PIN_A, LOW);                          //set PIN_A off
    ledPin = PIN_B;                                //set PIN_B om
  }
}


//turns motor using a digital PWM freqquency
void  turnMotor() {
  while (true) {
    digitalWrite(ledPin, HIGH);                                  // set motor on
    delayMicroseconds(on);                                       // keep signal on

    digitalWrite(ledPin, LOW);                                   // set motor on
    delayMicroseconds(off);                                        // keep signal off
  }
}
