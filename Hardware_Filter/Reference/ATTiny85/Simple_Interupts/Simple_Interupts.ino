#include <avr/interrupt.h>
 
volatile int value=0;
 
void setup()
{
    GIMSK = 0b00100000;    // turns on pin change interrupts
    PCMSK = 0b00010011;    // turn on interrupts on pins PB0, PB1, &amp;amp; PB4
    sei();                 // enables interrupts
}
 
void loop()
{
}
 
ISR(PCINT0_vect)
{
    value = 1;             // Increment volatile variable
}