#include <SPI.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>

void setup(){
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.csnPin = 8;
  Mirf.cePin = 7;
  Mirf.init();
  Mirf.setTADDR((byte *)"serv1");
  Mirf.payload = 32;
  Mirf.config();
  Serial.println("Beginning ...");
  
}

void loop(){
  byte data[32];

  if(!Mirf.isSending() && Mirf.dataReady()){
    int i;
    Serial.println("Got Packet");
    
    Mirf.getData((byte *) &data);
    for(i=0; i<sizeof(data); i++)
    {
      Serial.println(data[i]);
    }
  }
  Serial.println("No data");
  delay(500);
  
}
