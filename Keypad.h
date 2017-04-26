#ifndef Keypad_h
#define Keypad_h

#include "Arduino.h"
class Keypad
{
  public:
    // character array index is pin# -1
    Keypad(const byte pin, char* characters, int characterCount);
    char GetKey();
    float GetVoltage();
    ~Keypad();
  private:
    int numKeys;
    bool isKeyDown;
    char* keys; 
    byte analogPin;
    int* voltages;
    bool Range(int value, int compare, int range);
};

#endif 
