// Need to install Mirf and SPI85 libs from https://github.com/stanleyseow/attiny-nRF24L01
// Change lines according to what is said in here: http://nerdralph.blogspot.com/2014/01/nrf24l01-control-with-3-attiny85-pins.html


#include <SPI85.h>
#include <Mirf85.h>
#include <MirfHardwareSpiDriver85.h>

void setup()
{
  Serial.begin(115200);

  Mirf.spi = &MirfHardwareSpi; // Note: Orginal code here was "MirfHardwareSpi85" and not "MirfHardwareSpi"
  // Was changed b/c of different lib used
  Mirf.init();
  Mirf.setTADDR((byte *)"serv1");
  Mirf.payload = 32;
  Mirf.config();
}

void loop()
{
  Mirf.send((byte *) "Hello");
  delay(500);
}
