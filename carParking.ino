/*
  Hello World.ino
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-9-18

  Grove - Serial LCD RGB Backlight demo.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Wire.h>
#include <Servo.h>
#include "rgb_lcd.h"

#define IR1      4         //Pin connected to IR sensor 1
#define IR2      7         //Pin connected to IR sensor 2
#define IR3      8         //Pin connected to IR sensor 3
#define IR4      9         //Pin connected to IR sensor 4
#define IR5      5         //Pin connected to IR sensor 5
#define ServoM   2        //Pin connected to Servo Motor

rgb_lcd lcd;
Servo myservo;  // create servo object to control a servo

const int colorR = 0;
const int colorG = 255;
const int colorB = 0;
int pos = 80;
//int gateOpened = 0;
int pState = 0;
int cState = 0;
int total = 3;

void setup() 
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);
    pinMode(IR1, INPUT);
    pinMode(IR2, INPUT);
    pinMode(IR3, INPUT);
    pinMode(IR4, INPUT);
    pinMode(IR5, INPUT);
    // Print a message to the LCD.
    myservo.attach(ServoM);
    delay(1000);
    myservo.write(81);
}

void loop() 
{
  int in_IR1 = digitalRead(IR1);
  int in_IR2 = digitalRead(IR2);
  int in_IR3 = digitalRead(IR3);
  int in_IR4 = digitalRead(IR4);
  int in_IR5 = digitalRead(IR5);
  lcd.setCursor(0,1);
  lcd.print(total);
  if ((cState == 0) && in_IR1 && total>0){ //Check whether car entering from outside ; might have to check the condition again
    cState = 4;
    pState = 0;
    gateOpen();
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("4,0");
    //delay(4000);
  }else if ((cState == 4) && (pState ==0)){     //Check whether slots are available
    cState = 6;
    pState = 4;
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("6,4");    
  }else if ((cState == 6) && (pState == 4) && !in_IR1){
    cState = 2;
    pState = 6;
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("2,6");
  }else if ((cState == 2) && (pState == 6) && in_IR2){
    cState = 3;
    pState = 2;
    gateClose();
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("3,2");
    //delay(4000);
  }else if ((cState == 3) && (pState == 2)){
    cState = 1;
    pState = 3;
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("1,3");    
  }else if ((cState == 1) && (pState == 3) && !in_IR2){
    total=total-1;
    cState = 0;
    pState = 1;
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("0,1");
  }else if ((cState == 0) && in_IR2){  // Car going out
    cState = 1;
    pState = 0;
    gateOpen();
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("1,0");
    //delay(4000);
  }else if ((cState == 1) && (pState == 0)){     //Check whether slots are available
    cState = 3;
    pState = 1;
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("3,1");    
  }else if ((cState == 3) && (pState == 1) && !in_IR2){
    cState = 2;
    pState = 3;
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("2,3");
  }else if ((cState == 2) && (pState == 3) && in_IR1){
    cState = 6;
    pState = 2;
    gateClose();
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("6,2");
    //delay(4000);
  }else if ((cState == 6) && (pState == 2)){
    cState = 4;
    pState = 6;
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("4,6");    
  }else if ((cState == 4) && (pState == 6) && !in_IR1){
    total=total+1;
    cState = 0;
    pState = 4;
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("0,4");
  }
  if(in_IR3)
  {
    lcd.setCursor(1,0);
    lcd.print("F"); 
  }
  else
  {
    lcd.setCursor(1,0);
    lcd.print("N");
  }
  
  if(in_IR4)
  {
    lcd.setCursor(4,0);
    lcd.print("F"); 
  }
  else
  {
    lcd.setCursor(4,0);
    lcd.print("N");
  }
  
  if(in_IR5)
  {
    lcd.setCursor(7,0);
    lcd.print("F"); 
  }
  else
  {
    lcd.setCursor(7,0);
    lcd.print("N");
  }
  if (total == 0){
    lcd.setRGB(255, 0, 0);
  }else{
    lcd.setRGB(0, 255, 0);
  }
}

void gateOpen(){
  for (pos = 80; pos < 163; pos += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  } 
}

void gateClose(){
  for (pos = 163; pos > 80; pos -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  } 
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
