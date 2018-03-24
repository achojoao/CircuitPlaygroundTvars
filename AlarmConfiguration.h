#ifndef ALARMCONFIGURATION_H
#define ALARMCONFIGURATION_H

#include "Configuration.h"

class AlarmConfiguration: public Configuration {
  
public:

  AlarmConfiguration(){
    configurationTone = NOTE_C5;
    mode = 0;
    alarmHour = 8;
    alarmMinute = 0;
    alarmDays[0] = true;
    alarmDays[1] = false;
    alarmSnooze = true;
  }
  ~AlarmConfiguration(){}

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
        displayDays();
        break;  
      case 4:
        displaySnooze();
        break;  
    }
  }

  virtual bool upButtonPressed() {
    switch (mode) {
      case 0:        
        return true;
        break;
      case 1:
        alarmHour++;
        if (alarmHour > 23) {
          alarmHour = 0;
        }
        return false;
        break;
      case 2:
        alarmMinute += 15;
        if (alarmMinute >= 60) {
          alarmMinute = 0;
        }
        return false;
        break;  
      case 3:
        if (alarmDays[0] && alarmDays[1]) {
          alarmDays[0] = false;
          alarmDays[1] = false;
        } else if (alarmDays[0]) {
          alarmDays[0] = false;
          alarmDays[1] = true;
        } else if (alarmDays[1]) {
          alarmDays[0] = true;
          alarmDays[1] = true;
        } else {
          alarmDays[0] = true;
          alarmDays[1] = false;
        }
        return false;
        break;  
      case 4:
        alarmSnooze = !alarmSnooze;        
        return false;
        break;  
    }
  }

  virtual void downButtonPressed() {    
    mode++;
    if (mode == 5) {
      mode = 0;
      CircuitPlayground.playTone(configurationTone, 100);
    }
  }

  int getHour() {
    return alarmHour;
  }  

  int getMinute() {
    return alarmMinute;
  }

  bool getSnooze() {
    return alarmSnooze;
  }

  bool isEnabledToday() {
    if ((weekday() >= 2  || weekday() < 7) && alarmDays[0]) {
      return true;
    } else if ((weekday() == 7 || weekday() == 1) && alarmDays[1]) {
      return true; 
    } else {
      return false;
    }
  }

private:
  int mode;
  int alarmHour;
  int alarmMinute;
  bool alarmDays[2];
  bool alarmSnooze;

  void displayConfigurationAnimation() {
    
  }

  void displayHours() {
    if (alarmHour >= 12) {
      for(int i = 0; i < 10; i++) {
        CircuitPlayground.strip.setPixelColor(i,255,255,255);
      }      
    }
    int mHour = alarmHour > 12 ? alarmHour - 12  : alarmHour;
    int color = alarmHour > 12 ? 4 : 0;
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
    int color = alarmMinute / 15;
    for(int i = 0; i < 10; i++) {
      CircuitPlayground.strip.setPixelColor(i,colors[color][0],colors[color][1],colors[color][2]);
    }
    CircuitPlayground.strip.show();
  }
  
  void displayDays() {
    int color = 5;
    if (alarmDays[0] && alarmDays[1]) {
      color = 6;
    } else if (alarmDays[0]) {
      color = 7;
    } else if (alarmDays[1]) {
      color = 8;
    }
    for(int i = 0; i < 10; i++) {
      CircuitPlayground.strip.setPixelColor(i,colors[color][0],colors[color][1],colors[color][2]);
    }
    CircuitPlayground.strip.show();
  }
  
  void displaySnooze() {      
    int color = alarmSnooze ? 1 : 2;  
    for(int i = 0; i < 10; i++) {
      CircuitPlayground.strip.setPixelColor(i,colors[color][0],colors[color][1],colors[color][2]);
    }
    CircuitPlayground.strip.show();
  }
        
};

#endif
