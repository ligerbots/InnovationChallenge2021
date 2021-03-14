#include <avr/sleep.h>
#include <avr/power.h>
#include <SoftSerial.h>

const unsigned long WAIT_TIME = 5000;
const byte RX_PIN = 4;
const byte TX_PIN = 5;

SoftSerial HC12(RX_PIN, TX_PIN); // RX, TX

// Definitions for writing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup() 
{
  HC12.begin(9600);
} 

unsigned long lastSleep;

void loop() 
{
  if (millis () - lastSleep >= WAIT_TIME)
  {
    lastSleep = millis ();
    system_sleep();                         // Set to sleep
  }

  if (HC12.available() > 0)
  {
    byte flashes = HC12.read() - '0';
    if (flashes > 0 && flashes < 10)
      {
      // flash LED x times 
      for (byte i = 0; i < flashes; i++)
        {
        digitalWrite (GREEN_LED, HIGH);
        delay (200);  
        digitalWrite (GREEN_LED, LOW);
        delay (200);  
        }
      }        
  }  // end of if

}  // end of loop

// Function for putting ATTiny85 to sleep
void system_sleep() {
  whistle_status == 0;
  cbi(ADCSRA,ADEN);                    // Switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System sleeps here

  sleep_disable();                     // System continues execution here when ISR is triggered
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}