/*  
 *  ------ Waspmote Pro Code Example -------- 
 *  
 *  Explanation: This is the basic Code for Waspmote Pro
 *  
 *  Copyright (C) 2013 Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify  
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation, either version 3 of the License, or  
 *  (at your option) any later version.  
 *   
 *  This program is distributed in the hope that it will be useful,  
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of  
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
 *  GNU General Public License for more details.  
 *   
 *  You should have received a copy of the GNU General Public License  
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
 */
     
// Put your libraries here (#include ...)
#define DATA_LENGTH 1024

char* filename="/tes0.txt";
byte data[DATA_LENGTH];

void setup() {
  // put your setup code here, to run once:
  USB.ON();
  SD.ON();
//  USB.println(SD.flag, DEC);
  if(SD.create(filename)) {
    USB.println("Create OK");
  } else {
    USB.println("Create failed");
//    USB.println(SD.flag, DEC);
  }
  int i = 1000;
  unsigned long times[128];
  for(int j = 0; j < DATA_LENGTH; ++j) {
    data[j] = random();
  }
  for(int i = 0; i < 12800; i++) {
    data[i % DATA_LENGTH] = random();
    unsigned long t1 = millis();
    if(SD.appendln(filename, data)) {
      unsigned long t2 = millis();
      times[i % 128] = t2 - t1;
      if(127 == i % 128) {
         for(int ii = 0; ii < 128; ii++) {
           USB.println(times[ii]);
         }
      }

    } else {
      USB.println("Fail");
    }
  }
}


void loop() {
    // put your main code here, to run repeatedly:
    
}

