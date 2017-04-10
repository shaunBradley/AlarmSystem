#include "Arduino.h"
#include "Keypad.h"

Keypad* keypad;
  
// the setup routine runs once when you press reset:
void setup()
{
  Serial.begin(9600);
  // NO THREE!!!!!!!!
  char keys[] = {'*','7','4','1','0','8','5','2','#','9', '6'};
  keypad = new Keypad(0, keys, 11);
}

// the loop routine runs over and over again forever:
void loop()
{
  char x = keypad->GetKey();
  if (x != ' ')
  {
     Serial.println(x); 
  }
}
