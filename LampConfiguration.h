#ifndef LAMPCONFIGURATION_H
#define LAMPCONFIGURATION_H

#include "Configuration.h"

class LampConfiguration: public Configuration {
  
public:

  LampConfiguration(){
    configurationTone = NOTE_D5;
    currentColor = 0;
  }
  ~LampConfiguration(){}

  virtual void loop() {
    CircuitPlayground.clearPixels();
    CircuitPlayground.setBrightness(255);
    for(int i = 0; i < 10; i++) {
      CircuitPlayground.strip.setPixelColor(i, colors[currentColor][0], colors[currentColor][1], colors[currentColor][2]);
    }
    CircuitPlayground.strip.show();
  }

  virtual bool upButtonPressed() {
    return true;
  }

  virtual void downButtonPressed() {
    currentColor++;
    if (currentColor == sizeof(colors)/sizeof(colors[0]) ) {
      currentColor = 0; 
    }
  }

private:
  int currentColor;
};

#endif
