#include <SoftwareSerial.h>

/*
WIRING:
11: rx
10: tx
*/
const int radioRxPin = 11;
const int radioTxPin = 10;

SoftwareSerial HC12(radioTxPin, radioRxPin);
void setup() {
  Serial.begin(9600);
  HC12.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  while (HC12.available()) {
    uint8_t dat = HC12.read();

    Serial.println(dat);
    digitalWrite(LED_BUILTIN, dat);
  }
}
