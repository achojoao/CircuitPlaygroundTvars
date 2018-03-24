#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>

#include "Configuration.h"
#include "ClockConfiguration.h"
#include "AlarmConfiguration.h"
#include "LampConfiguration.h"
#include "Notes.h"

#define NO_BUTTON -1
#define UP_BUTTON 9
#define DOWN_BUTTON 2
#define NO_CONFIGURATION -1
#define TAP_THRESHOLD   100
#define SNOOZE_MINUTES  2

ClockConfiguration clockConfiguration;
AlarmConfiguration alarmConfiguration;
LampConfiguration lampConfiguration;

int currentConfiguration = -1;
Configuration* configurations[] = {
  &alarmConfiguration,
  &lampConfiguration,
  &clockConfiguration
};

int tapButton[] = {UP_BUTTON,DOWN_BUTTON};
int button = NO_BUTTON;
int bright = 255;
bool alarmEnabled = true;
bool alarmRinging = false;
bool alarmStopped = false;
int alarmNotes = 61; 
int alarmMelody[] = {NOTE_E5,0,NOTE_F5,NOTE_G5,0,NOTE_C6,0,NOTE_D5,0,NOTE_E5,NOTE_F5,0,NOTE_G5,0,NOTE_A5,NOTE_B5,0,NOTE_F6,0,NOTE_A5,0,NOTE_B5,NOTE_C6,NOTE_D6,NOTE_E6,NOTE_E5,0,NOTE_F5,NOTE_G5,0,NOTE_C6,0,NOTE_D6,0,NOTE_E6,NOTE_F6,0,NOTE_G5,0,NOTE_G5,NOTE_E6,0,NOTE_D6,0,NOTE_G5,NOTE_E6,0,NOTE_D6,0,NOTE_G5,NOTE_E6,0,NOTE_D6,0,NOTE_G5,NOTE_F6,0,NOTE_E6,0,NOTE_D6,NOTE_C6 }; 
int alarmNoteDurations[] = {8,16,16,16,16,2,8,8,16,16,2,4,8,16,16,16,16,2,8,8,16,16,4,4,4,8,16,16,16,16,2,8,8,16,16,2,4,8,16,16,8,8,8,16,16,8,8,8,16,16,8,8,8,16,16,8,8,8,16,16,2};
int alarmHour;
int alarmMinute;
int snoozeHour;
int snoozeMinute;
bool snoozeActivated = false;
bool clapped = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Circuit Playground Tvärs Lamp!");  
  CircuitPlayground.begin();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
  CircuitPlayground.setAccelTap(2, TAP_THRESHOLD);    
  setTime(0,0,0,1,1,2018);
  rainbowCycle();
  playCoin();
  CircuitPlayground.strip.clear();
  CircuitPlayground.strip.show();
}

void loop() {

  button = getButtonPress();
  delay(200);
  switch (button) {
    case UP_BUTTON:
      if (currentConfiguration == NO_CONFIGURATION) {
        currentConfiguration++;
        CircuitPlayground.playTone(configurations[currentConfiguration]->getTone(), 100);
        configurations[currentConfiguration]->upButtonPressed();          
      } else if (configurations[currentConfiguration]->upButtonPressed()) {          
        currentConfiguration++;          
        if (currentConfiguration >= (sizeof(configurations)/sizeof(Configuration*))) {
          currentConfiguration = NO_CONFIGURATION;
          playCoin();
        }  else {
          CircuitPlayground.playTone(configurations[currentConfiguration]->getTone(), 100);
        }
      }  
      break;
    case DOWN_BUTTON: 
      if (currentConfiguration > NO_CONFIGURATION) {
        configurations[currentConfiguration]->downButtonPressed();
      } else {
        dimmLamp();
      }
      break;
    default:
      if (currentConfiguration > NO_CONFIGURATION) {
        configurations[currentConfiguration]->loop();
      } else {
        CircuitPlayground.setBrightness(bright);
        checkAlarm();
        checkTapForHour();
        checkClapForLight();
      }
      break;
  }
  
}

void rainbowCycle() {
  for (int i=0; i<500; i++) {
    for(int j=0; j<10; j++) {
      CircuitPlayground.strip.setPixelColor(j, CircuitPlayground.colorWheel(((j * 256 / 10) + i) & 255));
    }
    CircuitPlayground.strip.show();
    delay(2);
  }
}

void playCoin() {
  CircuitPlayground.playTone(NOTE_B6,75);
  CircuitPlayground.playTone(0,75);
  CircuitPlayground.playTone(NOTE_E7,400);
}

int getButtonPress() {
  for (int b=0; b<2; b++) {
    if (CircuitPlayground.readCap(tapButton[b]) > TAP_THRESHOLD) {
      return tapButton[b];
    }
  }
  return NO_BUTTON;
}

void dimmLamp() {  
  for(int i=0; i<10; i++) {
    CircuitPlayground.strip.setPixelColor(i, 255, 255, 255);  
  }
  CircuitPlayground.strip.show();
  bright -= 40;
  if (bright < 40 && bright > 0) {
    bright = 0;
  } else if (bright < 0) {
    bright = 255;
  }
}

void checkTapForHour() {
  if (!alarmRinging) {
    float x = 0;
    float y = 0;
    float z = 0;
    for (int i=0; i<10; i++) {
      x += CircuitPlayground.motionX();
      y += CircuitPlayground.motionY();
      z += CircuitPlayground.motionZ();
      delay(1);
    }
    x /= 10;
    y /= 10;
    z /= 10;    
    float totalAccel = sqrt(y*y);
    if (totalAccel > 0.5) {
      displayHour();
    }
  }
}

void displayHour() {
  CircuitPlayground.strip.clear();
  CircuitPlayground.strip.setBrightness(255);
  for (int j=0; j<(hour()/5); j++) {  
    for(int i=0; i<10; i++) {
      CircuitPlayground.strip.setPixelColor(i, 0, 255, 0);  
    }
    CircuitPlayground.strip.show();
    delay(500);
    CircuitPlayground.strip.clear();
    CircuitPlayground.strip.show();
    delay(500);
  }
  for (int j=0; j<(hour()%5); j++) {  
    for(int i=0; i<10; i++) {
      CircuitPlayground.strip.setPixelColor(i, 0, 255, 0);  
    }
    CircuitPlayground.strip.show();
    delay(250);
    CircuitPlayground.strip.clear();
    CircuitPlayground.strip.show();
    delay(250);
  }
  for (int j=0; j<=(minute()/10); j++) {  
    for(int i=0; i<10; i++) {
      CircuitPlayground.strip.setPixelColor(i, 0, 0, 255);  
    }
    CircuitPlayground.strip.show();
    delay(500);  
    CircuitPlayground.strip.clear();
    CircuitPlayground.strip.show();
    delay(500);  
  }
}

void checkClapForLight() {   
  uint16_t light = CircuitPlayground.lightSensor();
  int level = (int)lerp(light,0,255,0.0,5.0);
  if (clapped || (!clapped && level == 0)) {
    Serial.println(clapped);
    float peakToPeak = CircuitPlayground.mic.soundPressureLevel(10);
    peakToPeak = max(10, peakToPeak);
    Serial.println(peakToPeak);        
    if (peakToPeak > 65) {
      if (clapped) {
        CircuitPlayground.strip.clear();
      } else {
        for(int i=0; i<10; i++) {
          CircuitPlayground.strip.setPixelColor(i, 255, 255, 255);  
        }
        CircuitPlayground.strip.setBrightness(255);
      }
      CircuitPlayground.strip.show();
      if (!clapped) {
        delay(1000);
      }
      clapped = !clapped;      
    }
  }
}

float lerp(float x, float xmin, float xmax, float ymin, float ymax) {
  if (x >= xmax) {
    return ymax;
  }
  if (x <= xmin) {
    return ymin;
  }
  return ymin + (ymax-ymin)*((x-xmin)/(xmax-xmin));
}

void checkAlarm() {  
  if (alarmEnabled) {
    if (alarmRinging) {
      if (!alarmStopped) {    
        playAlarm();
      }
    } else {
      calculateAlarm();    
      calculateSnooze();
      if ((alarmHour == hour() && alarmMinute == minute()) || 
          (snoozeActivated && snoozeHour == hour() && snoozeMinute == minute())) {
        if (!alarmStopped) {            
          alarmRinging = true;
        }  
      } else {
        alarmRinging = false;
        alarmStopped = false;
      }
    }
  }
}

void stopAlarm() {
  alarmRinging = false;
  alarmStopped = true;
}

void playAlarm() {    
  for (int thisNote = 0; thisNote < alarmNotes && !alarmStopped; thisNote++) {
      int noteDuration = 750 / alarmNoteDurations[thisNote];
      CircuitPlayground.playTone(alarmMelody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      button = getButtonPress();
      if (button == UP_BUTTON || button == DOWN_BUTTON) {
        stopAlarm();
      }
      delay(pauseBetweenNotes);
    }
}

void calculateAlarm() {
  alarmHour = alarmConfiguration.getHour();
  alarmMinute = alarmConfiguration.getMinute();
  alarmEnabled = alarmConfiguration.isEnabledToday();
}

void calculateSnooze() {
  snoozeHour = alarmHour;
  snoozeMinute = alarmMinute + SNOOZE_MINUTES;
  if (snoozeMinute>=60) {
    snoozeHour++;
    snoozeMinute = snoozeMinute - 60;
  }
  snoozeActivated = alarmConfiguration.getSnooze();
}

