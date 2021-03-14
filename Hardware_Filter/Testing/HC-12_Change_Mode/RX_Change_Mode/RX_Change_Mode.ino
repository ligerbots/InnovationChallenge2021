#include <SoftwareSerial.h>

SoftwareSerial HC12(8, 9); // HC-12 TX Pin, HC-12 RX Pin
#define setPin 7

byte incomingByte;
String readBuffer = "";

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(setPin, OUTPUT);
    digitalWrite(setPin, LOW); // Enter Transparent mode

    Serial.begin(1200);             // Serial port to computer
    HC12.begin(1200);               // Serial port to HC12
    //setRadio();
    defaultSettings();
    //setRadio();
    //delay(100);
}

void loop() {
  while (HC12.available()) {        // If HC-12 has data
    Serial.write(HC12.read());      // Send the data to Serial monitor
  }
  while (Serial.available()) {      // If Serial monitor has data
    HC12.write(Serial.read());      // Send that data to HC-12
  }
}

void setRadio(){
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(setPin, HIGH);
    delay(10);
    HC12.write("AT+B4800");         // Set Baud to 1,200bps
    delay(10);
    HC12.write("AT+FU1");            // Enter FU1 mode
    delay(10);

    Serial.end();
    HC12.end();
    Serial.begin(4800);             // Serial port to computer
    HC12.begin(4800);               // Serial port to HC12

    //while(Serial.available()) Serial.read(); // Remove any bits stuck in reg
    //while(HC12.available()) HC12.read();
    digitalWrite(setPin, LOW);      // Exit AT mode
    digitalWrite(LED_BUILTIN, LOW);
}

void radioSleep(){
    digitalWrite(setPin, HIGH);      // Enter AT command mode
    HC12.write("AT+SLEEP");         // Enter sleep mode
    digitalWrite(setPin, LOW);
}

void radioWake(){
    digitalWrite(setPin, HIGH);      // Entering AT command mode wakes module up
    delay(10);
    digitalWrite(setPin, LOW);
}

void defaultSettings(){
    digitalWrite(setPin, HIGH);
    delay(10);
    HC12.write("AT+DEFAULT");        // Enter default mode
    delay(10);
    digitalWrite(setPin, LOW);
}
