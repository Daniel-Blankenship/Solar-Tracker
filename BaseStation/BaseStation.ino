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

// panel tilt
// when both axes are at 90 the panel is level
float xAngle; // east or west, 0 = east, 180 = west
float yAngle; // north or south, 0 = north, 180 = south

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

  convertAltAz();


   
  delay(5000);
}

// updates the panel tilt values using the altitude and azimuth
void convertAltAz()
{
  float tilt = SunsAltitude;
  float tiltScale;
  float tiltDegreesXY;

  float xRatio;
  float yRatio;


  if(SunsAzimuth <= 45) // NE
  {
    tiltScale = (100/45) * SunsAzimuth;
  }
  else if(SunsAzimuth <= 90) // NE
  {
    tiltScale = (100/45) * (45 - (SunsAzimuth - 45));
  }
  else if(SunsAzimuth <= 135) // SE
  {
    tiltScale = (100/45) * (SunsAzimuth - 90);
  }
  else if(SunsAzimuth <= 180) // SE
  {
    tiltScale = (100/45) * (45 - (SunsAzimuth - 135));
  }
  else if(SunsAzimuth <= 225) // SW
  {
    tiltScale = (100/45) * (SunsAzimuth - 180);
  }
  else if(SunsAzimuth <= 270) // SW
  {
    tiltScale = (100/45) * (45 - (SunsAzimuth - 225));
  }
  else if(SunsAzimuth <= 315) // NW
  {
    tiltScale = (100/45) * (SunsAzimuth - 270);
  }
  else if(SunsAzimuth < 360) // NW
  {
    tiltScale = (100/45) * (45 - (SunsAzimuth - 315));
  }

  tilt = 1 + (1 * (tiltScale / 100));
  tiltDegreesXY = SunsAltitude * tilt;

  if(SunsAzimuth <= 90) // NE
  {
    xRatio = (90 - SunsAzimuth) / 90;
    yRatio = SunsAzimuth / 90;

    xAngle = xRatio * tiltDegreesXY;
    yAngle = yRatio * tiltDegreesXY;
  }
  else if(SunsAzimuth <= 180) // SE
  {
    xRatio = (90 - (SunsAzimuth - 90)) / 90;
    yRatio = (SunsAzimuth - 90) / 90;

    xAngle = 180 - (xRatio * tiltDegreesXY);
    yAngle = yRatio * tiltDegreesXY;
  }
  else if(SunsAzimuth <= 270) // SW
  {
    xRatio = (90 - (SunsAzimuth - 180)) / 90;
    yRatio = (SunsAzimuth - 180) / 90;

    xAngle = 180 - (xRatio * tiltDegreesXY);
    yAngle = 180 - (yRatio * tiltDegreesXY);
  }
  else if(SunsAzimuth < 360) // NW
  {
    xRatio = (90 - (SunsAzimuth - 270)) / 90;
    yRatio = (SunsAzimuth - 270) / 90;

    xAngle = xRatio * tiltDegreesXY;
    yAngle = 180 - (yRatio * tiltDegreesXY);
  }



  Serial.print("tiltScale: ");
  Serial.println(tiltScale,3);
  delay(50);
  Serial.print("tilt: ");
  Serial.println(tilt,3);
  delay(50);
  Serial.print("tiltDegreesXY: ");
  Serial.println(tiltDegreesXY,3);
  delay(50);
  Serial.print("xRatio: ");
  Serial.println(xRatio,3);
  delay(50);
  Serial.print("yRatio: ");
  Serial.println(yRatio,3);
  delay(50);
  Serial.print("xAngle: ");
  Serial.println(xAngle,3);
  delay(50);
  Serial.print("yAngle: ");
  Serial.println(yAngle,3);
  delay(50);
}
