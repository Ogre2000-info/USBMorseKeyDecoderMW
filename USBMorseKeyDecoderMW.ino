#include "Keyboard.h"
#include <map>

/*                    GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
*/

/*
For Seeduino XIAO board add Boards file
https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json
Install Seed SAMD Boards
test
*/

//Disable USB Diagnostics COM Port
//Currently not working!
#define DISABLE_USB_SERIAL

#define KEY_PIN   7 // Morse Key Pin
#define BUZZER    8 // Buzzer pin

int ToneFrequency = 1100;

// Program Description:
// *****************************************
char Program_ID[] = "CW USB Key Decoder";
char VerNo[] = "0.5";
char ProgDate[] = "08-09-2024";
char Copywrite[] = "Copyright (C) 2024, Marek Wagrodzki, All Rights Reserved.";
char License[] = "GNU GENERAL PUBLIC LICENSE";
// *****************************************

unsigned long DotTime = 200;
unsigned long DashTime = DotTime * 3;
unsigned long LetterGapTime = DotTime * 3;
unsigned long PressStartTime = 0;
unsigned long DepressStartTime = 0;
unsigned long PressDurationTime = 0;
unsigned long DepressDurationTime = 0;
boolean CurrentKeyState = false;
boolean LastKeyState = false;
String MorseSign = ""; //Store Morse code sign as Morse string

//Constant Map for 2nd Morse algorithm
std::map<String, int> MorseConstantMap = {
{ ".--..-.", 0x88},           //WinKey
{ "--.--", 0xE3},             //Start Menu
{ "......", KEY_BACKSPACE},   //Backspace
{ ".-.-", KEY_RETURN},        //RETURN
{ ".-..-.", 0x22},            //"
{ ".----.", 0x27},            //Apostrophe
{ "-.--.", 0x28},             //(
{ "-.--.-", 0x29},            //)
{ ".-.-.", 0x2B },            //+
{ "--..--", 0x2C },           //,
{ "-....-", 0x2D },           //-
{ ".-.-.-", 0x2E },           //.
{ "-..-.", 0x2F },            ///
{ "-----", 0x30},             //0
{ ".----", 0x31},             //1
{ "..---", 0x32},             //2
{ "...--", 0x33},             //3
{ "....-", 0x34},             //4
{ ".....", 0x35},             //5
{ "-....", 0x36},             //6
{ "--...", 0x37},             //7
{ "---..", 0x38},             //8
{ "----.", 0x39},             //9
{ "---...", 0x3A },           //:
{ "-...-", 0x3D },            //=
{ "..--..", 0x3F},            //?
{ ".--.-.", 0x40},            //@
{ ".-", 0x41},                //A
{ "-...", 0x42},              //B
{ "-.-.", 0x43},              //C
{ "-..", 0x44},               //D
{ ".", 0x45},                 //E
{ "..-.", 0x46},              //F
{ "--.", 0x47},               //G
{ "....", 0x48},              //H
{ "..", 0x49},                //I
{ ".---", 0x4A },             //J
{ "-.-", 0x4B },              //K
{ ".-..", 0x4C },             //L
{ "--", 0x4D },               //M
{ "-.", 0x4E },               //N
{ "---", 0x4F },              //O
{ ".--.", 0x50},              //P
{ "--.-", 0x51},              //Q
{ ".-.", 0x52},               //R
{ "...", 0x53},               //S
{ "-", 0x54},                 //T
{ "..-", 0x55},               //U
{ "...-", 0x56},              //V
{ ".--", 0x57},               //W
{ "-..-", 0x58},              //X
{ "-.--", 0x59},              //Y
{ "--..", 0x5A },             //Z
{ "..--", 0x20},              //Space
{ "...-.-", 0x2A},            //*
{ "....-...-", 0x5C }         //BackSlash

};


//Map morse string ".-" to value Function
int getConstantValue(const String& name) {
  auto it = MorseConstantMap.find(name);
  if (it != MorseConstantMap.end()) {
    return it->second;
  }
  return 0x0; // Return Value if code is not found.
}

void setup() {
  Keyboard.begin();

  pinMode(KEY_PIN, INPUT_PULLUP); //MorseKey input
  pinMode(BUZZER, OUTPUT); //Buzzer output
  
  //For test only - blink Board LED (pin 13)
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  tone(BUZZER, ToneFrequency);
  delay(1000);
  noTone(BUZZER);
}

void loop() {

  // Read Morse Key status
  CurrentKeyState = !digitalRead(KEY_PIN);
  
  // Light ON Board LED when Morse Key is pressed
  //Remember - LED_BUILTIN works in negative logic
  if (!CurrentKeyState)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    noTone(BUZZER);
  } else
  {
    digitalWrite(LED_BUILTIN, LOW);
    tone(BUZZER, ToneFrequency);    
  }

  //Glue Morse Key from .-
  if (CurrentKeyState != LastKeyState ) {
    //Pressed
    if(CurrentKeyState)
    {
      PressStartTime = millis();
      if (DepressDurationTime > LetterGapTime)
      {
        MorseSign = "";
      }
    }
    else
    //DePressed
    {
      DepressStartTime = millis();
      PressDurationTime = DepressStartTime - PressStartTime;

      //.- depends on Key press duration time
      if (PressDurationTime < DotTime)
      {
        MorseSign = MorseSign + ".";
      }
      else if (PressDurationTime < DashTime)
      {
        MorseSign = MorseSign + "-";
      }
      else if (PressDurationTime > LetterGapTime)
      {
//If PressTime is longer than usual
        MorseSign = ".-.-";
//Empty at the moment
      }
      else
      {
//If PressTime is unknown
//Empty at the moment
      }
    }

  }

  if (!CurrentKeyState) //When depressed
  {
    DepressDurationTime = millis() - DepressStartTime;
    if ((DepressDurationTime > 1000) && (MorseSign.length() > 0))
    {
        // ConstantMap Decoding
        Keyboard.write(getConstantValue(MorseSign));
      MorseSign = "";
    }
  }

LastKeyState = CurrentKeyState;
delay(40);

} //  End of the Main Loop
