/*
* Name: Teamachine
* Author: Miranda Bache
* Date: 2025-02-06
* Description: It makes a cup of tea (it does not add milk or sugar) from start to finish, unassisted (once properly set up).
* The timings ar currently set up for Rooibos tea but can be easily adjusted to other brewing times. Pretty simplu it does this
* by using a servo motor to turn the heat on, using a temperature sensor to detect when the water has boiled, turning the heat off,
* pumping the water into the mug with a small pump, dunking in the teabag with another servo, waiting for it to brew and finally 
* removing the teabag. 
*/


// Include Libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>
#include <RTClib.h>

// construct objects
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
RTC_DS3231 rtc;
Servo myservo1;
Servo myservo2;

// Init global variables
bool start = false;
bool on = false;
int WaterTime = -10;
int TestTemp = 0;
int OnTime = 0;
bool TeaThingsAreHappening = false;

// Init global constants
const int BrewTime = 3;

void setup() {
 // Init Hardware
  pinMode(3, OUTPUT);
  pinMode(8, INPUT);
  myservo1.attach(6);
  myservo2.attach(9);
  rtc.begin();
  sensors.begin();

  // init communication
  Serial.begin(9600);
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now();
  if (digitalRead(8) == 1 or start == true) { // When the button is pressed the system starts
    if (on == false && start == false) {      // If the heat isn't already turned on, turn it on
      TurnOn();
      SetTemp();
    }
    start = true;
    sensors.requestTemperatures();
    Serial.println(sensors.getTempCByIndex(0));
    if (now.minute() - OnTime >= 2 && sensors.getTempCByIndex(0) - TestTemp < 5 && TeaThingsAreHappening == false) {   //If after 2 minutes have passed an the temperature has not risen, try turning it on again
 
      TurnOn();
      SetTemp();
    }
    if (sensors.getTempCByIndex(0) > 87 or TeaThingsAreHappening == true) {          //If the water has boiled, proceed with the tea. Boiling point set to 87 due to temperture sensor updating slowly.
     TeaThingsAreHappening = true;
      if (on == true) {
        TurnOff();
      }
      WaitForTea();
    }
  }
}