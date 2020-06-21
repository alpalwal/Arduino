
HOME
STORE
SOFTWARE
EDU
RESOURCES
COMMUNITY
HELP
SEARCH
 Show Cart
SIGN IN
Arduino Forum >  Using Arduino >  Programming Questions >  [SOLVED] How to set time on DS1307 using Time library?
PRINT Go DownPages: [1]
Topic: [SOLVED] How to set time on DS1307 using Time library? (Read 81453 times) Previous Topic - Next Topic
bratan
avatar_bratan
*****
God Member
Posts: 626
Karma: 77  [add]
xronosclock.com
http://www.lensdigital.com

[SOLVED] How to set time on DS1307 using Time library?
Oct 25, 2012, 03:12 am Last Edit: Oct 29, 2012, 09:53 pm by bratan Reason: 1
I'm trying to set time on DS1307 chip via Time library, but can't figure out how to do it. I successfully changed Arduino's time with setTime(hr,min,sec,day,month,yr) but it doesn't propagate to RTC chip :(
Xronos Clock - A talking arduino based alarm clock is now available. Check out xronosclock.com for pictures, source code, schematics, etc.
nickgammon
avatar_nickgammon
*****
Global Moderator
Nick Gammon
Brattain Member
Posts: 28,096
Karma: 1997  [add]
Lua rocks!
http://www.gammon.com.au

Re: How to set time on DS1307 using Time library?
#1
Oct 25, 2012, 03:55 am 
Read this before posting a programming question

Your code?
Please post technical questions on the forum, not by personal message. Thanks!

More info: http://www.gammon.com.au/electronics
JChristensen
avatar_JChristensen
**
Faraday Member
Posts: 4,206
Karma: 259  [add]
CODE is a mass noun and should not be used in the plural or with an indefinite article.
http://adventuresinarduinoland.blogspot.com/

Re: How to set time on DS1307 using Time library?
#2
Oct 25, 2012, 04:41 am 
There is a second library that comes with the Time library, called DS1307RTC. Once the system time is set, all that is needed is a call to RTC.set():

Code: [Select]
#include <Time.h>
#include <DS1307RTC.h>
#include <Wire.h>
...
setTime(hr,min,sec,day,month,yr);
RTC.set(now());
arghduino
Guest

Re: How to set time on DS1307 using Time library?
#3
Oct 25, 2012, 10:53 am 
from adafruit:

Code: [Select]
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

void setup () {
    Serial.begin(57600);
    Wire.begin();
    RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop () {
    DateTime now = RTC.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    
    Serial.println();
    delay(3000);
}
bratan
avatar_bratan
*****
God Member
Posts: 626
Karma: 77  [add]
xronosclock.com
http://www.lensdigital.com

Re: How to set time on DS1307 using Time library?
#4
Oct 25, 2012, 04:59 pm 
Thanks guys! 
It's basically example code "RTC" from Time library examples.
RTC.set(now()); sounds like what I'm looking for, thanks Jack!

arghduino, I've tried Ada's example before, it doesn't set my RTC's chip time for some reason. I posted on adafruit forum, and they say it's because I'm not using their kit, which is true, but I don't see how it matters since I'm using same DS1307 chip.
Xronos Clock - A talking arduino based alarm clock is now available. Check out xronosclock.com for pictures, source code, schematics, etc.
JChristensen
avatar_JChristensen
**
Faraday Member
Posts: 4,206
Karma: 259  [add]
CODE is a mass noun and should not be used in the plural or with an indefinite article.
http://adventuresinarduinoland.blogspot.com/

Re: How to set time on DS1307 using Time library?
#5
Oct 25, 2012, 06:00 pm 
Hmmmm, that's odd, I wouldn't think you'd have to use their RTC kit either. I have several of their RTCs and they work fine with the Time.h and DS1307RTC.h libraries. I think I may have tried Ada's library in the past, and I don't remember any problems, but for whatever reason I just stick with Time.h and DS1307RTC.h. Time.h has a lot of functionality which I like. DS1307RTC.h also will work with a DS3231 (Chronodot).
bratan
avatar_bratan
*****
God Member
Posts: 626
Karma: 77  [add]
xronosclock.com
http://www.lensdigital.com

Re: How to set time on DS1307 using Time library?
#6
Oct 27, 2012, 03:27 pm 
Thanks again Jack! 
This totally worked!
Code: [Select]
setTime(hr,min,sec,day,month,yr);
RTC.set(now());


Yeah I'm at loss why Adafruit's library is not setting time on my chip. BTW I found this great and super simple code online that sets time just fine without any library:
Code: [Select]
//Arduino 1.0+ Only
//Arduino 1.0+ Only

#include "Wire.h"
#define DS1307_ADDRESS 0x68
byte zero = 0x00; //workaround for issue #527

void setup(){
  Wire.begin();
  Serial.begin(9600);
  setDateTime(); //MUST CONFIGURE IN FUNCTION
}

void loop(){
  printDate();
  delay(1000);
}

void setDateTime(){

  byte second =      00; //0-59
  byte minute =      19; //0-59
  byte hour =        21; //0-23
  byte weekDay =     4; //1-7
  byte monthDay =    27; //1-31
  byte month =       10; //1-12
  byte year  =       12; //0-99

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); //stop Oscillator

  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));

  Wire.write(zero); //start 

  Wire.endTransmission();

}

byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

void printDate(){

  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());

  //print the date EG   3/1/11 23:59:59
  Serial.print(month);
  Serial.print("/");
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(year);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

}
Xronos Clock - A talking arduino based alarm clock is now available. Check out xronosclock.com for pictures, source code, schematics, etc.
JChristensen
avatar_JChristensen
**
Faraday Member
Posts: 4,206
Karma: 259  [add]
CODE is a mass noun and should not be used in the plural or with an indefinite article.
http://adventuresinarduinoland.blogspot.com/

Re: How to set time on DS1307 using Time library?
#7
Oct 27, 2012, 05:03 pm 
Quote from: bratan on Oct 27, 2012, 03:27 pm

BTW I found this great and super simple code online that sets time just fine without any library:


While libraries are great because they simplify the mainline code, it's very much worthwhile to understand some of what's under the covers, in this case, using I2C to interface to the RTC chip directly. So if there is a problem, or a feature of the chip that the library doesn't implement, you can do it yourself. In the case of the DS1307RTC library, for example, it doesn't implement any methods to interface to the DS1307's static RAM.

Also note the layers of libraries here. DS1307RTC depends on both the Time library and the Wire library. The code from bildr only requires the Wire library. It could be taken one step further, and the ATmega328's registers could be manipulated directly to do the I2C communication, and then the Wire library could be eliminated.

Glad you got it working!

larryd
avatar_LarryD
*****
Brattain Member
Posts: 16,427
Karma: 1416  [add]
The last thing you did is where you should start looking.

Re: [SOLVED] How to set time on DS1307 using Time library?
#8
Nov 19, 2012, 09:02 pm 
Warning:    Your "code online" link causes:
2012-11-19 12:44:12,High,An intrusion attempt by 129.121.36.98 was blocked.,Blocked,No Action Required,Web Attack: Malicious Toolkit Website 9,No Action Required,No Action Required,"
With Norton antivrus from my computer.
No technical PMs.
The last thing you did is where you should start looking.
franlol
avatar_franlol
*
Newbie
Posts: 15
Karma: 8  [add]
Arduino r00lz

Re: [SOLVED] How to set time on DS1307 using Time library?
#9
Mar 06, 2014, 11:21 pm 
thanks for your replys :P
Mubashirpmc
avatar_Mubashirpmc
*
Newbie
Posts: 1
Karma: 0  [add]

Re: [SOLVED] How to set time on DS1307 using Time library?
#10
Feb 28, 2018, 06:44 pm 
Anybody have automatic school bell code
hanafiahgunawan99
avatar_hanafiahgunawan99
*
Newbie
Posts: 1
Karma: 0  [add]

Re: [SOLVED] How to set time on DS1307 using Time library?
#11
May 10, 2018, 05:26 pm 
look my video... https://www.youtube.com/watch?v=gQOWvMCIF2E
and this my code

#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(0,52,19,3,8,5,18);
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); 
  Wire.write(decToBcd(second)); 
  Wire.write(decToBcd(minute)); 
  Wire.write(decToBcd(hour)); 
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}
void loop()
{
  displayTime();
  delay(1000);
}
