/*
 * ESPloitV2
 * WiFi controlled HID Keyboard Emulator
 * By Corey Harding of www.Exploit.Agency / www.LegacySecurityGroup.com
 * Special thanks to minkione for helping port/test original V1 code to the Cactus Micro rev2
 * ESPloit is distributed under the MIT License. The license and copyright notice can not be removed and must be distributed alongside all future copies of the software.
 * MIT License
    
    Copyright (c) [2017] [Corey Harding]
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

//Requires Keyboard and SoftwareSerial library
#include <Keyboard.h>
#include <Mouse.h>
//#include <SoftwareSerial.h>

//Setup RX and TX pins to be used for the software serial connection
//const int RXpin=10;
//const int TXpin=11;
//SoftwareSerial SOFTserial(RXpin,TXpin);

//Prints debug lines via serial if set to 1
//const int debug=0;

String version = "2.2";

//Used later for determining if we are ready to release a key press or a combination of key presses
int keypressdone=0;

//Start serial interfaces and HID keyboard emulator
void setup() { 

//Start up the ESP 8266
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);
 
//  SOFTserial.begin(38400);
//  if(debug==1) {
//    Serial.begin(9600);
//  }
  Serial1.begin(38400);
  Serial.begin(38400);
  Keyboard.begin();
}



void pressCmd(cmd_paramter) {
  keypressdone=1;
//  String fullkeys = SOFTserial.readString();
  int str_len = cmd_paramter.length()+1; 
  char keyarray[str_len];
  cmd_paramter.toCharArray(keyarray, str_len);
//  SOFTserial.println(cmd_paramter);
  char delimiter[] = "+";
  char *keypart;
  keypart = strtok(keyarray, delimiter);
  while(keypart != NULL) {
//    if(debug==1) {
//      Serial.print("Pressing: "); //Debug line
//      Serial.println(keypart); //Debug line
//    }
    int key = atoi(keypart);
    delay(25);
    Keyboard.press(key);
    keypart = strtok(NULL, delimiter);
  }
  
  if(keypressdone==1) {
    delay(25);
    Keyboard.releaseAll();
//    if(debug==1) {
//      Serial.println("Releasing all keys."); //Debug line
//    }
    keypressdone=0;
  }
}


void printCmd(cmd_paramter) {
//    if(debug==1) {
//      Serial.print("Printing: "); //Debug line
//      Serial.println(cmd_paramter); //Debug line
//    }
  delay(25);
  Keyboard.print(cmd_paramter);
}


void printLineCmd(cmd_paramter) {
//    if(debug==1) {
//      Serial.print("Printing Line: "); //Debug line
//      Serial.println(keycode); //Debug line
//    }
  delay(25);
  Keyboard.print(cmd_paramter);
  delay(25);
  Keyboard.press(KEY_RETURN);
  delay(25);
  Keyboard.release(KEY_RETURN);
}


void mouseUpCmd(cmd_paramter) {
  int mousemoveamt = cmd_paramter.toInt();
  delay(25);
  Mouse.begin();
  Mouse.move(0, 0);
  Mouse.move(0, mousemoveamt*-1);
  Mouse.end();
}


void mouseDownCmd(cmd_paramter) {
  int mousemoveamt = cmd_paramter.toInt();
  delay(25);
  Mouse.begin();
  Mouse.move(0, 0);
  Mouse.move(0, mousemoveamt);
  Mouse.end();
}


void mouseLeftCmd(cmd_paramter) {
  int mousemoveamt = cmd_paramter.toInt();
  delay(25);
  Mouse.begin();
  Mouse.move(0, 0);
  Mouse.move(mousemoveamt*-1, 0);
  Mouse.end();
}


void mouseRightCmd(cmd_paramter) {
  int mousemoveamt = cmd_paramter.toInt();
  delay(25);
  Mouse.begin();
  Mouse.move(0, 0);
  Mouse.move(mousemoveamt, 0);
  Mouse.end();
}


void mouseClickRightCmd() {
  delay(25);
  Mouse.begin();
  Mouse.click(MOUSE_RIGHT);
  Mouse.end();
}


void mouseClickLeftCmd() {
  delay(25);
  Mouse.begin();
  Mouse.click(MOUSE_LEFT);
  Mouse.end();
}


void mouseClickMiddle() {
  delay(25);
  Mouse.begin();
  Mouse.click(MOUSE_MIDDLE);
  Mouse.end();
}


void getVersionCmd() {
  Serial1.println(String()+"Version:"+version);
}

//Do-IT!
void loop() {  
//  while (SOFTserial.available()) {
  while (Serial.available()) {
    String serial_link = Serial.readStringUntil('\n');
    Serial1.println(serial_link);
    Serial.println("Relaying command to connected ESP device.");
  }
  while (Serial1.available()) {
//    String cmd = SOFTserial.readStringUntil(':');
    String cmd = Serial1.readStringUntil(':');
    String cmd_paramter = SOFTserial.readString();
    String cmd_paramter = Serial1.readStringUntil('\n');
    //If command equals "Press:X" or "Press:X+Y+ETC"

    if(cmd == "For"):
      String* loop_content = malloc(1 * sizeof(char*));
      

    else if(cmd == "Press"){
      press(cmd_paramter);
    }
  
    //If command equals "Print:X"
    else if(cmd == "Print") {
      printCmd(cmd_paramter);
    }

    //If command equals "PrintLine:X"
    else if(cmd == "PrintLine") {
      printLineCmd(cmd_paramter);
    }

//mouse up
    //If command equals "MouseMoveUp:X"
    else if(cmd == "MouseMoveUp") {
      mouseUpCmd(cmd_paramter);
    }

//mouse down
    //If command equals "MouseMoveDown:X"
    else if(cmd == "MouseMoveDown") {
      mouseDownCmd(cmd_paramter);
    }

//mouse left
    //If command equals "MouseMoveLeft:X"
    else if(cmd == "MouseMoveLeft") {
      mouseLeftCmd(cmd_paramter);
    }

//mouse right
    //If command equals "MouseMoveRight:X"
    else if(cmd == "MouseMoveRight") {
      mouseRightCmd(cmd_paramter);
    }

//mouse click command EX: MouseClickLEFT: MouseClickRIGHT: MouseClickMIDDLE:
    //If command equals "MouseClickX:"
    else if(cmd == "MouseClickRIGHT") {
      mouseClickRightCmd();
    }
    else if(cmd == "MouseClickLEFT") {
      mouseClickLeftCmd();
    }
    else if(cmd == "MouseClickMIDDLE") {
      mouseClickMiddle();
    }
    else if(cmd == "GetVersion") {
      getVersionCmd();
    }

  }
}
