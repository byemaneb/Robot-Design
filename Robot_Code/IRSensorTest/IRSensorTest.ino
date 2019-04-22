
#define RIGHT_WHEEL_IR_SENSOR 16
#define LEFT_WHEEL_IR_SENSOR 16


int rightWheelValue = 0;
int leftWheelValue = 0;

void setup() {
  //intialize serial console
  Serial.begin(9600);

  //initailize all ports
  pinMode(RIGHT_WHEEL_IR_SENSOR, INPUT);
  pinMode(LEFT_WHEEL_IR_SENSOR, INPUT);
}

void loop() {
  //read and print RIGHT_WHEEL_IR_SENSOR_VALUE
  Serial.println("RIGHT_WHEEL_IR_SENSOR_VALUE");
  rightWheelValue = digitalRead(RIGHT_WHEEL_IR_SENSOR);   // read the input pin
  Serial.println(rightWheelValue);

  //read and print LEFT_WHEEL_IR_SENSOR_VALUE
  //Serial.println("LEFT_WHEEL_IR_SENSOR_VALUE");
  //leftWheelValue = digitalRead(LEFT_WHEEL_IR_SENSOR);   // read the input pin
  //Serial.println(leftWheelValue);

  // read value ever 2 sec
  //delay(2000);
}
