/*
   Creation & Computation - Digital Futures, OCAD University
   Kate Hartman / Nick Puckett

   Read a sensor Value and send it to AIO

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

AdafruitIO_Feed *bot1 = io.feed("bot-one");      //This creates the feed and gives it the name "sensor1"
AdafruitIO_Feed *bot2 = io.feed("bot-two");      //This creates the feed and gives it the name "sensor1"
AdafruitIO_Feed *bot3 = io.feed("bot-three");      //This creates the feed and gives it the name "sensor1"
AdafruitIO_Feed *leaderFeed = io.feed("leader");
AdafruitIO_Feed *timeFeed = io.feed("time");

int refreshRate = 2000;                         //free account on AIO is limited to 30 per minute
unsigned long lastRefresh = 0;                  //used to create the sending timer
int errorCounter = 0;

int pin1 = A2;
int pin2 = A3;
int bot1Val;
int bot2Val;
int bot3Val;
int randTime;
int leader = 1;
int getLeader;
int id = 1;
int timePassed;
boolean allCompleted = false;
boolean motorsOn = false;


void setup()
{

  Serial.begin(9600);
  while (! Serial);
  connectToAIO();
}

void loop()
{

  if ((leader == id) && (motorsOn == false)) {
    randTime = random(2000, 4000);
    Serial.println(randTime);
    startMotors();
  }

  if (millis() - lastRefresh >= randTime) {
    Serial.println("time is up");
    // count the amount of time the motors have been on, and if that time is up then turn them off
    stopMotors();
    if (leader == id) {
      timeFeed->save(randTime);
    }
    lastRefresh = millis();
  }

  //updateAIOfeeds();

}

void connectToAIO()
{
  io.connect();  //connect to the server using your login credentials

  Serial.println("Connecting to Server");

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
  if (millis() - lastRefresh >= refreshRate)
  {

    Serial.println(millis());

    io.run(); //update everything on AIO

    lastRefresh = millis();
  }
}

void startMotors() {

  // start the motor of pin1 & pin 2 & set motorsOn to true;
  motorsOn = true;
  Serial.println("startMotors");
  Serial.print(motorsOn);
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);

  bot1->save(1);

  //return motorsOn;
}

void stopMotors() {

  Serial.println("motors off");
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);

  // set motorsOn to false;
  motorsOn = false;

  bot1->save(0);

  //return motorsOn;

}
