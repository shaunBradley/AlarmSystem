#include "Keypad.h"
#include "Arduino.h"

Keypad::Keypad(const byte pin, char* characters, int characterCount)
{  
  this->isKeyDown = false;
  this->analogPin = pin;
  this->keys = new char[characterCount];
  this->voltages  = new int[characterCount];
  this->numKeys = characterCount;
  for(int i = 0; i < characterCount; i++)
  {
    this->keys[i] = characters[i];
    this->voltages[i] = (i+1)*1024/this->numKeys;
  }
  pinMode(this->analogPin, INPUT);
}

Keypad::~Keypad()
{
   delete[] this->keys;
   delete[] this->voltages;
}

int Keypad::GetVoltage()
{
  return analogRead(this->analogPin);
  //return (float)((analogRead(this->analogPin)/1024.0)*5.0);
}

bool Keypad::Range(int value, int compare, int range)
{
  if (value < (compare+range) && value > (compare-range))
  {return true;}
  else{return false;}
}

char Keypad::GetKey()
{
  int reading1 = analogRead(this->analogPin);
  delay(10);
  int reading2 = analogRead(this->analogPin);
  if (reading2 == 0){this->isKeyDown = false;}

  // if the two reading are withing 10/1024 of each other voltage is probally stable
  if (Range(reading1, reading2, 5))
  {
      for(int i = 0; i < this->numKeys; i++)
      {
       // if voltage is within (space between voltages)/4 it is a valid key press (/4 so it's less than the midpoint)
       if (Range(reading2, this->voltages[i], (this->voltages[1]-this->voltages[0])/3
       ) && !this->isKeyDown)
       { 
          this->isKeyDown = true;
          return this->keys[i];
       }
     } 
  }
  return NULL;
}

