/* Name: Berket Yemaneberhane
   Date: 4/8/19
   Assignment: Odometry
*/


/////////////////////////////////////////////////////////////////////////////////////

/* global variables*/

//length between wheels
#define ROBOT_WIDTH 2

// wheel radius in cm
#define WHEEL_RADIUS 2

// gear ratio relavent to output shaft
#define TURNS_REV 75

// number of ticks per revolution
#define TICK_REV  2

// amount of time to capture the number of ticks (ms)
#define REFRESH_TIME 1000

// Pie
#define PI 3.1415926535897932384626433832795

/////////////////////////////////////////////////////////////////////////////////////

/*variables that are used */

// angles for rotation of robot
int rightAngle = 0;
int leftAngle = 0;
static int thetaZ = 0;

// change in x and y direction from left and right wheel
int rightThetaX = 0;
int rightThetaY = 0;
int leftThetaX = 0;
int leftThetaY = 0;

// robot position
static int xPosition = 0;
static int yPosition = 0;
static int zPosition = 0;

// number of ticks
int rightTick = 0;
int leftTick = 0;

//distance
int distancePerTick;
int distanceTraveledRight = 0;
int distanceTraveledLeft = 0;

// direction robot is traveling 1 = forward  -1 = backward  0 = stopped
int robotDirection = 0;

//time
unsigned long timeTick;
unsigned long timeTock;
unsigned long timeCurrent;

//
int matrixSize = 4;


/////////////////////////////////////////////////////////////////////////////////////
/* initialization and code*/
//initializations
void setup() {
  // calulat the distance per tick
  distancePerTick = (2 * PI * WHEEL_RADIUS) / (TURNS_REV * TICK_REV);
}

// implemented codes
void loop() {
  // This will calculate the change in time, and basically run an update based on the refresh time
  timeTick = millis();
  while (timeTock < REFRESH_TIME ) {
     matrix();
     analytical();
    
    timeCurrent = millis();
    timeTock = timeCurrent - timeTick;
  }

}


/////////////////////////////////////////////////////////////////////////////////////

/* methods for calculating distance, angle, and change in the x and y directions*/

// calculate the distance wheel has traveled
int distance(int ticks) {
  int distance = 0;
  distance = ticks * distancePerTick;
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
  // matrix
  int positionMatrix[4][4] = {
    {1, 0, 0, xPosition},
    {0, 1, 0, yPosition},
    {0, 0, 1, zPosition},
    {0, 0, 0, 1},
  };

  int directionMatrix[4][4] = {
    {1, 0, 0, (WHEEL_RADIUS / 2)},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  };

  int rotationMatrix[4][4];
  int transformMatrix[4][4];
  int temp[4][4];

  // distance right and left wheel traveled
  distanceTraveledRight = distance(rightTick);
  distanceTraveledLeft = distance(leftTick);

  // angle of right and left wheel
  rightAngle = angle(distanceTraveledRight);
  leftAngle = angle(distanceTraveledLeft);

  thetaZ = rightAngle - leftAngle;
  
  rotationMatrix[4][4] = {
    {cos(thetaZ), -sin(thetaZ), 0, 0 },
    {sin(thetaZ), cos(thetaZ), 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  };

  //transformMatrix = (rotationMatrix * currentDirection * currentPosition) 
  // this will claculate the new posistion of the robot
    matrixMultiplication(currentPosition,directionMatrix,temp);       
    matrixMultiplication(temp,rotationMatrix,transformMatrix);

  // we then need to update the posistion of the robot
  //update x posistion
  xPosition = transformMatrix[1][4];

  //update y posistion
  yPosition = transformMatrix[2][4];

  //update z posistion
  zPosition = transformMatrix[3][4];

}

// multiply 2 matrix of any size
void matrixMultiplication(int mat1[][matrixSize], int mat2[][matrixSize], int res[][matrixSize]){ 
    int i, j, k; 
    for (i = 0; i < matrixSize; i++) { 
        for (j = 0; j < matrixSize; j++) { 
            res[i][j] = 0; 
            for (k = 0; k < NmatrixSize; k++) 
                res[i][j] += mat1[i][k] * mat2[k][j]; 
        } 
    } 
} 
