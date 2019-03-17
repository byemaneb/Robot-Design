// IR Obstacle Collision Detection Module
// Henry's Bench
#define INNER_IR  6
#define OUTER_IR  5
#define ledPin    10
#define ENCODER_REV 75  // number of rotations needed for encoder to rotate motor 360 degrees
#define ENCODER_TICS  4 // four tics per 1 encoder revolution


int isObstaclePin = 5;  // This is our input pin
int isObstacle = HIGH;  // HIGH MEANS NO OBSTACLE
int TIC = 0;
int PREVIOUS = 0;
int CURRENT = 0;
 
int needed_rev = 0;
unsigned long currentMillis = 0;

void setup() {
  //pinMode(LED, OUTPUT);
  pinMode(isObstaclePin, INPUT);
  pinMode(ledPin, OUTPUT);                     // sets the digital pin as output
  //pinMode(PIN_B, OUTPUT);                     // sets the digital pin as output
  Serial.begin(9600);
  TIC = 0;

needed_rev = ENCODER_REV * ENCODER_TICS;
  

}

void loop() {
  currentMillis = millis(); // Store the current time in time 
  Serial.println("begining time");

Serial.println(currentMillis);
  while (true) {
 
    PREVIOUS = isObstacle;
    
    isObstacle = digitalRead(INNER_IR);

    CURRENT = isObstacle;
    
    if( TIC < needed_rev){
      
    digitalWrite(ledPin, HIGH);                                  // set motor on
    delayMicroseconds(800);                                       // keep signal on

    digitalWrite(ledPin, LOW);                                   // set motor on
    delayMicroseconds(200);                                        // keep signal off
    }
    if( TIC > needed_rev){
    digitalWrite(ledPin, LOW);                                   // set motor on
    }
    if (PREVIOUS != CURRENT)
    {
      TIC++;
    }

    //Serial.println(TIC);
  }

  currentMillis = millis(); // Store the current time in time 
  Serial.println("end time");

  Serial.println(currentMillis);

}
