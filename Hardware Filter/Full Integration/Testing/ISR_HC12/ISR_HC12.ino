// Requires headers for AVR defines and ISR function
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <SendOnlySoftwareSerial.h>

#define INTERRUPT_PIN PCINT0  // This is PB1 per the schematic
#define INT_PIN PB0           // Interrupt pin of choice: PB1 (same as PCINT1) - Pin 6
#define LED_PIN PB1          // PB4 - Pin 3
#define PCINT_VECTOR PCINT0_vect      // This step is not necessary - it's a naming thing for clarity

SendOnlySoftwareSerial HC12(2); // Pin to RX of HC12

volatile bool whistle_status = 0;
volatile bool sent = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  HC12.begin(9600);
  cli();                            // Disable interrupts during setup
  PCMSK |= (1 << INTERRUPT_PIN);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
  pinMode(INT_PIN, INPUT_PULLUP);   // Set our interrupt pin as input with a pullup to keep it stable
  sei();                            //last line of setup - enable interrupts after setup
}
// This is the interrupt handler called when there is any change on the INT_PIN
// ISR is defined in the headers - the ATtiny85 only has one handler

void loop(){
  if (whistle_status == 1 && sent == 0){
    digitalWrite(LED_PIN, HIGH);
    HC12.println(1);
    sent = 1;
  }
  else if (whistle_status == 0 && sent == 1){
    digitalWrite(LED_PIN, LOW);
    HC12.println(0);
    sent = 0;
    // put ATTiny85 to sleep
  }
}


ISR(PCINT_VECTOR)
{
  if (whistle_status == 0){
    whistle_status = 1;
    //HC12.println("Whistle is on!");
  }
  else{
    //HC12.println("Whistle is off!");
    whistle_status = 0;
  }
  //delayMicroseconds(500);
}
