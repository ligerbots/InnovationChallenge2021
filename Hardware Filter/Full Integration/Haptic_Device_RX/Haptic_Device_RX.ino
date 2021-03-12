// Programmed by Kevin Yang for FRC 2877 Ligerbots
// Personal website: http://bykevinyang.com/
// 2/16/2021

#include <SoftSerial.h>
#include <avr/sleep.h>

#define motorPin 1

// Definitions for writing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

volatile bool whistle_status;
const unsigned long WAIT_TIME = 5000;
int radio_info;

volatile bool motor_status;

SoftSerial HC12(0, 2); // HC-12 TX Pin, HC-12 RX Pin

void setup() {
    pinMode(motorPin, OUTPUT);

    HC12.begin(9600);               // Serial port to HC12
}

unsigned long lastSleep;

void loop() {

  while (HC12.available () > 0){
        radio_info = HC12.read();

        if (radio_info == 49){
            whistle_status = 1;
      }
        else{
            whistle_status = 0;
      }
  }

  if (whistle_status == 1 && motor_status == 0){
      digitalWrite(motorPin, HIGH);
      motor_status = 1;
  }
  else if (whistle_status == 0 && motor_status == 1){
      digitalWrite(motorPin, LOW);
      motor_status = 0;
  }

  if (millis () - lastSleep >= WAIT_TIME)
  {
    lastSleep = millis ();
    system_sleep();
  }
    
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
