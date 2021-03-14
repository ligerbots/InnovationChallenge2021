#include <SendOnlySoftwareSerial.h>

SendOnlySoftwareSerial HC12(2);  // Tx pin

void setup()
{
  HC12.begin(9600);
}

void loop()
{
  HC12.print("Hello World!");
  delay (1000);
}
