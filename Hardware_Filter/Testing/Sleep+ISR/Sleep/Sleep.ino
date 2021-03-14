// Requires headers for AVR defines and ISR function
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <avr/sleep.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define INTERRUPT_PIN PCINT0  // This is PB1 per the schematic
#define INT_PIN PB0           // Interrupt pin of choice: PB1 (same as PCINT1) - Pin 6
#define LED_PIN PB1          // PB4 - Pin 3
#define PCINT_VECTOR PCINT0_vect      // This step is not necessary - it's a naming thing for clarity

volatile bool whistle_status = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  cli();                            // Disable interrupts during setup
  PCMSK |= (1 << INTERRUPT_PIN);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
  pinMode(INT_PIN, INPUT_PULLUP);   // Set our interrupt pin as input with a pullup to keep it stable
  sei();                            //last line of setup - enable interrupts after setup
  system_sleep();
}
// This is the interrupt handler called when there is any change on the INT_PIN
// ISR is defined in the headers - the ATtiny85 only has one handler

void loop(){
  if (whistle_status == 1){
    digitalWrite(LED_PIN, HIGH);
  }
  else{
    digitalWrite(LED_PIN, LOW);
    system_sleep();
  }
}

// set system into the sleep state 
// system wakes up when wtchdog is timed out
void system_sleep() {
  whistle_status == 0;
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System sleeps here

  sleep_disable();                     // System continues execution here when ISR is triggered
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}


ISR(PCINT_VECTOR)
{
  if (whistle_status == 0){
    whistle_status = 1;
  }
  else{
    whistle_status = 0;
  }
  //delayMicroseconds(500);
}
