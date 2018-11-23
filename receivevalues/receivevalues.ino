#include <ESP32Servo.h>

/*
   Creation & Computation - Digital Futures, OCAD University
   Kate Hartman / Nick Puckett

   Read a value from Adafruit IO

   Be sure to first install
   Libraries:
   Adafruit MQTT
   Arduino Http
   Adafruit IO

   You will also need to create your account at https://io.adafruit.com

   Also, because this library is configured differently you input the details of the SSID + password of
   your Wifi here


 *  * ***NOTE***
   When Wifi is enabled you can only use these pins as analog inputs
   A2
   A3
   A4
   A9 - 33
   A7 - 32
*/


#include "AdafruitIO_WiFi.h"



//get this data from your adafruit.io account
//login and click View AIO Key

#define IO_USERNAME "alusiu"
#define  IO_KEY "64a4aa6f984b48b79d236c93333066d8"

//for ocad wifi make sure you have registered the MAC address of your board
//if you are testing this somewhere else, you'll need to update these settings
#define WIFI_SSID "ocadu-embedded"
#define WIFI_PASS "internetofthings"


AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *rightwheelFeed = io.feed("rightwheel");      //This creates the feed and gives it the name "sensor1"
AdafruitIO_Feed *leftwheelFeed = io.feed("leftwheel");      //This creates the feed and gives it the name "sensor1"

AdafruitIO_Feed *pauseFeed = io.feed("pause");      //This creates the feed and gives it the name "sensor1"

//This creates the feed and gives it the name "sensor1"
//if the feed already exists it connects you to it.
int refreshRate = 2000;
int rightWheel = 500;                         //free account on AIO is limited to 30 per minute
int leftWheel = 500;
int pauseTime = 500;
int pauseTime2 = 500;

boolean pause1 = false;
boolean pause2 = false;

//free account on AIO is limited to 30 per minute
//free account on AIO is limited to 30 per minute
unsigned long lastRefresh = 0;                  //used to create the sending timer
unsigned long lastRefresh2 = 0;                  //used to create the sending timer
unsigned long lastRefresh3 = 0;
unsigned long lastRefresh4 = 0;//used to create the sending timer

int errorCounter = 0;

#define SERVO_PIN_LEFT 33
#define SERVO_PIN_RIGHT 32

//this variable will hold the value we read from AIO

Servo servoleft;
Servo servoright;




void setup()
{


  Serial.begin(9600);

  servoright.setPeriodHertz(50);
  servoleft.setPeriodHertz(50);

  //servoright.attach(SERVO_PIN_RIGHT);
  //servoleft.attach(SERVO_PIN_LEFT);

  connectToAIO();                           //connect to the service
}

void loop()
{
  updateAIOfeeds();

}

void connectToAIO()
{
  io.connect();  //connect to the server using your login credentials
  Serial.println("Connecting to Server");

  leftwheelFeed->onMessage(readIncomingLeft);

  rightwheelFeed->onMessage(readIncomingRight);  ///connect the feed to a callback function to handle incoming data

  pauseFeed->onMessage(readIncomingPause);

  //update the status in the console as it attempts to make a connection
  while (io.status() < AIO_CONNECTED)
  {
    Serial.print("-notYet-");
    delay(500);
    errorCounter++;

    if (errorCounter >= 10)
    {
      Serial.println("**Something isn't working**");
    }

  }
  Serial.println("***Success***");  //Successfully connected
  Serial.println(io.statusText());

}


void updateAIOfeeds()
{

  if (millis() - lastRefresh >= rightWheel) {
    Serial.println("right on!");
    servoright.detach();

    if (rightWheel > leftWheel) {
      if (millis() -  lastRefresh3 >= pauseTime) {
        lastRefresh = millis();
      }
    } else {
      if (millis() -  lastRefresh3 >= pauseTime2) {
        lastRefresh = millis();
      }
    }

  } else {

    servoright.attach(SERVO_PIN_RIGHT);
    lastRefresh3 = millis();
    forward();
    Serial.println("right off!");
  }

  if (millis() - lastRefresh2 >= leftWheel) {
    Serial.println("left off!");
    servoleft.detach();
    if (rightWheel > leftWheel) {
      if (millis() -  lastRefresh4 >= pauseTime) {
        lastRefresh2 = millis();
      }
    } else {
      if (millis() -  lastRefresh4 >= pauseTime2) {
        lastRefresh2 = millis();
      }
    }

  } else {
    Serial.println("left on!");
    servoleft.attach(SERVO_PIN_LEFT);
    lastRefresh4 = millis();
    backward();

  }

  io.run(); //update everything on AIO

}

void readIncomingRight(AdafruitIO_Data * inData)  ///callback function for handling the incoming data
//this was defined inside the connectAIO function.  line 82
{
  rightWheel = atol(inData->value());
  Serial.print("Incoming Value Right: ");
  Serial.println(rightWheel);

}


void readIncomingLeft(AdafruitIO_Data * inData)  ///callback function for handling the incoming data
//this was defined inside the connectAIO function.  line 82
{
  leftWheel = atol(inData->value());
  Serial.print("Incoming Value Left: ");
  Serial.println(leftWheel);

}

void readIncomingPause(AdafruitIO_Data * inData)  ///callback function for handling the incoming data
//this was defined inside the connectAIO function.  line 82
{
  pauseTime = atol(inData->value());

  if (rightWheel > leftWheel) {
    pauseTime2 = (rightWheel - leftWheel) + pauseTime;
  } else {
    pauseTime2 = (leftWheel - rightWheel) + pauseTime;
  }
  Serial.print("Incoming Value Left: ");
  Serial.println(pauseTime);

}

void forward() {
  servoright.write(360);
  servoright.write(0);
}

void backward() {
  servoleft.write(0);
  servoleft.write(360);
}
