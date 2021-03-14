// Programmed by Kevin Yang for FRC 2877 Ligerbots
// Personal website: http://bykevinyang.com/
// 2/16/2021


// Requires headers for AVR defines and ISR function
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h> 
#include <avr/sleep.h>
#include <SendOnlySoftwareSerial.h> 
// We need the ISR of the ATTiny85 for the whistle interrupt
// therefore we can't use RX in software serial (since it takes up the one ISR we have)

// Definitions for writing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// Pin declarations
#define INTERRUPT_PIN PCINT0        // This is PB1 per the schematic
#define INT_PIN PB0                 // Interrupt pin of choice: PB1 (same as PCINT1) - Pin 6
#define LED_PIN PB1                 // PB4 - Pin 3
#define PCINT_VECTOR PCINT0_vect    // This step is not necessary - it's a naming thing for clarity

// Setup software serial (TX only!)
SendOnlySoftwareSerial HC12(2);     // I/O pin 2 of ATTiny85 Pin to RX of HC12

// Vars for whistle and sent 
volatile bool whistle_status;
volatile bool sent;

// Note: System wakes up when ISR is triggered (so when whistle is blown)
void setup() {
  pinMode(LED_PIN, OUTPUT);         // For LED_BUILTIN on digispark

  HC12.begin(9600);                 // Being HC12 Serial

  cli();                            // Disable interrupts during setup
  PCMSK |= (1 << INTERRUPT_PIN);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
  pinMode(INT_PIN, INPUT_PULLUP);   // Set our interrupt pin as input with a pullup to keep it stable
  sei();                            // last line of setup - enable interrupts after setup

  system_sleep();                   // Put ATTiny85 to sleep
}

void loop(){

  if (whistle_status == 1 && sent == 0){
    digitalWrite(LED_PIN, HIGH);
    //wakeup_pulse();
    HC12.print(1, HEX);
    delay(10);
    HC12.print(1, HEX);
    delay(10);
    HC12.print(1, HEX);
    delay(10);
    HC12.print(1, HEX);
    delay(10);
    
    sent = 1;
  }
  
  else if (whistle_status == 0 && sent == 1){
    digitalWrite(LED_PIN, LOW);
    HC12.print(0, HEX);
    delay(10);
    HC12.print(0, HEX);
    delay(10);
    HC12.print(0, HEX);
    delay(10);
    HC12.print(0, HEX);
    delay(10);

    sent = 0;
    system_sleep();
  }

}

// Interupt Service Routine for whistle
// Triggers on both rising and falling (so any state change)
ISR(PCINT_VECTOR)
{
  if (whistle_status == 0){
    whistle_status = 1;
  }

  else{
    whistle_status = 0;
  }
}

// Function for putting ATTiny85 to sleep
void system_sleep() {
  whistle_status = 0;
  cbi(ADCSRA,ADEN);                    // Switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System sleeps here

  sleep_disable();                     // System continues execution here when ISR is triggered
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}

void wakeup_pulse(){
  HC12.print(255, BIN);
  HC12.flush();
}
