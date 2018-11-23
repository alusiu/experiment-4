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

AdafruitIO_Feed *leftwheel = io.feed("leftwheel");      //This creates the feed and gives it the name "sensor1"

AdafruitIO_Feed *rightwheel = io.feed("rightwheel"); 

int refreshRate = 2500;
int refreshRate2 = 2500;                         //free account on AIO is limited to 30 per minute
//free account on AIO is limited to 30 per minute
unsigned long lastRefresh = 0;                  //used to create the sending timer
int errorCounter = 0;


int randTime1 = 0;
int randTime2 = 0;



void setup()
{

  Serial.begin(9600);
  while (! Serial);

  connectToAIO();
}

void loop()
{
  if (millis() - lastRefresh >= refreshRate)
  {
    randTime1 = random(1000, 5000);
    randTime2 = random(1000, 5000);

    if (randTime1 > randTime2) {
      refreshRate = randTime1;
    } else {
      refreshRate = randTime2;
    }
    updateAIOfeeds();
  }
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
  if (millis() - lastRefresh >= refreshRate+refreshRate2)
  {
    rightwheel->save(randTime1);     //write the value to the val1 property
    leftwheel->save(randTime2);
    Serial.print("sent right: ");
    Serial.print(randTime1);
    Serial.print("sent left: ");
    Serial.println(randTime2);

    //Serial.println(millis());

    io.run(); //update everything on AIO

    lastRefresh = millis();
  }

}
