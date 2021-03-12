/*    Arduino Long Range Wireless Communication using HC-12
        Example 02 - Changing channels using push buttons
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/

#include <SoftwareSerial.h>

#define setPin 6

SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

byte incomingByte;
String readBuffer = "";

void setup() {
  Serial.begin(9600);             // Open serial port to computer
  HC12.begin(9600);               // Open serial port to HC12
  pinMode(setPin, OUTPUT);
  digitalWrite(setPin, HIGH);     // HC-12 normal mode
}

void loop() {
  // ==== Storing the incoming data into a String variable
  while (HC12.available()) {             // If HC-12 has data
    incomingByte = HC12.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
  }
  delay(100);
  // ==== Sending data from one HC-12 to another via the Serial Monitor
  while (Serial.available()) {
    HC12.write(Serial.read());
  }
  // === If button 1 is pressed, set channel 01
  if (readBuffer == "AT+C001") {
    digitalWrite(setPin, LOW);           // Set HC-12 into AT Command mode
    delay(100);                          // Wait for the HC-12 to enter AT Command mode
    HC12.print(readBuffer);              // Send AT Command to HC-12 ("AT+C001")
    delay(200);
    while (HC12.available()) {           // If HC-12 has data (the AT Command response)
      Serial.write(HC12.read());         // Send the data to Serial monitor
    }
    Serial.println("Channel successfully changed");
    digitalWrite(setPin, HIGH);          // Exit AT Command mode
    readBuffer = "";
  }
  // === If button 2 is pressed, set channel 02
  if (readBuffer == "AT+C002") {
    digitalWrite(setPin, LOW);           // Set HC-12 into AT Command mode
    delay(100);                          // Wait for the HC-12 to enter AT Command mode
    HC12.print(readBuffer);              // Send AT Command to HC-12
    delay(200);
    while (HC12.available()) {           // If HC-12 has data (the AT Command response)
      Serial.write(HC12.read());         // Send the data to Serial monitor
    }
    Serial.println("Channel successfully changed");
    digitalWrite(setPin, HIGH);          // Exit AT Command mode

    readBuffer = "";
  }
  checkATCommand();
  readBuffer = "";                       // Clear readBuffer
}
// ==== Custom function - Check whether we have received an AT Command via the Serial Monitor
void checkATCommand () {
  if (readBuffer.startsWith("AT")) {     // Check whether the String starts with "AT"
    digitalWrite(setPin, LOW);           // Set HC-12 into AT Command mode
    delay(100);                          // Wait for the HC-12 to enter AT Command mode
    HC12.print(readBuffer);              // Send AT Command to HC-12
    delay(200);
    while (HC12.available()) {           // If HC-12 has data (the AT Command response)
      Serial.write(HC12.read());         // Send the data to Serial monitor
    }
    digitalWrite(setPin, HIGH);          // Exit AT Command mode
  }
}