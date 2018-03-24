#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Adafruit_CircuitPlayground.h>
#include "Notes.h"
#include <TimeLib.h>

static int colors[][3] = { {255,255,255}, {255,0,0}, {0,255,0}, {0,0,255}, {255,255,0}, {0,255,255}, {255,0,255},
                          {255,128,0}, {255,0,128}, {0,255,128}, {128,255,0}, {0,128,255}, {128,0,255},
                          {128,0,0}, {0,0,128}, {0,128,0}, {128,0,128}, {128,128,0}, {0,128,128} };

class Configuration {
public:
  virtual ~Configuration() {}
  int getTone() {
    return configurationTone;
  }
  virtual void loop() = 0;
  virtual bool upButtonPressed() = 0;
  virtual void downButtonPressed() = 0;

protected:

  int configurationTone;

};

#endif
