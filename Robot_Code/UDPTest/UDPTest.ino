/*
  WiFi UDP Send and Receive String

  This sketch wait an UDP packet on localPort using a WiFi shield.
  When a packet is received an Acknowledge packet is sent to the client on port remotePort

  Circuit:
   WiFi shield attached

  created 30 December 2012
  by dlf (Metodo2 srl)

*/


#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 5005;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char sendBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

struct receivedData {
  float robotSpeed;
  float robotTurn;
  float robotAngle;
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
//test variables

void setup() {
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8, 7, 4, 2);

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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

void loop() {

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



  dataOut  = (sendData*)sendBuffer;

  dataOut->robotSpeed = dataIn->robotSpeed;
  dataOut->robotTurn = dataIn->robotTurn;
  dataOut->robotAngle = dataIn->robotAngle;
  dataOut->xPosition = 34;
  dataOut->yPosition = 25;
  dataOut->globalAngle = 0;

  Udp.beginPacket(Udp.remoteIP(), 5005);

  Udp.write((byte*)dataOut, sizeof( sendData));
  Udp.endPacket();
  }
}


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
