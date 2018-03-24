#ifndef CLOCKCONFIGURATION_H
#define CLOCKCONFIGURATION_H

#include "Configuration.h"

class ClockConfiguration: public Configuration {
  
public:

  ClockConfiguration(){
    mode = 0;  
    configurationTone = NOTE_E5;
  }
  ~ClockConfiguration(){}

  virtual void loop() {
    CircuitPlayground.clearPixels();
    CircuitPlayground.setBrightness(255);
    switch (mode) {
      case 0:  
        displayConfigurationAnimation();
        break;
      case 1:
        displayHours();
        break;
      case 2:
        displayMinutes();
        break;  
      case 3:
        displayDay();
        break;  
      case 4:
        displayMonth();
        break; 
      case 5:        
        displayYear();
        break;  
    }
  }

  //Don't know why but switch/case for 5 returned true
  virtual bool upButtonPressed() {
    if (mode == 0) {
        return true;
    } else if (mode == 1) {
        adjustTime(SECS_PER_HOUR);
        return false;
    } else if (mode == 2) {
        adjustTime(SECS_PER_MIN);
        return false;
    } else if (mode == 3) {
        adjustTime(SECS_PER_DAY);
        return false;
    } else if (mode == 4) {
        int clockMonth = month();
        clockMonth++;
        if (clockMonth > 12) {
          clockMonth = 1;
        }
        setTime(hour(),minute(),second(),day(),clockMonth,year());
        return false;
     } else if (mode == 5) {
        int clockYear = year();
        clockYear++;
        if (clockYear >= 2060) {
          clockYear = 2000;
        }
        setTime(hour(),minute(),second(),day(),month(),clockYear);
        return false;
     }
  }

  virtual void downButtonPressed() {
    mode++;
    if (mode == 6) {
      mode = 0;
      CircuitPlayground.playTone(configurationTone, 100);
    }
  }

private:

  int mode;

  void displayConfigurationAnimation(){
    
  }

  void displayHours() {
    if (hour() >= 12) {
      for(int i = 0; i < 10; i++) {
        CircuitPlayground.strip.setPixelColor(i,255,255,255);
      }      
    }
    int mHour = hour() > 12 ? hour() - 12  : hour();
    int color = hour() > 12 ? 4 : 0;
    if (mHour == 0) {
        CircuitPlayground.strip.setPixelColor(0,colors[color][0],colors[color][1],colors[color][2]);
        CircuitPlayground.strip.setPixelColor(9,colors[color][0],colors[color][1],colors[color][2]);
    } else if (mHour > 0 && mHour < 6) {
      CircuitPlayground.strip.setPixelColor(9-(mHour-1),colors[color][0],colors[color][1],colors[color][2]);
    } else if (mHour == 6) {
      CircuitPlayground.strip.setPixelColor(4,colors[color][0],colors[color][1],colors[color][2]);
      CircuitPlayground.strip.setPixelColor(5,colors[color][0],colors[color][1],colors[color][2]);
    } else if (mHour < 12) {
      CircuitPlayground.strip.setPixelColor(4-(mHour-7),colors[color][0],colors[color][1],colors[color][2]);
    }
    CircuitPlayground.strip.show();
  }
  
  void displayMinutes() {
    int mMinute = minute() / 5;
    int rest = minute() % 5;
    int color;
    switch (rest) {
      case 0:
        color = 0;
        break;
      case 1:
        color = 1;
        break;
      case 2:
        color = 2;
        break;  
      case 3:
        color = 3;
        break;  
      case 4:
        color = 4;
        break; 
    }
    if (mMinute == 0) {
        CircuitPlayground.strip.setPixelColor(0,colors[color][0],colors[color][1],colors[color][2]);
        CircuitPlayground.strip.setPixelColor(9,colors[color][0],colors[color][1],colors[color][2]);
    } else if (mMinute > 0 && mMinute < 6) {
      CircuitPlayground.strip.setPixelColor(9-(mMinute-1),colors[color][0],colors[color][1],colors[color][2]);
    } else if (mMinute == 6) {
      CircuitPlayground.strip.setPixelColor(4,colors[color][0],colors[color][1],colors[color][2]);
      CircuitPlayground.strip.setPixelColor(5,colors[color][0],colors[color][1],colors[color][2]);
    } else if (mMinute < 12) {
      CircuitPlayground.strip.setPixelColor(4-(mMinute-7),colors[color][0],colors[color][1],colors[color][2]);
    }
    CircuitPlayground.strip.show();
  }
  
  void displayDay() {
    int mDay = day() / 5;
    int rest = day() % 5;
    int color;
    switch (rest) {
      case 0:
        color = 0;
        break;
      case 1:
        color = 1;
        break;
      case 2:
        color = 2;
        break;  
      case 3:
        color = 3;
        break;  
      case 4:
        color = 4;
        break; 
    }    
    if (mDay >= 0 && mDay < 5) {
      CircuitPlayground.strip.setPixelColor(9-mDay,colors[color][0],colors[color][1],colors[color][2]);
    } else if (mDay == 5) {
      CircuitPlayground.strip.setPixelColor(4,colors[color][0],colors[color][1],colors[color][2]);
      CircuitPlayground.strip.setPixelColor(5,colors[color][0],colors[color][1],colors[color][2]);
    } else if (mDay > 5) {
      CircuitPlayground.strip.setPixelColor(4-(mDay-6),colors[color][0],colors[color][1],colors[color][2]);
    }
    CircuitPlayground.strip.show();
  }
  
  void displayMonth() {
    int color = 3;
    if (month() > 0 && month() < 6) {
      CircuitPlayground.strip.setPixelColor(9-(month() -1),colors[color][0],colors[color][1],colors[color][2]);
    } else if (month() == 6) {
      CircuitPlayground.strip.setPixelColor(4,colors[color][0],colors[color][1],colors[color][2]);
      CircuitPlayground.strip.setPixelColor(5,colors[color][0],colors[color][1],colors[color][2]);
    } else if (month() < 12) {
      CircuitPlayground.strip.setPixelColor(4-(month()-7),colors[color][0],colors[color][1],colors[color][2]);
    } else if (month() == 12) {
      CircuitPlayground.strip.setPixelColor(9,colors[color][0],colors[color][1],colors[color][2]);
      CircuitPlayground.strip.setPixelColor(0,colors[color][0],colors[color][1],colors[color][2]);
    }
    CircuitPlayground.strip.show();
  }
  
  void displayYear() {
    int mYear = (year() - 2000) / 5;
    int rest = (year() - 2000) % 5;
    int color;
    switch (rest) {
      case 0:
        color = 0;
        break;
      case 1:
        color = 1;
        break;
      case 2:
        color = 2;
        break;  
      case 3:
        color = 3;
        break;  
      case 4:
        color = 4;
        break; 
    }
    if (mYear == 0) {
        CircuitPlayground.strip.setPixelColor(0,colors[color][0],colors[color][1],colors[color][2]);
        CircuitPlayground.strip.setPixelColor(9,colors[color][0],colors[color][1],colors[color][2]);
    } else if (mYear >= 0 && mYear < 5) {
      CircuitPlayground.strip.setPixelColor(9-mYear,colors[color][0],colors[color][1],colors[color][2]);
    } else if (mYear == 5) {
      CircuitPlayground.strip.setPixelColor(4,colors[color][0],colors[color][1],colors[color][2]);
      CircuitPlayground.strip.setPixelColor(5,colors[color][0],colors[color][1],colors[color][2]);
    } else if (mYear > 5) {
      CircuitPlayground.strip.setPixelColor(4-(mYear-6),colors[color][0],colors[color][1],colors[color][2]);
    }
    CircuitPlayground.strip.show();
  }

};

#endif
