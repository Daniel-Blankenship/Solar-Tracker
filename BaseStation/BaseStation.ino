// Author: Daniel Blankenship
// Website: https://github.com/Daniel-Blankenship/Solar-Tracker

// ------------------------------------------------------------------

#include "Wire.h"
#include "RTC_Code.h"
#include <BigNumber.h>
#include <BigNumberMath.h>

// Settings
// ------------------------------------------------------------------

// +/- : N/S E/W
float latitude = 35.96;
float longitude = -83.92;

// GMT offset
float timezone = -5;

// ------------------------------------------------------------------
// End of settings

int month, year, day, dayOfWeek;
float hour, minute, second;
byte secondRTC, minuteRTC, hourRTC, dayOfWeekRTC, dayOfMonthRTC, monthRTC, yearRTC;

float SunsAltitude;
float SunsAzimuth;
float pi = 3.14159265;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

// End of setup
// ------------------------------------------------------------------

void loop()
{
  getAltitudeAzimuth();
     
  Serial.print("Sun's Alt: ");
  Serial.println(SunsAltitude,3);
  delay(50);
  Serial.print("Sun's Az: ");
  Serial.println(SunsAzimuth , 3);
  delay(50);
   
  delay(5000);
}
