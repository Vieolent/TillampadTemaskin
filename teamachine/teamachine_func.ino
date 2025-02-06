/*
* Name: Teamachine functions
* Author: Miranda Bache
* Date: 2025-02-06
* Description: The document contains all functions for project Teamachine
*/


/*
*This function turns the stove on
*Parameters: Void
*Returns: Void
*/
void TurnOn() {
  myservo2.write(180);
  delay(5000);
  myservo2.write(90);
  on = true;
}

/*
*This function turns the stove off
*Parameters: Void
*Returns: Void
*/
void TurnOff() {
  myservo2.write(180);
  delay(5000);
  myservo2.write(90);
  on = false;
}

/*
*This function reads the temperature and time and updates global variables
*Parameters: Void
*Returns: Void
*/
void SetTemp() {
  DateTime now = rtc.now();
  OnTime = now.minute();
  sensors.requestTemperatures();
  TestTemp = sensors.getTempCByIndex(0);
}

/*
*This function dunks the teabag in and waits for the water to get pumped into the mug before removing the teabag and turning the machine off
*Parameters: Void
*Returns: Void
*/
void WaitForTea() {
  myservo1.write(0);
  SetWaterTime();
  DateTime now = rtc.now();
  digitalWrite(3, HIGH);
  Serial.println("and the current time is:");
  Serial.println(now.minute());
  if (now.minute() - WaterTime == BrewTime) {
    digitalWrite(3, LOW);
    delay(5000);
    myservo1.write(90);
    start = false;
  }
}

/*
*This function remembers what time the pump started pumping the water to the mug  and saves the value as a global variable
*Parameters: Void
*Returns: Void
*/
void SetWaterTime() {
  DateTime now = rtc.now();
  if (WaterTime == -10) {
    WaterTime = now.minute();
    Serial.println("And the WaterTime is:");
    Serial.println(WaterTime);
  }
  if (WaterTime == 59) {
    WaterTime = -1;
  }
  if (WaterTime == 58) {
    WaterTime = -2;
  }
}