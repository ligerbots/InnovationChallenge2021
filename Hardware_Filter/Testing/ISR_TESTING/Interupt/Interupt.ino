// Requires headers for AVR defines and ISR function
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <SoftSerial_INT0.h>

#define INTERRUPT_PIN PCINT0  // This is PB1 per the schematic
#define INT_PIN PB0           // Interrupt pin of choice: PB1 (same as PCINT1) - Pin 6
#define LED_PIN PB1          // PB4 - Pin 3
#define PCINT_VECTOR PCINT0_vect      // This step is not necessary - it's a naming thing for clarity

SoftSerial HC12(5, 2); // HC-12 TX Pin, HC-12 RX Pin

volatile bool whistle_status = 0;
volatile bool sent = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  cli();                            // Disable interrupts during setup
  PCMSK |= (1 << INTERRUPT_PIN);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
  pinMode(INT_PIN, INPUT_PULLUP);   // Set our interrupt pin as input with a pullup to keep it stable
  sei();                            //last line of setup - enable interrupts after setup
}
// This is the interrupt handler called when there is any change on the INT_PIN
// ISR is defined in the headers - the ATtiny85 only has one handler

void loop(){
  if (whistle_status == 1){
    digitalWrite(LED_PIN, HIGH);
    HC12.write(whistle_status);
    sent = 1;
  }
  
  else{
    digitalWrite(LED_PIN, LOW);
    if (sent == 1){
      HC12.write(whistle_status);
      sent = 0;
    }
  }
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
