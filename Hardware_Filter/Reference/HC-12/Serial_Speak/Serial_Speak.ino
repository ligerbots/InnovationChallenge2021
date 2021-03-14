/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/

#include <SoftwareSerial.h>

SoftwareSerial HC12(8, 9); // HC-12 TX Pin, HC-12 RX Pin

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
}

void loop() {
  while (HC12.available()) {        // If HC-12 has data
    Serial.println(HC12.read());      // Send the data to Serial monitor
    //Serial.end();
  }
  while (Serial.available()) {      // If Serial monitor has data
    HC12.println(Serial.read(), DEC);      // Send that data to HC-12
    //HC12.end();
  }
}
