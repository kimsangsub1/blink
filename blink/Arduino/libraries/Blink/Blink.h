#ifndef Blink_h
#define Blink_h

#include "Arduino.h"

class Blink
{
  public:
    Blink(int pin);
    void on(int _delay);
    void off(int _delay);
  private:
    int _pin;
};

#endif
