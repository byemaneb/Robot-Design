/* Name: Berket Yemaneberhane
   Date: 4/8/19
   Assignment: Odometry
*/


/////////////////////////////////////////////////////////////////////////////////////

/* global variables*/

//length between wheels
#define ROBOT_WIDTH 8

// wheel radius in cm
#define WHEEL_RADIUS 2

// gear ratio relavent to output shaft
#define TICKS_PER_ROTATION 75

// number of ticks per revolution
#define TICK_REV  2

// amount of time to capture the number of ticks (ms)
#define REFRESH_TIME 1000

// Pie
#define PI 3.1415926535897932384626433832795

#define MATRIX_SIZE 4

/////////////////////////////////////////////////////////////////////////////////////

/*variables that are used */

// angles for rotation of robot
int rightAngle = 0;
int leftAngle = 0;
float thetaZ = 0;

// change in x and y direction from left and right wheel
int rightThetaX = 0;
int rightThetaY = 0;
int leftThetaX = 0;
int leftThetaY = 0;

// robot position
int xPosition = 0;
int yPosition = 0;
int zPosition = 0;

// number of ticks
int rightTick = 150;
int leftTick = 0;

//
float wheelCircumfrence = WHEEL_RADIUS * 2 * PI;
float robotCircumference = ROBOT_WIDTH * 2 * PI;

//distance
int distancePerTick;
float distanceTraveledRight = 0;
float distanceTraveledLeft = 0;
int distanceTraveled = 0;

// direction robot is traveling 1 = forward  -1 = backward  0 = stopped
int robotDirection = 0;

//time
unsigned long timeTick;
unsigned long timeTock;
unsigned long timeCurrent;

//
int matrixSize = 4;
int rows = 4;
int columns = 4;

float positionMatrix[4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  };

  float directionMatrix[4][4] = {
    {1, 0, 0, ROBOT_WIDTH},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  };

/////////////////////////////////////////////////////////////////////////////////////
/* initialization and code*/
//initializations
void setup() {
  // calulat the distance per tick
  distancePerTick = (2 * PI * WHEEL_RADIUS) / (TICKS_PER_ROTATION * TICK_REV);
  positionMatrix[0][0] =1;
  positionMatrix[1][1] =1;
  positionMatrix[2][2] =1;
  positionMatrix[3][3] =1;
}

// implemented codes
void loop() {
  // This will calculate the change in time, and basically run an update based on the refresh time
  /*
  timeTick = millis();
  while (timeTock < REFRESH_TIME ) {
    timeCurrent = millis();
    timeTock = timeCurrent - timeTick;
  }
  */
  matrix();
 // analytical();
  delay(2000);
}
/////////////////////////////////////////////////////////////////////////////////////

/* methods for calculating distance, angle, and change in the x and y directions*/

// calculate the distance wheel has traveled
float distance(int ticks) {
  float distance = 0;
  float numberOfWheelRotations = 0;
  numberOfWheelRotations = ticks / TICKS_PER_ROTATION ;
  return numberOfWheelRotations * wheelCircumfrence;  //return wheel distance
}


// calculate angle of turn
int angle(int distanceTraveled) {
  int angle = 0 ;
  angle = atan(distanceTraveled / ROBOT_WIDTH);
  return angle;

}

// calculates the change in the Y direction
int thetaY(int angle) {
  int thetaY = 0;
  thetaY = (ROBOT_WIDTH / 2) * sin(angle);
}

// calculate the change in the X direction
int thetaX(int angle) {
  int thetaX = 0;
  thetaX = (ROBOT_WIDTH / 2) - ((ROBOT_WIDTH / 2) * cos(angle));
}


/////////////////////////////////////////////////////////////////////////////////////

/* methods for calculating x, y , and theta_z location */

// calcualtions using the analytical method
void analytical(void) {
  // distance right and left wheel traveled
  distanceTraveledRight = distance(rightTick);
  distanceTraveledLeft = distance(leftTick);


  // angle of right and left wheel
  rightAngle = angle(distanceTraveledRight);
  leftAngle = angle(distanceTraveledLeft);

  // set new orientation of robot
  thetaZ = thetaZ + rightAngle - leftAngle;

  //change in the x direction of robot
  rightThetaX = thetaX(rightAngle);
  leftThetaX = thetaX(leftAngle);

  // set new x position
  xPosition = xPosition + leftThetaX - rightThetaX;


  // change in the y direction of robot
  rightThetaY = thetaY(rightAngle);
  leftThetaY = thetaY(leftAngle);

  // set new y position
  yPosition = yPosition + rightThetaY + leftThetaY;

}

// calculateing using the matrix method
void matrix(void) {

  float transformMatrix[4][4]= {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  };

  // distance right and left wheel traveled
  distanceTraveledRight = distance(rightTick);

  distanceTraveledLeft = distance(leftTick);

  // angle of right and left wheel
  rightAngle = matAngle(distanceTraveledRight);

  leftAngle = matAngle(distanceTraveledLeft);

  thetaZ = rightAngle - leftAngle;

 float rotationMatrix[4][4]  = {
    {cos(thetaZ), -sin(thetaZ), 0, 0},
    {sin(thetaZ), cos(thetaZ), 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  };

  // this will claculate the new posistion of the robot
  matrixMultiplication(directionMatrix, rotationMatrix,positionMatrix,transformMatrix);
  
  // we then need to update the posistion of the robot
  
  //update x posistion
  xPosition = transformMatrix[0][3];

  //update y posistion
  yPosition = transformMatrix[1][3];

  //update z posistion
  zPosition = transformMatrix[2][3];

  positionMatrix[0][3] = xPosition;

  //update y posistion
  positionMatrix[1][3] = xPosition;

  //update z posistion
  positionMatrix[2][3] = xPosition;
  

  // print values and see that they are inputted correctly
  printMatrix(positionMatrix);
}

// multiply 2 matrix of any size

void matrixMultiplication(float mat1[][MATRIX_SIZE],float mat2[][MATRIX_SIZE],float mat3[][MATRIX_SIZE],float resultMatrix[][MATRIX_SIZE]) {
  
  int i, j, k;
  float temp1, temp2;
  float temp[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  };
  for (i = 0; i < matrixSize; i++) {
    for (j = 0; j < matrixSize; j++) {
      for (k = 0; k < matrixSize; k++){
        temp1 = mat1[i][k];
        temp2 = mat2[k][j];
        temp[i][j] += temp1 * temp2 ;
       /*
        Serial.print("i");
        Serial.print(i);
        Serial.print("");
        Serial.print("j");
        Serial.print(j);
                Serial.print("k");
        Serial.print(k);
        Serial.println("");
        */

      }
    }
  }
  //printMatrix(temp);
  for (i = 0; i < matrixSize; i++) {
    for (j = 0; j < matrixSize; j++) {
      for (k = 0; k < matrixSize; k++){
        temp1 = temp[i][k];
        temp2 = mat3[k][j];
        resultMatrix[i][j] += temp1 * temp2 ;
      }
    }
  }
  printMatrix(resultMatrix);
}

// print matrix
void printMatrix( const float matrix[][MATRIX_SIZE] ) {
   // loop through array's rows
   for ( int i = 0; i < rows; ++i ) {
      // loop through columns of current row
      for ( int j = 0; j < columns; ++j ){
      Serial.print (matrix[ i ][ j ] );
      Serial.print (" " );
      }
      Serial.println (" " );
   } 
   Serial.println (" " );
}

//angle using thte cercumfrence method
float matAngle(float wheelDistance){
  return  2 *PI * (wheelDistance / robotCircumference);
}
