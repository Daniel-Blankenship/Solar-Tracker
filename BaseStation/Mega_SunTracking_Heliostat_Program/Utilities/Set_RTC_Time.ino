  #include "Wire.h"  
  #define DS1307_I2C_ADDRESS 0x68
  
  
  
  int SetTime = 1;//Set this variable to 1 to set the time without a button.
  float timezone = -5;//Give your timezone here for double checking against GMT.
  byte Set_Second = 0;
  byte Set_Minute = 10;
  byte Set_Hour = 0;
  byte Set_DayOfMonth = 28;
  byte Set_Month = 1;
  byte Set_Year = 21;
  byte Set_DayOfWeek = 4;

void setup(){
  Wire.begin();
  Serial.begin(9600);
  if (SetTime==1){
  setDateDs1307(Set_Second, Set_Minute, Set_Hour, Set_DayOfWeek, Set_DayOfMonth, Set_Month, Set_Year);
  }
}

void loop(){
 if (SetTime == 0){
  setDateDs1307(Set_Second, Set_Minute, Set_Hour, Set_DayOfWeek, Set_DayOfMonth, Set_Month, Set_Year);
  Serial.println("Time Has Been Set");
  delay(3000);
 }
  byte secondRTC, minuteRTC, hourRTC, dayOfWeekRTC, dayOfMonthRTC, monthRTC, yearRTC;
  getDateDs1307(&secondRTC, &minuteRTC, &hourRTC, &dayOfWeekRTC, &dayOfMonthRTC, &monthRTC, &yearRTC);
  Serial.print("Local Time: ");
  printtime(hourRTC, minuteRTC, secondRTC, monthRTC, dayOfMonthRTC, yearRTC, dayOfWeekRTC, 0);//Displays the local time
  Serial.print("GMT:        ");
  printtime(hourRTC, minuteRTC, secondRTC, monthRTC, dayOfMonthRTC, yearRTC, dayOfWeekRTC, 1);//Displays the time at the prime meridian.
  Serial.println("  ");
  delayInMillis(1000);
}

void delayInMillis(long delayInMicrosec){
     long t1,t2;
     t1=millis(); 
     t2=millis();
     while ((t1+delayInMicrosec)>t2){t2=millis();
     if (t2<t1){t2=t1+delayInMicrosec+1;}//Check if micros() rolled over
     }
}



//
// Maurice Ribble
// 4-17-2008
// http://www.glacialwanderer.com/hobbyrobotics

// This code tests the DS1307 Real Time clock on the Arduino board.
// The ds1307 works in binary coded decimal or BCD.  You can look up
// bcd in google if you aren't familior with it.  There can output
// a square wave, but I don't expose that in this code.  See the
// ds1307 for it's full capabilities.




// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

// Stops the DS1307, but it has the side effect of setting seconds to 0
// Probably only want to use this for testing
/*void stopDs1307()
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(0x80);
  Wire.endTransmission();
}*/

// 1) Sets the date and time on the ds1307
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Assumes you're passing in valid numbers
void setDateDs1307(byte second,        // 0-59
                   byte minute,        // 0-59
                   byte hour,          // 1-23
                   byte dayOfWeek,     // 1-7
                   byte dayOfMonth,    // 1-28/29/30/31
                   byte month,         // 1-12
                   byte year)          // 0-99
{
   Wire.beginTransmission(DS1307_I2C_ADDRESS);
   Wire.write((byte)0);
   Wire.write((byte)decToBcd(second));    // 0 to bit 7 starts the clock
   Wire.write((byte)decToBcd(minute));
   Wire.write((byte)decToBcd(hour));      // If you want 12 hour am/pm you need to set
                                   // bit 6 (also need to change readDateDs1307)
   Wire.write((byte)decToBcd(dayOfWeek));
   Wire.write((byte)decToBcd(dayOfMonth));
   Wire.write((byte)decToBcd(month));
   Wire.write((byte)decToBcd(year));
   Wire.endTransmission();
}

// Gets the date and time from the ds1307
void getDateDs1307(byte *second,
          byte *minute,
          byte *hour,
          byte *dayOfWeek,
          byte *dayOfMonth,
          byte *month,
          byte *year)
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write((byte)0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  *second     = bcdToDec(Wire.read() & 0x7f);
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  *dayOfWeek  = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month      = bcdToDec(Wire.read());
  *year       = bcdToDec(Wire.read());
}

  void printtime(int hour, int minute, int second, int month, int dayOfMonth, int year, int dayOfWeek, int GMT_YorN){
  if (GMT_YorN==1){hour = hour - timezone;//Note: Timezone is a global variable
  if (hour<0){hour = hour + 24;}
  if (hour>=24){hour=hour-24;}
  }
  
  Serial.print(hour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(second, DEC);
  Serial.print("  ");

  if (GMT_YorN!=1){
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print("  Day_of_week:");
  Serial.println(dayOfWeek, DEC);
  }
}

