 /*Programme d'une horlage pour Ramadhan sur Arduino UNO
Ecrit par Areour Mohamed Cherif
Date : 12/05/2018
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
#include "DS1307.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define Sd_Chip_pin 4

#include <TMRpcm.h>// playing the melody library

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define BUFFPIXEL 20

// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

DS1307 clock; //define a object of DS1307 class

TMRpcm tmrpcm; // Create a melody function

unsigned int i=0,j=0,k=0,l=0,m=0,n=0;

void desplayDigit();

void setup() {

  tft.begin();
  
  clock.begin();
  clock.fillByYMD(2018,6,12);
  clock.fillByHMS(20,10,30);
  clock.setTime();//write time to the RTC chip

  tmrpcm.speakerPin = 9;      // 9 pin to attach digital speakers + leg
  tmrpcm.quality(1);          // Set 1 for 2x oversampling
  tmrpcm.setVolume(6);        // 0 to 7. Set volume level

  SD.begin(Sd_Chip_pin); 
  
  tft.setRotation(1);
  
  background();
  delay(500);
  }

void loop() {
  desplayDigit();  
  
  if ((clock.hour == 20)&& (clock.minute == 11)&&(clock.second == 00)){
   
    bmpDraw("ftour.bmp", 0, 0);
    tmrpcm.play("Makkah.wav");

    while(tmrpcm.isPlaying())
    {
    }
    tmrpcm.disable();
    background();
    i=0;j=0;k=0;l=0;m=0;n=0;
    desplayDigit();
       
  }
  delay(500);
}

void desplayDigit(){
  
  tft.setTextColor(ILI9341_RED,ILI9341_WHITE);
  tft.setTextSize(5);
  clock.getTime();
  
  if (k != clock.hour){
    tft.setCursor(45, 90);
    if (clock.hour >= 0 && clock.hour < 10) {
          tft.print('0');
        }
        tft.print(clock.hour);
        tft.print(":");
        k=clock.hour;    
  }
  
  if (j != clock.minute){
        tft.setCursor(125, 90);
        if (clock.minute >= 0 && clock.minute < 10) {
          tft.print('0');
        }
        tft.print(clock.minute);
        tft.print(":");
        j=clock.minute;
    } 
  if( i != clock.second){
          tft.setCursor(210, 90);
          if (clock.second >= 0 && clock.second < 10) {
            tft.print('0');
          }
          tft.print(clock.second);
          i=clock.second;
        }  

tft.setTextSize(3);
   if (l != clock.dayOfMonth){
        tft.setCursor(65, 150);
        if (clock.dayOfMonth >= 0 && clock.dayOfMonth < 10) {
          tft.print('0');
        }
        tft.print(clock.dayOfMonth);
        tft.print("/");
        l=clock.dayOfMonth;    
  }
  
  if (m != clock.month){
        tft.setCursor(120, 150);
        if (clock.month >= 0 && clock.month < 10) {
          tft.print('0');
        }
        tft.print(clock.month);
        tft.print("/");
        m=clock.month;
    } 
  if( n != (clock.year+2000)){
          tft.setCursor(175, 150);
          tft.print(clock.year+2000);
          n=clock.year+2000;
        }  
} 

void background(){
  tft.fillScreen(ILI9341_WHITE);
  tft.setRotation(1);
  bmpDraw("OHA.bmp", 0, 0);
}

void bmpDraw(char *filename, int16_t x, int16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col, x2, y2, bx1, by1;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        x2 = x + bmpWidth  - 1; // Lower-right corner
        y2 = y + bmpHeight - 1;
        if((x2 >= 0) && (y2 >= 0)) { // On screen?
          w = bmpWidth; // Width/height of section to load/display
          h = bmpHeight;
          bx1 = by1 = 0; // UL coordinate in BMP file
          if(x < 0) { // Clip left
            bx1 = -x;
            x   = 0;
            w   = x2 + 1;
          }
          if(y < 0) { // Clip top
            by1 = -y;
            y   = 0;
            h   = y2 + 1;
          }
          if(x2 >= tft.width())  w = tft.width()  - x; // Clip right
          if(y2 >= tft.height()) h = tft.height() - y; // Clip bottom
  
          // Set TFT address window to clipped image bounds
          tft.startWrite(); // Requires start/end transaction now
          tft.setAddrWindow(x, y, w, h);
  
          for (row=0; row<h; row++) { // For each scanline...
  
            // Seek to start of scan line.  It might seem labor-
            // intensive to be doing this on every line, but this
            // method covers a lot of gritty details like cropping
            // and scanline padding.  Also, the seek only takes
            // place if the file position actually needs to change
            // (avoids a lot of cluster math in SD library).
            if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
              pos = bmpImageoffset + (bmpHeight - 1 - (row + by1)) * rowSize;
            else     // Bitmap is stored top-to-bottom
              pos = bmpImageoffset + (row + by1) * rowSize;
            pos += bx1 * 3; // Factor in starting column (bx1)
            if(bmpFile.position() != pos) { // Need seek?
              tft.endWrite(); // End TFT transaction
              bmpFile.seek(pos);
              buffidx = sizeof(sdbuffer); // Force buffer reload
              tft.startWrite(); // Start new TFT transaction
            }
            for (col=0; col<w; col++) { // For each pixel...
              // Time to read more pixel data?
              if (buffidx >= sizeof(sdbuffer)) { // Indeed
                tft.endWrite(); // End TFT transaction
                bmpFile.read(sdbuffer, sizeof(sdbuffer));
                buffidx = 0; // Set index to beginning
                tft.startWrite(); // Start new TFT transaction
              }
              // Convert pixel from BMP to TFT format, push to display
              b = sdbuffer[buffidx++];
              g = sdbuffer[buffidx++];
              r = sdbuffer[buffidx++];
              tft.writePixel(tft.color565(r,g,b));
            } // end pixel
          } // end scanline
          tft.endWrite(); // End last TFT transaction
        } // end onscreen
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
