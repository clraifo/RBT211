/*
- This version uses Timer1 interrupt to alternate between the two external LEDs
- The button press will still be handled with an external interrupt. 
- When pressed, it will stop the external LEDs from blinking and turn them ON. 
- Additionally, it will start the on-board LED (PB5) to start blinking.
- Once the button is released, the two external LEDs will resume their alternating pattern and the on-board LED will stop blinking.

This version maintains a global button_flag variable to keep track of whether the button is currently pressed. This 
flag determines which LED(s) to toggle in the Timer1 compare match ISR.

The INT0 ISR has been modified to check the button_flag. If the button was not previously pressed, it turns on both external LEDs 
and sets the flag. If the button was previously pressed, it turns off both external LEDs and the on-board LED, and clears the flag.

Connect external LEDs to PD6 and PD7
Connect button input to PD2
*/

//Always include the F_CPU to reduce errors
#ifndef F_CPU				
#define F_CPU 16000000UL	
#endif

#include <avr/io.h>			
#include <avr/interrupt.h>		

#define LED0 PD6
#define LED1 PD7
#define ONBOARD_LED PB5
#define BUTTON PD2 

volatile uint8_t button_flag = 0;

int main(void)
{
    DDRD |= (1 << LED0) | (1 << LED1); 
    DDRB |= (1 << ONBOARD_LED); 
    DDRD &= ~(1 << BUTTON); 
    PORTD |= (1 << BUTTON); 

    TCCR1A = 0; 
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TIMSK1 = (1 << OCIE1A);
    OCR1A = 15624;

    EICRA = (1 << ISC01) | (1 << ISC00); 
    EIMSK = (1 << INT0);  

    sei();
    
    while (1)
    {
    }

    return(0);
}

ISR(TIMER1_COMPA_vect) {
    if (button_flag == 0) {
        PORTD ^= (1 << LED0) | (1 << LED1); 
    } else {
        PORTB ^= (1 << ONBOARD_LED);
    }
}

ISR(INT0_vect){
    if (button_flag == 0){
        PORTD |= (1 << LED0) | (1 << LED1);
        button_flag = 1;
    } else {
        PORTD &= ~((1 << LED0) | (1 << LED1));
        PORTB &= ~(1 << ONBOARD_LED);
        button_flag = 0;
    }
}
