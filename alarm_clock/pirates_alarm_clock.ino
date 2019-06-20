/*
right/left -  - goes to set minute/hour functions for alarm and then clears screen
right = +
left = -
Select to move forward

down - turns on alarm
*/

//************libraries**************//
#include <RTClib.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// Initialize rtc
RTC_PCF8523 rtc;

// Set the backlight color
#define WHITE 0x7

//**************Alarm***************//
#define LED 13
#define buzzer 10

//************Variables**************//
int hourupg;
int minupg;
int setAll = 0;

uint8_t alarmHours = 5, alarmMinutes = 50;  // Holds the current alarm time

void setup() {
  Serial.begin(9600); // Debugging output
  lcd.begin(16, 2);   // set up the LCD's number of columns and rows: 
  pinMode(LED,OUTPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer as an output
  Wire.begin();
  rtc.begin();

  alarmOffPrint(); // Start with the alarm turned off

  // If the clock isn't set right - try to re-set it to the current time
  if (! rtc.begin()) {
    Serial.println("Couldn't find rtc");
    while (1);
  }

  if ( ! rtc.initialized()) {
    Serial.println("rtc is NOT running!");
    // following line sets the rtc to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

uint8_t i=0; //// DO I NEED THIS
void loop() {
    uint8_t buttons = lcd.readButtons(); 
    if ((buttons & BUTTON_LEFT)&&(buttons & BUTTON_RIGHT)) {
        DisplaySetHourAll();
        DisplaySetMinuteAll();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alarm Set           ");
        delay(1000);
        lcd.clear();
    }

    DisplayDateTime(); // void DisplayDateTime
    Alarm(); // Alarm control
}

// Top row display - current time
void DisplayDateTime () {
    // We show the current time on row 1
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);

    hourupg=now.hour();
    lcd.print("Hour : ");

    if (now.hour()>12) {
        hourupg = hourupg - 12;
        lcd.print(hourupg);
    } else {
        lcd.print(now.hour(), DEC);
    }

    lcd.print(":");
    if (now.minute()<=9) {
        lcd.print("0");
    }
    lcd.print(now.minute(), DEC);
    minupg=now.minute();

    if (now.hour()>12) {
        lcd.print(" PM  ");
    } else {
        lcd.print(" AM  ");
    }
}

// alarm on - what to show
void alarmOnPrint() {
    lcd.setCursor(0,1);
    lcd.print("Alarm: ");
    int printAlarm ;

    if (alarmHours>12) {
        printAlarm = alarmHours - 12;
        lcd.print(printAlarm);
    } else {
        lcd.print(alarmHours, DEC);
    }
    
    lcd.print(":");
    if (alarmMinutes<=9) {
        lcd.print("0");
    }    
    lcd.print(alarmMinutes, DEC);

    if (alarmHours>12) {
        lcd.print(" PM  ");
    } else {
        lcd.print(" AM  ");
    }
}

// alarm off - what to show
void alarmOffPrint() {
    lcd.setCursor(0, 1); 
    lcd.print("Alarm:  OFF     ");
}

// Main alarm work gets done here
void Alarm() {
    uint8_t buttons = lcd.readButtons();

    if(buttons & BUTTON_DOWN) {
        setAll=setAll+1;
    }
    if (setAll==0) {     
        alarmOffPrint();
        noTone (buzzer);
        digitalWrite(LED,LOW);
    }
    if (setAll==1) {
        alarmOnPrint();    
        DateTime now = rtc.now();
        // bool breakAlarm = false ;
        bool triggerAlarm = true ;

        // If it's alarm time - trigger it once and sleep for a minute so the alarm will have passed
        if ( now.hour() == alarmHours && now.minute() == alarmMinutes ) {
            triggerAlarm = true;

            while(triggerAlarm){
                digitalWrite(LED,HIGH);
                triggerAlarm = playPirates();
                DisplayDateTime();
                Serial.print("LOOP\n");
                Serial.print(triggerAlarm);
                Serial.print("\n");
                if (!triggerAlarm) {
                    Serial.print("broken\n");
                    digitalWrite(LED,LOW);
                    break;
                }    
                delay(200);
            }
            delay(60000);
        }
        else {
            noTone (buzzer);
            digitalWrite(LED,LOW);
        }

    } 
    if (setAll==2) {
        setAll=0;
    }
    delay(200);
}


// Setting the alarm hour
void DisplaySetHourAll() {
    bool setHour = true ;
    while(setHour = true){
        uint8_t buttons = lcd.readButtons();

        lcd.setCursor(0,0);
        lcd.print("Set HOUR Alarm:");
        lcd.setCursor(0,1);
        
        if (alarmHours>12) {
            int printAlarm = alarmHours - 12;
            lcd.print(printAlarm);
            lcd.print(" PM");
        } else {
            lcd.print(alarmHours, DEC);
            lcd.print(" AM");
        }
        lcd.print("                 ");
        delay(200);

        if(buttons & BUTTON_RIGHT) {
            if(alarmHours==23) {
                alarmHours=0;
            }
            else {
                alarmHours=alarmHours+1;
            }
        }   
        if(buttons & BUTTON_LEFT) {
            if(alarmHours==0) {
                alarmHours=23;
            }
            else {
                alarmHours=alarmHours-1;
            }
        }
        if(buttons & BUTTON_SELECT) {
            break;
        }
    }
    delay(200);
}

// Setting the alarm minutes
void DisplaySetMinuteAll() {
    bool setMin = true ;
    while(setMin = true){
        uint8_t buttons = lcd.readButtons();

        lcd.setCursor(0,0);
        lcd.print("Set MINUTE Alarm:");
        lcd.setCursor(0,1);
        lcd.print(alarmMinutes,DEC);
        lcd.print("           ");
        delay(200);

        if(buttons & BUTTON_RIGHT) {
            if(alarmMinutes==23) {
                alarmMinutes=0;
            }
            else {
                alarmMinutes=alarmMinutes+1;
            }
        }   
        if(buttons & BUTTON_LEFT) {
            if(alarmMinutes==0) {
                alarmMinutes=23;
            }
            else {
                alarmMinutes=alarmMinutes-1;
            }
        }
        if(buttons & BUTTON_SELECT) {
            break;
        }
    }
    delay(200);    
}

float playPirates () {    
    const int songspeed = 1.75; //Change to 2 for a slower version of the song, the bigger the number the slower the song
    
    //Defining note frequency
    #define NOTE_C4  262   
    #define NOTE_D4  294
    #define NOTE_E4  330
    #define NOTE_F4  349
    #define NOTE_G4  392
    #define NOTE_A4  440
    #define NOTE_B4  494
    #define NOTE_C5  523
    #define NOTE_D5  587
    #define NOTE_E5  659
    #define NOTE_F5  698
    #define NOTE_G5  784
    #define NOTE_A5  880
    #define NOTE_B5  988

    //Note of the song, 0 is a rest/pulse    
    int notes[] = {       
        NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
        NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
        NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
        NOTE_A4, NOTE_G4, NOTE_A4, 0,
        
        NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
        NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
        NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
        NOTE_A4, NOTE_G4, NOTE_A4, 0,
        
        NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
        NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0, 
        NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
        NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,
        
        NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
        NOTE_D5, NOTE_E5, NOTE_A4, 0, 
        NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
        NOTE_C5, NOTE_A4, NOTE_B4, 0
    };

    //duration of each note (in ms) Quarter Note is set to 250 ms
    int duration[] = {         
        125, 125, 250, 125, 125, 
        125, 125, 250, 125, 125,
        125, 125, 250, 125, 125,
        125, 125, 375, 125, 
        
        125, 125, 250, 125, 125, 
        125, 125, 250, 125, 125,
        125, 125, 250, 125, 125,
        125, 125, 375, 125, 
        
        125, 125, 250, 125, 125, 
        125, 125, 250, 125, 125,
        125, 125, 250, 125, 125,
        125, 125, 125, 250, 125,

        125, 125, 250, 125, 125, 
        250, 125, 250, 125, 
        125, 125, 250, 125, 125,
        125, 125, 375, 375
    };

    bool triggerAlarm;
    triggerAlarm = true;

    for (int i=0;i<76;i++){              //203 is the total number of music notes in the song
        // be able to break out of the song         
        uint8_t buttons = lcd.readButtons();
        if(buttons & BUTTON_SELECT) {            
            triggerAlarm = false;
            Serial.print("BREAK\n");
            return triggerAlarm;
        } 
        int wait = duration[i] * songspeed;
        tone(buzzer,notes[i],wait);          //tone(pin,frequency,duration)
        delay(wait); //delay is used so it doesn't go to the next loop before tone is finished playing    
    } 
    Serial.print("SONG\n");
    return triggerAlarm;
}
