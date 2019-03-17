/*Name:_______________________________________Berket Yemaneberhane
   HW:________________________________________ECE-370: Full H-Bridge
   Date:______________________________________2/18/19
   honor Code:_____“On my honor, I have neither given nor received unauthorized aid on this assignment, and I pledge that I am in compliance with the GMU Honor System.”
*/

#define SERIAL_BAUD 9600              // serial baud rate       
#define PIN_A     10                // pin for forward motor direction
#define PIN_B     6               // pin for reverse motor direction
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

void setup() {
  Serial.begin(SERIAL_BAUD);          // initialize baud rate for serial
  
  pinMode(PIN_A, OUTPUT);                     // sets the digital pin as output
  pinMode(PIN_B, OUTPUT);                     // sets the digital pin as output
  
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
}

void loop() {
  switch (OPTION) {
    // case 1: change the PWM duty cycle
  case 1:
      if (PWM_Q == 0) {
        Serial.println("Please enter a duty cycle between 0-100% or 555 to return to main menu");               
        PWM_Q = 1;
        temp++;
      }

      SPEED_DONE = Convert();
      if (SPEED_DONE == ERROR ) {
        break;
      } else if (SPEED_DONE == EXIT_CODE) {
        OPTION = 0;
        QUES = 0;
        PWM_Q = 0;
        SPEED_DONE = 0;
        break;
      } else  if (SPEED_DONE > MAX_INPUT || SPEED_DONE < MIN_INPUT) {
        Serial.println("Please enter a duty cycle between 0-100% or 555 to return to main menu");               
      } else if (Duty_Cycle != SPEED_DONE && ERROR != 1) {
        Duty_Cycle =  SPEED_DONE;
        convertDutyCycle(Duty_Cycle);

        OPTION = 0;
        QUES = 0;
        PWM_Q = 0;
        SPEED_DONE = 0;
        //Serial.println("this is the duty cycle percent");                                  
        //Serial.println( Duty_Cycle);

      } else if (Duty_Cycle == SPEED_DONE) {
        Serial.println("Please enter a new speed or 555 to return to main menu ");               
      }

      break;

    // case 1: change motor direction
    case 2:

      if (DIR_Q == 0 ) {
        Serial.println("Please enter 1 to turn motor off or 2 to go reverse or 3 to go forward");                                  
        DIR_Q = 1;
        temp2++;
      }

      DIR_DONE = Convert();                                   


      if ((DIR_DONE == 1 || DIR_DONE == 2 || DIR_DONE == 3) && (MOTOR_DIRECTION != DIR_DONE) ) { MOTOR_DIRECTION = DIR_DONE;
      changeDirection(MOTOR_DIRECTION);
      
    //reset values
    OPTION = 0;
        QUES = 0;
        DIR_Q = 0;
        DIR_DONE = 0;
        //Serial.println("this is the direction");                                  
        //Serial.println( MOTOR_DIRECTION);
      }

      break;

  // case 1: Run motor
    case 3:
      //Serial.println("entered case 3");                                  // print value
      turnMotor();
      break;
    
  
  // case default: main menu to ask questions
  default:
      if (temp > 0 && temp2 > 0) {
        OPTION = 3;
      }

      if (QUES != 0) {
        OPTION = Convert();
        if (OPTION != 0 && OPTION != 1 && OPTION != 2 && OPTION != ERROR)  {
          Serial.println("\nEnter 1 to change speed or enter 2 to change direction");                                  
        }
      }
      else {
    
    // menu options
        delay(3000);
        Serial.println("\nEnter 1 to change speed or enter 2 to change direction");                                  
        Serial.println("current speed:");                                  
        Serial.println(Duty_Cycle);                                  
        Serial.println("current direction:");                                  
        Serial.println(MOTOR_DIRECTION);                                  

      }
      QUES = 1;
      break;

  } // end switch
}// end loop 

// convert inputed serial data as a integer value
int Convert(void) {
  int incomingByte = 0;
  int integerValue = 0;
  if (Serial.available() > 0) {                                                   // something came across serial
    while (1) {                                                                   // force into a loop until 'n' is received
      incomingByte = Serial.read();                                               // byte that was read
      if (incomingByte == '\n') break;                                            // exit the while(1), we're done receiving
      integerValue *= 10;                                                         // shift left 1 decimal place
      integerValue = ((incomingByte - 48) + integerValue);                        // convert ASCII to integer, add, and shift left 1 decimal place
    }
    return integerValue;
  } else {
    return  ERROR;
  }
}


void convertDutyCycle(int duty_cycle) {
  Serial.println("i made it to the convertor");  
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
