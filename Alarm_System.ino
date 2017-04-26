#include "Arduino.h"
#include "Keypad.h"
#include "Display.h"
#define terminator '\n'
#define lcdTogglePin 13
/*********************************NOTES*********************************/
//   sending  print(arg) via serial will print arg to lcd              //
//   if you want to send a with close bracket then use excape char  '\'//
//   sending sleep(true) will sleep the uno                            //
//   sending sleep(flalse) will wake it up                             //
/***********************************************************************/

// objects / variables
Keypad* keypad;
AlarmDisplay* lcd;
String input = "";
String password = "";
boolean receiveComplete = false;
bool keysDisabled = false;
bool hidepassword = false;
bool isSleeping = false;

enum receivedCommand{ printLCD, sleep };

struct commandAndData
{
  public:
    commandAndData(String s, receivedCommand c)
    {
      this->data = s;
      this->command = c;
    }
    receivedCommand command;
    String data;
};

const String cmd[2] = {"print(", "sleep("};

// functions

void setup()
{
  Serial.begin(9600);
  pinMode(lcdTogglePin, OUTPUT);
  digitalWrite(lcdTogglePin, !isSleeping);
  input.reserve(16);
  // in order of voltage division
  char keys[] = {'*','7','4','1','0','8','5','2',terminator,'9', '6', '3'};
  keypad = new Keypad(0, keys, 12);
  lcd = new AlarmDisplay(12, 11, 5, 4, 3, 2);
  lcd->begin(16,2);
  lcd->SetTopMessage("");
  lcd->SetBottomMessage(">");
}

int lengthTo(String& s, int startIndex, char endChar)
{
  int i = startIndex;
  while(s[i] != endChar)
  {
    i++;
  }
  return i;
}

String getArg(const String& func, const String& argv, char terminatingChar)
{
  String s = "";
  if(argv.length())
  {
   int i = func.length();
   while(argv[i] != terminatingChar)
   {
      if(argv[i] == '\\') {  i++; }
      s += argv[i];
      i++;
    }
  }
  return s;
}

commandAndData GetCommand(String* s)
{
  for(int i = 0; i < 2; i++)
  {
    if(s->length() >= cmd[i].length())
    {
      if(s->substring(0, cmd[i].length()) ==  cmd[i])
      {
        return commandAndData(getArg(cmd[i], *s, ')'), i);
      }
    }
  }
  return commandAndData("", -1);
}

String ToStars(String& pwd)
{
  String s = "";
  for(int i = 0; i < pwd.length(); i++)
  {
    s += '*';
  }
  return s;
}

void loop()
{
  // print the string when a newline arrives:
  if (receiveComplete) 
  {
    commandAndData c = GetCommand(&input);
    switch(c.command)
    {
      case printLCD:
        lcd->SetTopMessage(c.data);
        lcd->SetBottomMessage(">" + password);
        keysDisabled = false;
        break;
      case sleep:
        if(c.data == "true")
        {
            digitalWrite(lcdTogglePin, LOW);
            isSleeping = true;
        }
        else if(c.data == "false")
        {
            digitalWrite(lcdTogglePin, HIGH);
            isSleeping = false;
        }
        break;
      default:
        break;
    }
    input = "";
    receiveComplete = false;
  }
  
  if(!isSleeping)
  {
    char x = keypad->GetKey();
    if (x && !keysDisabled)
    {
      if(x == '*')
      {
        if(password == "")
        {
          hidepassword = !hidepassword;
          if(hidepassword)
          {
             lcd->SetBottomMessage(">hide pwd mode");
          }
          else
          {
            lcd->SetBottomMessage(">show pwd mode");
          }
        }
        else
        {
           lcd->SetBottomMessage(">");
        }
        password = "";
      }
      else if (x == terminator)
      {
        if(password.length())
        {
           keysDisabled = true;
           password += x;
           Serial.print(password);
           lcd->SetBottomMessage("waiting...");
           password = "";
        }
      }
      else
      {
         password += x;
        if(hidepassword){ lcd->SetBottomMessage(">" + ToStars(password)); }
        else{ lcd->SetBottomMessage(">" + password); }
      }
    } 
  }
}

void serialEvent() {
  while (Serial.available()) 
  {  
    char c = (char)Serial.read();
    if (c == terminator)
    { receiveComplete = true;  }
    else{ input += c; }
  }
}
