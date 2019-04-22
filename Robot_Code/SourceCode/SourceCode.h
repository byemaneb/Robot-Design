#ifndef TEST_H
#define TEST_H
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

#define SECRET_SSID "Berket"
#define SECRET_PASS "Berket12"

#define RIGHT_WHEEL_IR_SENSOR 16
#define LEFT_WHEEL_IR_SENSOR 16

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
      SetIRSensor()
      motorSetup()
      IMUSetup()
      debugSetup() #notblocking

   Loop:
      checkIMU()
      a = checkUDP() #non- blocking
      (V, Delta )= Parse (a)
      setSpeed(V)
      setDir(Delta) #motorLeft and motorRight are global variables
      if(picked up) #motorLeft and motorRight = 0 are global variables
      sleep() # this can basically be your refresh rate
*/


int status = WL_IDLE_STATUS;      
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 5005;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

// this will be the data structure used to store all our data
struct dataStructure {
  float robotSpeed = 0;
  float robotAngle = 0;
};

// create an instance of the data structure
dataStructure *robotData;

//test variables
int rightWheelValue = 0;
int leftWheelValue = 0;

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

    Serial.println("variable 1");
    Serial.println(robotData->robotSpeed);
    Serial.println("variable 2");
    Serial.println(robotData->robotAngle);

    if (len > 0) packetBuffer[len] = 0;

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), 5005);
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}

void initializePorts(){
    //intialize serial console
  Serial.begin(9600);

  //initailize all ports
  pinMode(RIGHT_WHEEL_IR_SENSOR, INPUT);
  pinMode(LEFT_WHEEL_IR_SENSOR, INPUT);
}

void testFunction(){
    //read and print RIGHT_WHEEL_IR_SENSOR_VALUE
  Serial.println("RIGHT_WHEEL_IR_SENSOR_VALUE");
  rightWheelValue = digitalRead(RIGHT_WHEEL_IR_SENSOR);   // read the input pin
  Serial.println(rightWheelValue);

  //read and print LEFT_WHEEL_IR_SENSOR_VALUE
  Serial.println("LEFT_WHEEL_IR_SENSOR_VALUE");
  leftWheelValue = digitalRead(LEFT_WHEEL_IR_SENSOR);   // read the input pin
  Serial.println(leftWheelValue);

  // read value ever 2 sec
  //delay(2000);
}
#endif
