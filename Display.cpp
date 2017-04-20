#include "Arduino.h"
#include "Display.h"

AlarmDisplay::AlarmDisplay(uint8_t rs,  uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3): ( rs, enable, d0, d1, d2, d3)
{
}

void AlarmDisplay::SetTopMessage(char* msg)
{
  this->topMsg = StrCpy(msg);
  PrintToLCD();
}

void AlarmDisplay::AppendToTopMessage(char c)
{
  this->topMsg = StrAppend(this->topMsg, c);
  PrintToLCD();
}

void AlarmDisplay::SetBottomMessage(char* c)
{
  this->bottomMsg = StrCpy(c);
  PrintToLCD();
}

void AlarmDisplay::AppendToBottomMessage(char c)
{
  this->bottomMsg = StrAppend(this->bottomMsg, c);
  PrintToLCD();
}

char* AlarmDisplay::StrAppend(char* str, char c)
{    
  char* newStr;
  if(str)
  {
    int len = 0;
    while(str[len] != '\0'){len++;}
    newStr = new char[len+2];
    for(int i = 0; i < len; i++)
    {
      newStr[i] = str[i];
    }
    newStr[len] = c;
    newStr[len+1] = '\0';
    delete[] str;
  }
  return newStr;
}

char* AlarmDisplay::StrCpy(char* str)
{
  char* newStr;
  if(str)
  {
    int len = 0;
    while(str[len] != '\0'){len++;}
    newStr = new char[len+1];
    for(int i = 0; i < len; i++)
    {
      newStr[i] = str[i];
    }
    newStr[len] = '\0';
  }
  return newStr;
}

void AlarmDisplay::PrintToLCD()
{
   this->clear();
   this->setCursor(0, 0);
   if(this->topMsg != NULL){this->print(this->topMsg); }
   this->setCursor(0, 1);
   if(this->bottomMsg != NULL){this->print(this->bottomMsg);}
}

AlarmDisplay::~AlarmDisplay()
{
  delete[] this->topMsg;
  delete[] this->bottomMsg;
}

