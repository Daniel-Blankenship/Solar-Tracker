//**************************************************************
//  Name    : Simple Arduino Sun Tracking / Heliostat Control Program  
//  Author  : Gabriel Miller      
//  Website : http://www.cerebralmeltdown.com                                   
//  Notes   : Code for controlling various types of solar       
//          : machines.                                         
//****************************************************************
  #include "Wire.h"  
  #include "RTC_Code.h"
  #include <BigNumber.h>
  #include <BigNumberMath.h>
  

//PUT YOUR LATITUDE, LONGITUDE, AND TIME ZONE HERE
  float latitude = 35.96;
  float longitude = -83.92;
  float timezone = -5;
  
//If you live in the northern hemisphere, put 0 here. If you live in the southern hemisphere put 1.
  int useNorthAsZero = 1;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//END OF USER SETTINGS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    
    
    
    
//BigNumber Library Settings
  int calculationScale=10;
  int calculationSpeed=0;// 0 for slow, 1 for fast (but less accurate)
//
    
    
    


/////////////////////////////////////////////////////////// 
//MISC. VARIABLES USED THROUGHOUT THE PROGRAM
///////////////////////////////////////////////////////////  
  float pi = 3.14159265, SunsAltitude, SunsAzimuth, h, delta;
  //float helioaltdif, helioazdif, prehelioalt, prehelioaz, helioalt, helioaz;  
  //float preAlt = 0, preAz = 0, altdif, azdif;
  int iterationsAfterReset, preTargetsUsed, machineNumber, windToggleCount, midCycle, FirstIterationAfterArduinoReset, justFinishedManualControl;
  int  targetsUsed = 1;
  unsigned long updateTime = 0,  now = 0; 
  float altManualSpeedSwap, azManualSpeedSwap, altMove, azMove, UDCenter, LRCenter;
  int joystickModeOnOff, joystickTriggerOnce, manualMachineNumber;

/////////////////////////////////////////////////////////// 
//END MISC. VARIABLES
///////////////////////////////////////////////////////////
  
  
///////////////////////////////////////////////////////////  
//MISC. ARRAYS  
///////////////////////////////////////////////////////////  

///////////////////////////////////////////////////////////  
//END MISC. ARRAYS  
///////////////////////////////////////////////////////////  
  
  
  
  
  
  
  
  
  
/////////////////////////////////////////////////////////// 
// VOID SETUP
/////////////////////////////////////////////////////////// 
  void setup(){

  Wire.begin();
  Serial.begin(9600);
  
  
  ////////////////////////////////              
  //TWO WIRE STEP/DIR DRIVER BOARD CODE  

  ////////////////////////////////  

//Arduino pins automatically default to inputs, so this code has thus been commented out.
//  pinMode(azLimitPin, INPUT);
//  pinMode(altLimitPin, INPUT);
//  pinMode(WindProtectionSwitch, INPUT);
//  pinMode(manualModeOnOffPin, INPUT);
//  pinMode(HeliostatToSun, INPUT);
//  pinMode(plusOneButton, INPUT);
//  pinMode(minusOneButton, INPUT);



}
/////////////////////////////////////////////////////////// 
//END VOID SETUP
/////////////////////////////////////////////////////////// 






/////////////////////////////////////////////////////////// 
// VOID LOOP
/////////////////////////////////////////////////////////// 
void loop()
{  
  
  int month, year, day, dayOfWeek;
  float hour, minute, second;
  byte secondRTC, minuteRTC, hourRTC, dayOfWeekRTC, dayOfMonthRTC, monthRTC, yearRTC;
   
  RTC_Code::getDateDs1307(&secondRTC, &minuteRTC, &hourRTC, &dayOfWeekRTC, &dayOfMonthRTC, &monthRTC, &yearRTC);
  if (joystickModeOnOff!=1){
  Serial.println(" ");   
  Serial.print("Time: ");
  RTC_Code::printtime(hourRTC, minuteRTC, secondRTC, monthRTC, dayOfMonthRTC, yearRTC, dayOfWeekRTC);//Displays the RTC time
  delay(500);
  }
  
  //The variables below can be set to accept input from devices other than an RTC.
  dayOfWeek=dayOfWeekRTC;//NOT CURRENTLY USED
  year = yearRTC;//NOT CURRENTLY USED
  month = monthRTC;
  day = dayOfMonthRTC;
  hour = hourRTC;
  minute = minuteRTC;
  second = secondRTC; 
  /////////////////////////////////////////////////////////////////////////////////
  

  
  //if ((digitalRead(manualModeOnOffPin)!=HIGH) && (joystickModeOnOff!=1)){     
 // now = millis();
 // if ( (now + updateEvery*1000) < updateTime){updateTime=now+updateEvery;}
 // }//END if ((digitalRead(manualModeOnOffPin)!=HIGH))
  
  
  
  
  //USE THIS CODE TO RUN THE PROGRAM AT FAST SPEED. DON"T FORGET TO UNCOMMENT THE } } TOWARD THE BOTTOM
  //YOU MAY ALSO WISH TO COMMENT OUT THE "MachineOn" AND "MachineOff" CODE TO GET RID OF THE DELAY
  //  month = 6;
  //  day = 9;
  //  for (float hour=8; hour <= 21; hour++){
  ////  Serial.print("Hour ");
  ////  Serial.println(hour);
  ////  Serial.println(TotalAltSteps);
  ////  Serial.println(TotalAzSteps);
  //    for (float minute=0; minute <= 59; minute++){
  //    secondCounter=updateEvery+1;      
  ////  Serial.print("Minute ");
  ////  Serial.println(minute);



  //if (digitalRead(WindProtectionSwitch)!=HIGH && (FirstIterationAfterArduinoReset!=0)){  
  //Updates the position of the sun, which in turn tells the machine(s) that it is time to move.

      //SunPositionAlgo_LowAc::CalculateSunsPositionLowAc(month, day, hour, minute, second, timezone, latitude, longitude, SunsAltitude, SunsAzimuth, delta, h);
      year = year + 2000;
      findSunsAltAndAzOne(year, month, day, timezone, hour, minute, second, latitude, longitude);
      SunsAltitude = SunsAltitude + (1.02/tan((SunsAltitude + 10.3/(SunsAltitude + 5.11)) * pi/180.0))/60.0;//Refraction Compensation: Meeus Pg. 105

      //if(useNorthAsZero==1){
      //if (SunsAzimuth<0){
      //SunsAzimuth=(SunsAzimuth+180)*-1;
      //}
      //if (SunsAzimuth>0){
      //SunsAzimuth=(SunsAzimuth-180)*-1;
      //}
      
      //Serial.print("Sun's Azimuth Modified for Southern Hemisphere: ");
      //Serial.println(SunsAzimuth);
      //}
      
      SunsAzimuth = SunsAzimuth + 180;
      
      if(SunsAzimuth > 360)
      {
        SunsAzimuth = SunsAzimuth - 360;
      }
      
      if ((joystickModeOnOff!=1)){
           Serial.print("Sun's Alt: ");
           Serial.println(SunsAltitude,3);
           delay(50);
           Serial.print("Sun's Az: ");
           Serial.println(SunsAzimuth , 3);
           delay(50);
      }

  delay(5000);
  //END Update Every X seconds


//}//END do not reset after arduino resets when Wind Protection Switch is high
//  //}}//End for loops that run the program at fast speed

}//End Void Loop
/////////////////////////////////////////////////////////// 
// END VOID LOOP
/////////////////////////////////////////////////////////// 
