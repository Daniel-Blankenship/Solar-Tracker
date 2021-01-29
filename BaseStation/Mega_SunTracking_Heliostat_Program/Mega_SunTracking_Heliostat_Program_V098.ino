//**************************************************************
//  Name: Simple Arduino Sun Tracking / Heliostat Control Program  
//  Original Author: Gabriel Miller      
//  Website: http://www.cerebralmeltdown.com
//
//  Modified by: Daniel Blankenship
//  Website: https://github.com/Daniel-Blankenship/Solar-Tracker
//
//  Notes   : This code was shortened to only include functions
//          : that will be implemented in my own approach to
//          : building a solar tracking system.
//          : The RTC code, and sun position calculation code.
//          : A big thank you to Gabriel Miller!
//****************************************************************

// BigNumber Library Settings
  int calculationScale = 10;
  int calculationSpeed = 0; // 0 for slow, 1 for fast (but less accurate)

// updates global time variables with fresh information from the real time clock
void getRTC()
{
  RTC_Code::getDateDs1307(&secondRTC, &minuteRTC, &hourRTC, &dayOfWeekRTC, &dayOfMonthRTC, &monthRTC, &yearRTC);
  Serial.println(" ");   
  Serial.print("Time: ");
  RTC_Code::printtime(hourRTC, minuteRTC, secondRTC, monthRTC, dayOfMonthRTC, yearRTC, dayOfWeekRTC); // Displays the RTC time
  
  // The variables below can be set to accept input from devices other than an RTC.
  dayOfWeek = dayOfWeekRTC; //NOT CURRENTLY USED
  
  year = yearRTC;
  year = year + 2000;
   
  month = monthRTC;
  day = dayOfMonthRTC;
  hour = hourRTC;
  minute = minuteRTC;
  second = secondRTC;
}

// calculates and updates global altitude and azimuth variables
void getAltitudeAzimuth()
{
  // update time before calculating altitude and azimuth
  getRTC();
  
  findSunsAltAndAzOne(year, month, day, timezone, hour, minute, second, latitude, longitude);
  SunsAltitude = SunsAltitude + (1.02/tan((SunsAltitude + 10.3/(SunsAltitude + 5.11)) * pi/180.0))/60.0; // Refraction Compensation: Meeus Pg. 105
  
  // makes north = 0
  SunsAzimuth = SunsAzimuth + 180;
  if(SunsAzimuth >= 360)
  {
    SunsAzimuth = SunsAzimuth - 360;
  }
}
