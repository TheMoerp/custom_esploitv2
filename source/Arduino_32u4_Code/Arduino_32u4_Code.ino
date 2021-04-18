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



void pressCmd(cmd_parameter) {
  keypressdone=1;
//  String fullkeys = SOFTserial.readString();
  int str_len = cmd_parameter.length()+1; 
  char keyarray[str_len];
  cmd_parameter.toCharArray(keyarray, str_len);
//  SOFTserial.println(cmd_parameter);
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


void printCmd(cmd_parameter) {
//    if(debug==1) {
//      Serial.print("Printing: "); //Debug line
//      Serial.println(cmd_parameter); //Debug line
//    }
  delay(25);
  Keyboard.print(cmd_parameter);
}


void printLineCmd(cmd_parameter) {
//    if(debug==1) {
//      Serial.print("Printing Line: "); //Debug line
//      Serial.println(keycode); //Debug line
//    }
  delay(25);
  Keyboard.print(cmd_parameter);
  delay(25);
  Keyboard.press(KEY_RETURN);
  delay(25);
  Keyboard.release(KEY_RETURN);
}


void mouseUpCmd(cmd_parameter) {
  int mousemoveamt = cmd_parameter.toInt();
  delay(25);
  Mouse.begin();
  Mouse.move(0, 0);
  Mouse.move(0, mousemoveamt*-1);
  Mouse.end();
}


void mouseDownCmd(cmd_parameter) {
  int mousemoveamt = cmd_parameter.toInt();
  delay(25);
  Mouse.begin();
  Mouse.move(0, 0);
  Mouse.move(0, mousemoveamt);
  Mouse.end();
}


void mouseLeftCmd(cmd_parameter) {
  int mousemoveamt = cmd_parameter.toInt();
  delay(25);
  Mouse.begin();
  Mouse.move(0, 0);
  Mouse.move(mousemoveamt*-1, 0);
  Mouse.end();
}


void mouseRightCmd(cmd_parameter) {
  int mousemoveamt = cmd_parameter.toInt();
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
    String cmd_parameter = SOFTserial.readString();
    String cmd_parameter = Serial1.readStringUntil('\n');
    

    // fic command equals "For: X"
    if(cmd == "For"):
      int iterations = cmd_parameter;

      // read in loop content
      int max_loop_content = 10;
      String loop_content[max_loop_content];
      int i;
      for(i = 0; i < max_loop_content; i++) {
        String content_cmd = Serial1.readStringUntil(':');
        String content_parameter = SOFTserial.readString();
        String content_parameter = Serial1.readStringUntil('\n');
        if(content_cmd == "END"){
          break;
        }
        loop_content[i] = content_cmd + ":" + content_parameter;
      }
      int acctual_amount = i;
      // run loop content
      for(int i = 0; i < iterations; i++) {
        for(int j = 0; j < acctual_amount; j++)
          // TODO: split cmd part and paramter part and give to switch case
          // SWITCH CASE: case cmd: cmd-function(paramter)
      }

    

    //If command equals "Press:X" or "Press:X+Y+ETC"
    else if(cmd == "Press"){
      press(cmd_parameter);
    }
  
    //If command equals "Print:X"
    else if(cmd == "Print") {
      printCmd(cmd_parameter);
    }

    //If command equals "PrintLine:X"
    else if(cmd == "PrintLine") {
      printLineCmd(cmd_parameter);
    }

//mouse up
    //If command equals "MouseMoveUp:X"
    else if(cmd == "MouseMoveUp") {
      mouseUpCmd(cmd_parameter);
    }

//mouse down
    //If command equals "MouseMoveDown:X"
    else if(cmd == "MouseMoveDown") {
      mouseDownCmd(cmd_parameter);
    }

//mouse left
    //If command equals "MouseMoveLeft:X"
    else if(cmd == "MouseMoveLeft") {
      mouseLeftCmd(cmd_parameter);
    }

//mouse right
    //If command equals "MouseMoveRight:X"
    else if(cmd == "MouseMoveRight") {
      mouseRightCmd(cmd_parameter);
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
