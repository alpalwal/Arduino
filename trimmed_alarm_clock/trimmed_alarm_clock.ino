/*
:Project:Allarm_Clock

=== BOM
Add the bill of the materials you need for this project.

|===
| ID | Part name      | Part number | Quantity
| R1 | 10k Resistor   | 1234-abcd   | 10       
| L1 | Red LED        | 2345-asdf   | 5        
| A1 | Arduino Zero   | ABX00066    | 1        
|===


=== Help
This document is written in the _AsciiDoc_ format, a markup language to describe documents. 
If you need help you can search the http://www.methods.co.nz/asciidoc[AsciiDoc homepage]
or consult the http://powerman.name/doc/asciidoc[AsciiDoc cheatsheet]

*/
//************libraries**************//
#include <Wire.h>
#include <RTClib.h>
// #include <LiquidCrystal_I2C.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

//************************************//
// LiquidCrystal_I2C lcd(0x27,20,4); // Display  I2C 20 x 4
RTC_DS1307 RTC;

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define WHITE 0x7

//************Button*****************//
// int P1=6; // Button SET MENU' = select
// int P2=7; // Button + = right
// int P3=8; // Button - = left
// int P4=9; // SWITCH Alarm = down

//**************Alarm***************//
#define LED 13
#define buzzer 10

//************Variables**************//
int hourupg;
int minupg;
int yearupg;
int monthupg;
int dayupg;
int menu =0;
int setAll =0;

uint8_t alarmHours = 0, alarmMinutes = 0;  // Holds the current alarm time
uint8_t buttons = lcd.readButtons();

void setup()
{
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(LED,OUTPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer as an output
  printAllOff();
  Serial.begin(9600);
  lcd.setBacklight(WHITE);
  Wire.begin();
  RTC.begin();

  // if (! RTC.isrunning()) {
  //   Serial.println("RTC is NOT running!");
  //   // Set the date and time at compile time
  //   RTC.adjust(DateTime(__DATE__, __TIME__));
  // }
  // // RTC.adjust(DateTime(__DATE__, __TIME__)); //removing "//" to adjust the time
  //   // The default display shows the date and time
  int menu=0;
}
 
void loop()
{ 
// check if you press the SET button and increase the menu index
  if(buttons & BUTTON_SELECT) 
  {
   menu=menu+1;
  }
  if((buttons & BUTTON_RIGHT)&&(buttons & BUTTON_LEFT))                                                                                                                                                                     
  {
    
    DisplaySetHourAll();
    DisplaySetMinuteAll();
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("ALARM");
    lcd.setCursor(5,1);
    lcd.print(alarmHours, DEC);
    lcd.print(":");
    lcd.print(alarmMinutes, DEC);
    delay(1000);
    lcd.clear();
  }
// in which subroutine should we go?
  if (menu==0)
    {
     DisplayDateTime(); // void DisplayDateTime
     Alarm(); // Alarm control
          }
  if (menu==1)
    {
    StoreAgg(); 
    delay(500);
    menu=0;
    }
    delay(100);
}

void DisplayDateTime ()
{
// We show the current date and time
  DateTime now = RTC.now();

  lcd.setCursor(0, 2);
  lcd.print("Hour : ");
  
  if (now.hour()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.hour(), DEC);
  hourupg=now.hour();
  lcd.print(":");
  if (now.minute()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.minute(), DEC);
  minupg=now.minute();
  lcd.print(":");
  if (now.second()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.second(), DEC);
  
  lcd.setCursor(0, 1);
  lcd.print("Date : ");
  if (now.day()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.day(), DEC);
  dayupg=now.day();
  lcd.print("/");
  if (now.month()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.month(), DEC);
  monthupg=now.month();
  lcd.print("/");
  lcd.print(now.year(), DEC);
  yearupg=now.year();
 
  char DOW[][10]={"Sunday   ","Monday   ","Tuesday  ","Wednesday","Thursday ","Friday   ","Saturday "};
  lcd.setCursor(0, 0); 
  lcd.print("Day  : ");
  lcd.print(DOW[now.dayOfTheWeek()]); // if it appears error in the code, enter the code given below
  //lcd.print(DOW[now.dayOfWeek()]);
}
 
void StoreAgg()
{
// Variable saving
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SAVING IN");
  lcd.setCursor(0,1);
  lcd.print("PROGRESS");
  RTC.adjust(DateTime(yearupg,monthupg,dayupg,hourupg,minupg,0));
  delay(200);
}
void DisplaySetHourAll()// Setting the alarm minutes
{
  while(buttons & BUTTON_SELECT){

  lcd.clear();

  if(buttons & BUTTON_RIGHT)
  {
    if(alarmHours==23)
    {
      alarmHours=0;
    }
    else
    {
      alarmHours=alarmHours+1;
    }
  }
   if(buttons & BUTTON_LEFT)
  {
    if(alarmHours==0)
    {
      alarmHours=23;
    }
    else
    {
      alarmHours=alarmHours-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set HOUR Alarm:");
  lcd.setCursor(0,1);
  lcd.print(alarmHours,DEC);
  delay(200);
 }
 delay(200);
}

void DisplaySetMinuteAll()// Setting the alarm minutes
 {
  while(buttons & BUTTON_SELECT){ 

  lcd.clear();
  if(buttons & BUTTON_RIGHT)
  {
    if (alarmMinutes==59)
    {
      alarmMinutes=0;
    }
    else
    {
      alarmMinutes=alarmMinutes+1;
    }
  }
   if(buttons & BUTTON_LEFT)
  {
    if (alarmMinutes==0)
    {
      alarmMinutes=59;
    }
    else
    {
      alarmMinutes=alarmMinutes-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set MIN. Alarm:");
  lcd.setCursor(0,1);
  lcd.print(alarmMinutes,DEC);
  delay(200);
 }
 delay(200);
}
void printAllOn(){
   lcd.setCursor(0,3);
  lcd.print("Alarm: ");

  
  
  if (alarmHours <= 9)
  {
    lcd.print("0");
  }
  lcd.print(alarmHours, DEC);
  
  lcd.print(":");
  if (alarmMinutes <= 9)
  {
    lcd.print("0");
  }
  lcd.print(alarmMinutes, DEC); 

}
void printAllOff() {
  lcd.setCursor(0, 3);
  lcd.print("Alarm: Off  ");  
}
void Alarm(){
  
  if(buttons & BUTTON_DOWN)
    {
    setAll=setAll+1;
    }
  if (setAll==0)
    {
     printAllOff();
     noTone (buzzer);
     digitalWrite(LED,LOW);
     }
  if (setAll==1)
    {
     printAllOn();    
     DateTime now = RTC.now();
     if ( now.hour() == alarmHours && now.minute() == alarmMinutes )
        {
         DateTime now = RTC.now();
         digitalWrite(LED,HIGH);
         tone(buzzer,880); //play the note "A5" (LA5)
         delay (300);
         tone(buzzer,698); //play the note "F6" (FA5)
        }
    else{
         noTone (buzzer);
         digitalWrite(LED,LOW);
        }
    
    } 
  if (setAll==2)
    {
     setAll=0;
    }
    delay(200);
}