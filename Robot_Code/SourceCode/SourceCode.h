#ifndef TEST_H
#define TEST_H
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <LSM303.h>

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
#define ROBOT_WIDTH 95     //length between wheels
#define WHEEL_RADIUS 15    // wheel radius in cm
#define TICKS_PER_ROTATION 150   // gear ratio relavent to output shaft
#define REFRESH_TIME 1000     // amount of time to capture the number of ticks (ms)
#define PI 3.1415926535897932384626433832795    // Pie
#define MATRIX_SIZE 4

//use for imu pickup
#define TIMEWINDOWSIZE  50
#define THRESHOLD 150

//use for heading
#define OFFSET  2

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
char sendBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back
WiFiUDP Udp;

struct receivedData {
  float robotSpeed;
  float robotTurn;
  float robotAngle;
  bool returnData;
};

receivedData *dataIn;

struct sendData {
  float robotSpeed = 0;
  float robotTurn = 0;
  float robotAngle = 0;
  float xPosition = 0;
  float yPosition = 0;
  float globalAngle = 0;
};

sendData *dataOut;
//use for driving
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


// use for pick up

char report[80];

int long timeWindow[TIMEWINDOWSIZE]; // Time window array
int long previousAcceleration = 0;  // used for setting window
int long currentAcceleration = 0;  // used for setting window
int long loggedTime = 0;  // used for setting window
int i_w = 0;  // increment from time window
int previousAverageAcceleration = 0;  // increment from time window
int state = 0;

bool pickup = false;

// use for orientation
float globalAngle = 0;

float transformMatrixRight[4][4] = {
  {cos(phi), -sin(phi), 0, 0},
  {sin(phi), cos(phi), 0, robotRadius},
  {0, 0, 0, 0},
  {0, 0, 0, 1},
};

float transformMatrixLeft[4][4] = {
  {cos(-phi), -sin(-phi), 0, 0},
  {sin(-phi), cos(-phi), 0, -robotRadius},
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
float north = 0 ;
float south = (180 / 360) * 2 * PI;
float east = (360 / 360) * 2 * PI;
float west = (90 / 360) * 2 * PI;

float currentRotationAngle = 0;

LSM303 compass;
/////////////////////////////////////////////////////////////////////////////////////
float xPosition = 0 ;
float yPosition = 0;

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

void outputUDP() {
  dataOut  = (sendData*)sendBuffer;

  dataOut->robotSpeed = dataIn->robotSpeed;
  dataOut->robotTurn = dataIn->robotTurn;
  dataOut->robotAngle = dataIn->robotAngle;
  dataOut->xPosition = transformMatrix[0][3];
  dataOut->yPosition = transformMatrix[1][3];
  dataOut->globalAngle = globalAngle;

  Udp.beginPacket(Udp.remoteIP(), 5005);

  Udp.write((byte*)dataOut, sizeof( sendData));
  Udp.endPacket();
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

    dataIn  = (receivedData*)packetBuffer;
    Udp.read( (byte *) &dataIn, 8);      // read the data packet

    //print incoming values
    Serial.println("variable 1");
    Serial.println(dataIn->robotSpeed);
    Serial.println("variable 2");
    Serial.println(dataIn->robotTurn);

    if (len > 0) packetBuffer[len] = 0;

    if ((dataIn->returnData) == true) {
      outputUDP();
      dataIn->returnData = false;
    }
  }
}

// this will either drive the Robot forward, left or right or stop

void driveRobot() {
  if (dataIn->robotTurn == 0) {
    dataIn->robotTurn = 0;
    rightWheelTick = 0;
    leftWheelTick = 0;
    analogWrite(LEFT_WHEEL_FORWARD, dataIn->robotSpeed);
    analogWrite(RIGHT_WHEEL_FORWARD, dataIn->robotSpeed);
  } else  if (dataIn->robotTurn == 1) {
    if (leftWheelTick < 500) {
      analogWrite(LEFT_WHEEL_FORWARD, dataIn->robotSpeed);
      analogWrite(RIGHT_WHEEL_FORWARD, STOP);
    } else {
      dataIn->robotTurn = 0;
    }
  } else if (dataIn->robotTurn == -1) {
    if (rightWheelTick < 500) {
      analogWrite(LEFT_WHEEL_FORWARD, STOP);
      analogWrite(RIGHT_WHEEL_FORWARD, dataIn->robotSpeed);
    }  else {
      dataIn->robotTurn = 0;

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

void rightWheelInterrupt(void) {
  rightWheelTick++;
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

void leftWheelInterrupt(void) {
  leftWheelTick++;
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

  // compass
  Wire.begin();
  compass.init();
  compass.enableDefault();

  compass.m_min = (LSM303::vector<int16_t>) {
    -2058, -2014, -2626
  };
  compass.m_max = (LSM303::vector<int16_t>) {
    +3026, +3968, +3617
  };
  
  memset (timeWindow, 0, sizeof(timeWindow));
}

void pickUP() {
  compass.read();

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

  if (changeInAcceleration > THRESHOLD) {
    dataIn->robotSpeed = STOP;
  }

  previousAverageAcceleration = averageAcceleration;
}

void orientation() {
  compass.read();
  float heading = compass.heading();
  globalAngle = heading;
  Serial.println("heading");
  Serial.println(heading);
}

void rotate() {
  if ((globalAngle > (dataIn->robotAngle + OFFSET)) || (globalAngle < (dataIn->robotAngle - OFFSET))) {
    analogWrite(LEFT_WHEEL_FORWARD, STOP);
    analogWrite(RIGHT_WHEEL_FORWARD, 90);
  } else {
    analogWrite(LEFT_WHEEL_FORWARD, STOP);
    analogWrite(RIGHT_WHEEL_FORWARD, STOP);
    delay(2000);
    dataIn->robotAngle = 0;
  }

}
#endif
