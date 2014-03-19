#include <WiFlyHQ.h>
#include <Servo.h> 

// WiFi credentials
const char wifiSSID[] = "PopGun";
const char wifiPassword[] = "glassynet289";

// Server & port to connect to
const char server[] = "192.168.1.120";
const int port = 9001;

WiFly wifly;


//where incoming data will be stored
int rotationVal;
int colorVal;

Servo hatServo;  // create servo object to control a servo  
int pos = 0;    // variable to store the servo position 

//RGB LED 
int redPin = 3;
int greenPin = 5;
int bluePin = 6; 

String readString;


void setup() 
{ 
  Serial.begin(9600);

  hatServo.attach(9);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Begin serial ports
  wifly.begin(&Serial);

  /* Setup the WiFly to connect to a wifi network */
  wifly.setSSID(wifiSSID);
  wifly.setPassphrase(wifiPassword);
  wifly.setJoin(WIFLY_WLAN_JOIN_AUTO);
  wifly.enableDHCP();
  wifly.save();
  wifly.join();

  wifly.setDeviceID("Wifly-TCP");
  wifly.setIpProtocol(WIFLY_PROTOCOL_TCP);

  // Close old connection if active
  if (wifly.isConnected()) {
    wifly.close();
  }

  // Connect initially before we hit the run loop
  wifly.open(server, port);

} 



void loop() {
  //Read incoming data via Serial connection

  if (wifly.isConnected() == false) {
    wifly.open(server, port);
  } 
  else {
    int available = wifly.available();

    if (available < 0) {
      //Serial.println("Disconnected");
    } 
    else if (available > 0) {

      char c = Serial.read(); //gets one byte from serial buffer

      if (c == '\n') {
        for (int i = 0; i < readString.length(); i++) {
          if (readString.substring(i, i+1) == ",") {
            String rot = readString.substring(0, i);
            String col = readString.substring(i+1);
            rotationVal = rot.toInt();
            colorVal = col.toInt();
            break;
          }
        }

        readString = "";
      } 
      else {
        readString += c; //makes the string readString
      }
    }
  }

  //SERVO MOTOR
  hatServo.write(rotationVal);

  //RGB LED
  analogWrite(redPin, colorVal);
  analogWrite(greenPin, colorVal);
  analogWrite(bluePin, 255-colorVal);

}







