 /*Programme d'une horlage pour Ramadhan sur Arduino UNO
Ecrit par Areour Mohamed Cherif
Date : 23/05/2018
E-mail : openhardwaredz@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Fonts/FreeSansBold12pt7b.h"
#include "Fonts/FreeSansBold24pt7b.h"
#include "DS1307.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

#define PIEZO 3 // pin 3 is capable of PWM

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

DS1307 clock; //define a object of DS1307 class

int i=0,j=0,k=0,l=0,m=0,n=0;

void desplayDigit();

void setup() {

  pinMode(PIEZO, OUTPUT);

  tft.begin();
  
  clock.begin();
  clock.fillByYMD(2018,5,23);
  clock.fillByHMS(19,57,30);
  //clock.fillDayOfWeek();
  clock.setTime();//write time to the RTC chip
    
  tft.setRotation(1);
  background();
  delay(500);
  }

void loop() {
  desplayDigit();  
  
  if ((clock.hour == 19)&& (clock.minute == 58)&&(clock.second == 00)){
    background();
    
    tft.setFont(&FreeSansBold24pt7b);
    tft.setTextColor(ILI9341_RED);
    
    for (int i=0;i < 10; i++ ){
      tft.setCursor(100, 100);
      tft.print ("Saha");
      tft.setCursor(55, 200);
      tft.print ("ftourkom");
      analogWrite(PIEZO, 128); // 50 percent duty cycle tone to the piezo
      delay(1000);
      tft.fillRect(45,50,220,190,ILI9341_WHITE);
      digitalWrite(PIEZO, LOW); // turn the piezo off
      delay(1000);
      
    }
    
    background();
    i=0;j=0;k=0;l=0;m=0;n=0;
    desplayDigit();
  }
  delay(500);
}

void desplayDigit(){
  tft.setFont(&FreeSansBold24pt7b);
  tft.setTextColor(ILI9341_RED);
  clock.getTime();
  
  if (k != clock.hour){
    tft.fillRect(45,55,60,40,ILI9341_WHITE);
        tft.setCursor(45, 90);
        if (clock.hour >= 0 && clock.hour < 10) {
          tft.print('0');
        }
        tft.print(clock.hour);
        tft.print(":");
        k=clock.hour;    
  }
  
  if (j != clock.minute){
        tft.fillRect(125,55,60,40,ILI9341_WHITE);
        tft.setCursor(125, 90);
        if (clock.minute >= 0 && clock.minute < 10) {
          tft.print('0');
        }
        tft.print(clock.minute);
        tft.print(":");
        j=clock.minute;
    } 
  if( i != clock.second){
          tft.fillRect(210,55,90,40,ILI9341_WHITE);
          tft.setCursor(210, 90);
          if (clock.second >= 0 && clock.second < 10) {
            tft.print('0');
          }
          tft.print(clock.second);
          i=clock.second;
        }  

   if (l != clock.dayOfMonth){
    tft.fillRect(25,100,60,40,ILI9341_WHITE);
        tft.setCursor(25, 130);
        if (clock.dayOfMonth >= 0 && clock.dayOfMonth < 10) {
          tft.print('0');
        }
        tft.print(clock.dayOfMonth);
        tft.print("/");
        l=clock.dayOfMonth;    
  }
  
  if (m != clock.month){
        tft.fillRect(100,100,60,40,ILI9341_WHITE);
        tft.setCursor(100, 130);
        if (clock.month >= 0 && clock.month < 10) {
          tft.print('0');
        }
        tft.print(clock.month);
        tft.print("/");
        m=clock.month;
    } 
  if( n != (clock.year+2000)){
          tft.fillRect(175,100,120,40,ILI9341_WHITE);
          tft.setCursor(175, 130);
          tft.print(clock.year+2000);
          n=clock.year+2000;
        }  
} 

void background(){
  tft.fillScreen(ILI9341_WHITE);
  tft.setRotation(1);
  
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(ILI9341_BLUE);
  tft.setCursor(25,30);
  tft.print("OHA Ramadhan Clock");
}
