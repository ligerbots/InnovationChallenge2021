// From Instructables: https://www.instructables.com/ATTiny-Port-Manipulation/

#include <avr/io.h>
#define LED_PIN PB1
#define BUZZER_PIN PB3

void setup()
{  
 DDRB |= (1 << LED_PIN);        // Sets LED_PIN to output (changes reg to 1)
 DDRB |= (1 << BUZZER_PIN);     // Sets BUZZER_PIN to output (changes reg to 1)
}

void loop()
{ 
 delay(random(600000, 900000)); 
 byte state = random(0, 2); 
 switch(state)
  {  
   case 0:  
   PORTB |= (1 << LED_PIN);     // Sets LED_PIN to HIGH
   delay(20);  
   PORTB &= ~(1 << LED_PIN);    // Sets LED_PIN to LOW
   break;

   case 1:  
   PORTB |= (1 << BUZZER_PIN);  // Sets BUZZER_PIN to HIGH
   delay(20);  
   PORTB &= ~(1 << BUZZER_PIN);  // Sets BUZZER_PIN to LOW
   break; 
  }
}