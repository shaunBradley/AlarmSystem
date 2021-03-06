#ifndef Display_h
#define Display_h
#include <LiquidCrystal.h>
#include "Arduino.h"

class AlarmDisplay: public LiquidCrystal
{
  public:
    AlarmDisplay(uint8_t rs,  uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
    void SetTopMessage(String msg);
    void AppendToTopMessage(char c);
    void SetBottomMessage(String msg);
    void AppendToBottomMessage(char c);
    ~AlarmDisplay();
  private:
    char* StrAppend(const char* str, char c);
    char* StrCpy(String &str);
    char* topMsg;
    char* bottomMsg;
    void PrintToLCD();
};

#endif 
