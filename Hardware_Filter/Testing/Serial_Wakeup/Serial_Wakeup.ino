#include <avr/sleep.h>
#include <SoftSerial.h>
//#include <avr/delay.h>

// Definitions for writing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define LED PB1

SoftSerial HC12(0,2); // RX, TX

int whistle_status;
//bool cleared_buffer;

void setup(){
    pinMode(LED, OUTPUT);
    HC12.begin(9600);
    system_sleep();
    digitalWrite(LED, HIGH);
}

void loop(){

  if (HC12.available() > 0){
      whistle_status = HC12.read();
  }
  if 
    
}

// Function for putting ATTiny85 to sleep
void system_sleep() {
  cbi(ADCSRA,ADEN);                    // Switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System sleeps here

  sleep_disable();                     // System continues execution here when ISR is triggered
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
  clear_buffer();
}

void clear_buffer(){
  while(HC12.available()){
  HC12.read();
  }
}

// Note: System wakes up when ISR is triggered (so when whistle is blown)
