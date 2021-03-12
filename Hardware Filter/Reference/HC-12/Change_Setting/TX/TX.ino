/*    Arduino Long Range Wireless Communication using HC-12
    Example 02 - Changing channels using push buttons - Buttons side
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/

#include <SoftwareSerial.h>

#define setPin 6
#define button1 4
#define button2 3

SoftwareSerial HC12(10, 11);         // HC-12 TX Pin, HC-12 RX Pin

byte incomingByte;
String readBuffer = "";

int button1State = 0;
int button1Pressed = 0;
int button2State = 0;
int button2Pressed = 0;

void setup() {
  Serial.begin(9600);                   // Open serial port to computer
  HC12.begin(9600);                     // Open serial port to HC12
  pinMode(setPin, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  digitalWrite(setPin, HIGH);           // HC-12 normal, transparent mode
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

  // ==== If button 1 is pressed, set the channel 01
  button1State = digitalRead(button1);
  if (button1State == HIGH & button1Pressed == LOW) {
    button1Pressed = HIGH;
    delay(20);
  }
  if (button1Pressed == HIGH) {
    HC12.print("AT+C001");               // Send the AT Command to the other module
    delay(100);
    //Set AT Command Mode
    digitalWrite(setPin, LOW);           // Set HC-12 into AT Command mode
    delay(100);                          // Wait for the HC-12 to enter AT Command mode
    HC12.print("AT+C001");               // Send AT Command to HC-12
    delay(200);
    while (HC12.available()) {           // If HC-12 has data (the AT Command response)
      Serial.write(HC12.read());         // Send the data to Serial monitor
    }
    Serial.println("Channel successfully changed");
    digitalWrite(setPin, HIGH);          // Exit AT Command mode
    button1Pressed = LOW;
  }
  
  // ====  If button 2 is pressed, set the channel 02
  button2State = digitalRead(button2);
  if (button2State == HIGH & button2Pressed == LOW) {
    button2Pressed = HIGH;
    delay(100);
  }
  if (button2Pressed == HIGH) {
    HC12.print("AT+C002"); // Send the AT Command to the other module
    delay(100);
    //Set AT Command Mode
    digitalWrite(setPin, LOW);           // Set HC-12 into AT Command mode
    delay(100);                          // Wait for the HC-12 to enter AT Command mode
    HC12.print("AT+C002");               // Send AT Command to HC-12
    delay(200);
    while (HC12.available()) {           // If HC-12 has data (the AT Command response)
      Serial.write(HC12.read());         // Send the data to Serial monitor
    }
    Serial.println("Channel successfully changed");
    digitalWrite(setPin, HIGH);
    button2Pressed = LOW;
  }
  checkATCommand();
  readBuffer = "";                       // Clear readBuffer
}
// ==== Custom function - Check whether we have received an AT Command via the Serial Monitor
void checkATCommand () {
  if (readBuffer.startsWith("AT")) {     // Check whether the String starts with "AT"
    digitalWrite(setPin, LOW);           // Set HC-12 into AT Command mode
    delay(200);                          // Wait for the HC-12 to enter AT Command mode
    HC12.print(readBuffer);              // Send AT Command to HC-12
    delay(200);
    while (HC12.available()) {           // If HC-12 has data (the AT Command response)
      Serial.write(HC12.read());         // Send the data to Serial monitor
    }
    digitalWrite(setPin, HIGH);          // Exit AT Command mode
  }
}