#ifndef TEST_H
#define TEST_H
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

#define SECRET_SSID "Berket"
#define SECRET_PASS "Berket12"

#define RIGHT_WHEEL_IR_SENSOR 16
#define LEFT_WHEEL_IR_SENSOR 15

#define LEFT_WHEEL_FORWARD 10
#define LEFT_WHEEL_REVERSE 11
#define RIGHT_WHEEL_FORWARD 12
#define RIGHT_WHEEL_REVERSE 13
#define ANGLE 115
#define STOP 0

// use for matrix multiplication
#define ROBOT_WIDTH 100     //length between wheels
#define WHEEL_RADIUS 20    // wheel radius in cm
#define TICKS_PER_ROTATION 150   // gear ratio relavent to output shaft
#define REFRESH_TIME 1000     // amount of time to capture the number of ticks (ms)
#define PI 3.1415926535897932384626433832795    // Pie
#define MATRIX_SIZE 4




/*SBC: Code for raspberry Pi
   connect2port()
   initalizeStructure(X)
   loop()
      a = TerminalInput() #block until a new user input
     (V, Delta )= Parse (a)
      X = (V,Delta) # this is just the structcute
      send(X)
*/

/*Microcontroller code
   Setup:
      #AccessPoinr()
      #OpenPort()
      #initailizeStructure()  // this is will be done through the global struct
      #SetIRSensor()
      #motorSetup()
      ****IMUSetup()
      #debugSetup() #notblocking

   Loop:
      ****checkIMU()
      #a = checkUDP() #non- blocking
      #(V, Delta )= Parse (a)
      setSpeed(V)
      setDir(Delta) #motorLeft and motorRight are global variables
      ****if(picked up) #motorLeft and motorRight = 0 are global variables
      sleep() # this can basically be your refresh rate
*/

//use for wifi connection
int status = WL_IDLE_STATUS;
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)


//set up UDP port
unsigned int localPort = 5005;      // local port to listen on
char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back
WiFiUDP Udp;

// this will be the data structure used to store all our data
struct dataStructure {
  float robotSpeed = 0;
  float robotTurn = 0;
  float robotAngle = 0;
};

// create an instance of the data structure
dataStructure *robotData;

//test variables
int rightWheelValue = 0;
int leftWheelValue = 0;
int robotDirection = 0;
int rightWheelTick = 0;
int leftWheelTick = 0;

// use for transform
float wheelDistancePerTick = (2 * PI *  WHEEL_RADIUS) / TICKS_PER_ROTATION;
float cRobot = 2 * PI *  ROBOT_WIDTH;
float phi = (wheelDistancePerTick / cRobot) * 2 * PI;
float robotRadius = ROBOT_WIDTH / 2;
float xDistance = sin(phi) * robotRadius;
float yDistance = cos(phi) * robotRadius;
float temp1, temp2;
int i, j, k;

float transformMatrixRight[4][4] = {
  {cos(phi), -sin(phi), 0, xDistance},
  {sin(phi), cos(phi), 0, yDistance},
  {0, 0, 0, 0},
  {0, 0, 0, 1},
};

float transformMatrixLeft[4][4] = {
  {cos(-phi), -sin(-phi), 0, -xDistance},
  {sin(-phi), cos(-phi), 0, -yDistance},
  {0, 0, 0, 0},
  {0, 0, 0, 1},
};

float transformMatrix[4][4] = {
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1},
};

// compass
float north = (0 / 360)* 2 * PI;
float south = (180 / 360)* 2 * PI;
float east = (360 / 360)* 2 * PI;
float west = (90 / 360)* 2 * PI;

float currentRotationAngle = 0;

/////////////////////////////////////////////////////////////////////////////////////


// print wifi details
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

// connect to wifi
void connectWifi() {
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8, 7, 4, 2);

  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}

//  this will read and parse the incoming data
void readUDP() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 512);

    robotData  = (dataStructure*)packetBuffer;
    Udp.read( (char *) robotData, sizeof( robotData));      // read the data packet

    Serial.println("speed");
    Serial.println(robotData->robotSpeed);
    Serial.println("turn");
    Serial.println(robotData->robotTurn);
    Serial.println("angle");
    Serial.println(robotData->robotAngle);

    if (len > 0) packetBuffer[len] = 0;

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), 5005);
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}






// this will either drive the Robot forward, left or right or stop

void driveRobot() {
   // Serial.println("speed");
    //Serial.println(robotData->robotSpeed);
    Serial.println("turn");
    Serial.println(robotData->robotTurn);
    //Serial.wprintln("angle");
    //Serial.println(robotData->robotAngle)

  if (robotData->robotTurn == 0) {
    robotData->robotTurn = 0;
    rightWheelTick =0;
    leftWheelTick =0;
    analogWrite(LEFT_WHEEL_FORWARD, robotData->robotSpeed);
    analogWrite(RIGHT_WHEEL_FORWARD, robotData->robotSpeed);
  } else  if (robotData->robotTurn == 1) {
    if (leftWheelTick < 200) {
      analogWrite(LEFT_WHEEL_FORWARD, robotData->robotSpeed);
      analogWrite(RIGHT_WHEEL_FORWARD, STOP);
    } else {
      robotData->robotTurn = 0;
    }
  } else if (robotData->robotTurn == -1) {
    if (rightWheelTick < 200) {
      analogWrite(LEFT_WHEEL_FORWARD, STOP);
      analogWrite(RIGHT_WHEEL_FORWARD, robotData->robotSpeed);
    }  else {
      robotData->robotTurn = 0;
      
    }
  }
}

// print matrix
void printMatrix( const float matrix[][MATRIX_SIZE] ) {
  // loop through array's rows
  for ( int i = 0; i < MATRIX_SIZE; ++i ) {
    // loop through columns of current row
    for ( int j = 0; j < MATRIX_SIZE; ++j ) {
      Serial.print (matrix[ i ][ j ] );
      Serial.print (" " );
    }
    Serial.println (" " );
  }
  Serial.println (" " );
}

void transformRight(void) {
  float resultMatrix[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  };

  //matrix multiplication
  for (i = 0; i < MATRIX_SIZE; i++) {
    for (j = 0; j < MATRIX_SIZE; j++) {
      resultMatrix[i][j] = 0;
      for (k = 0; k < MATRIX_SIZE; k++) {
        temp1 = transformMatrix[i][k];
        temp2 = transformMatrixRight[k][j];
        resultMatrix[i][j] += temp1 * temp2 ;
      }
    }
  }
  // assign transform to global transform
  for (i = 0; i < MATRIX_SIZE; i++) {
    for (j = 0; j < MATRIX_SIZE; j++) {
      transformMatrix[i][j] = resultMatrix[i][j];
    }
  }
}

void transformLeft(void) {
  float resultMatrix[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  };

  //matrix multiplication
  for (i = 0; i < MATRIX_SIZE; i++) {
    for (j = 0; j < MATRIX_SIZE; j++) {
      resultMatrix[i][j] = 0;
      for (k = 0; k < MATRIX_SIZE; k++) {
        temp1 = transformMatrix[i][k];
        temp2 = transformMatrixLeft[k][j];
        resultMatrix[i][j] += temp1 * temp2 ;
      }
    }
  }
  // assign transform to global transform
  for (i = 0; i < MATRIX_SIZE; i++) {
    for (j = 0; j < MATRIX_SIZE; j++) {
      transformMatrix[i][j] = resultMatrix[i][j];
    }
  }
}

void leftWheelInterrupt() {
  leftWheelTick++;
  //transformLeft;
}

void rightWheelInterrupt() {
  rightWheelTick++;
  //transformRight;
}

void orientRobot(){
  
}

void initializePorts() {
  //intialize serial console
  Serial.begin(9600);

  //initialize IRsensors
  pinMode(RIGHT_WHEEL_IR_SENSOR, INPUT_PULLUP);
  pinMode(LEFT_WHEEL_IR_SENSOR, INPUT_PULLUP);

  // initialize wheels
  pinMode(LEFT_WHEEL_FORWARD, OUTPUT);
  pinMode(LEFT_WHEEL_REVERSE, OUTPUT);
  pinMode(RIGHT_WHEEL_FORWARD, OUTPUT);
  pinMode(RIGHT_WHEEL_REVERSE, OUTPUT);

  //Stop robot
  analogWrite(LEFT_WHEEL_REVERSE, STOP);
  analogWrite(RIGHT_WHEEL_REVERSE, STOP);
  analogWrite(LEFT_WHEEL_FORWARD, STOP);
  analogWrite(RIGHT_WHEEL_FORWARD, STOP);

  // set innterupts for IR sensors
  attachInterrupt(digitalPinToInterrupt(RIGHT_WHEEL_IR_SENSOR), leftWheelInterrupt, RISING); // attach interupt to a function
  attachInterrupt(digitalPinToInterrupt(LEFT_WHEEL_IR_SENSOR), rightWheelInterrupt, RISING); // attach interupt to a function

}

//test function
void testFunction() {

  driveRobot();
  
    Serial.println ("netTransformMatrix" );
    printMatrix(transformMatrix);
  
}
#endif
