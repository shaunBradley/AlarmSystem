#include "Arduino.h"
#include "Keypad.h"
#include "Display.h"

Keypad* keypad;
AlarmDisplay* lcd;
char* password;
char* input;
boolean revieveComplete = false;

void setup()
{
  Serial.begin(9600);
  // in order of voltage division
  char keys[] = {'*','7','4','1','0','8','5','2','#','9', '6', '3'};
  keypad = new Keypad(0, keys, 12);
  lcd = new AlarmDisplay(12, 11, 5, 4, 3, 2);
  lcd->begin(16,2); 
  lcd ->SetTopMessage(" ");
  lcd->SetBottomMessage(">");
}

char* appendStr(char* str, char c)
{    
  char* newStr;
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
  return newStr;
}

// called by arduino event manager
void serialEvent()
{      
    while (Serial.available() && !revieveComplete) 
    {
      char c = (char)Serial.read();
      input = appendStr(input, c);
      if((char)Serial.peek() == '#')
      {
        lcd->SetTopMessage(input);
        revieveComplete = true;
      }
    }
}

void loop()
{
  char x = keypad->GetKey();
  if (x)
  {
    password = appendStr(password, x);
    lcd->AppendToBottomMessage(x);
    if(x == '*')
    {
      lcd->SetBottomMessage(">");
    }
    if (x == '#')
    {
      lcd->SetBottomMessage("sending...");
      // still sending the '#' so pi can use that character as a end char
      Serial.write(password);
      delete[] password;
    }
  }
}
