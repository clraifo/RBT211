/*
In this version, the two external LEDs alternate blinking in the default state. When the ISR is triggered by the pushbutton,
they are turned off and the onboard LED blinks at the same rate. The blink rate of the LEDs can be adjusted by changing 
the value of the Output Compare Register A (OCR1A) for Timer1. 

To increase the blink rate (make the LEDs blink more frequently), decrease the value of OCR1A. 
To decrease the blink rate (make the LEDs blink less frequently), increase the value of OCR1A.

The actual blink rate can be calculated using the following formula:

Blink rate (in seconds) = (OCR1A + 1) * Prescaler / F_CPU (current settings = (15624 + 1) * 1024 / 16,000,000 ≈ 1 second)

- This version uses Timer1 interrupt to alternate between the two external LEDs
- The button press will still be handled with an external interrupt
- When pressed, it will stop the external LEDs from blinking and turn them ON
- Additionally, it will start the on-board LED (PB5) to start blinking
- Once the button is released, the two external LEDs will resume their alternating pattern and the on-board LED will stop blinking

This version maintains a global button_flag variable to keep track of whether the button is currently pressed. This 
flag determines which LED(s) to toggle in the Timer1 compare match ISR.

The INT0 ISR has been modified to check the button_flag. If the button was not previously pressed, it turns on both external LEDs 
and sets the flag. If the button was previously pressed, it turns off both external LEDs and the on-board LED, and clears the flag.

Connect external LEDs to PD6 and PD7
Connect button input to PD2
*/

#ifndef F_CPU				    //Checks whether the clock is defined
#define F_CPU 16000000UL	    //Sets it to 16mHz
#endif                          //Ends the if loop

#include <avr/io.h>			    //Enables AVR I/O
#include <avr/interrupt.h>		//Enables use of interrrupts

#define LED0 PD6                //Assigns LED0 to Pin PD6
#define LED1 PD7                //Assigns LED0 to Pin PD7
#define ONBOARD_LED PB5         //Assigns the onboard LED to PB5 (this is not really necessary)
#define BUTTON PD2              //Assigns te pushbutton to Pin PD2

volatile uint8_t button_flag = 0;    //declares a volatile variable as an unisgned 8 bit integer, names it button_flag and 
                                     //initializes it to 0

int main(void)
{
    DDRD |= (1 << LED0) | (1 << LED1);     //sets LED0/PD6 and LED1/PD7 to output
    DDRB |= (1 << ONBOARD_LED);            //sets ONBOARD_LED/PB5 to output
    DDRD &= ~(1 << BUTTON);                //sets BUTTON/PD2 to a pullup resistor
    PORTD |= (1 << BUTTON);                

    TCCR1A = 0;             //Sets the Timer/Counter Control Register A to 0, disables all features controlled by TCCR1A, leaves 
                            //it in a simple counting mode. TCCR1A is in 15.11.1 in the datasheet.
    
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);  // Sets specific bits in the Timer/Counter Control Register B to control 
                                                        // the operation of Timer1. This sets bits for Clock Select, which  control 
                                                        // the prescaler for Timer1. By setting CS12 and CS10 (and leaving CS11 
                                                        // unset), it sets a prescaler of 1024. TCCR1B is in 15.11.2 in the datasheet.
                                                        
    TIMSK1 = (1 << OCIE1A);         // Sets Output Compare Interrupt Enable 1 A bit (OCIE1A) in the Timer/Counter Interrupt Mask (TIMSK) Register 
                                    // This enables the Output Compare A Match interrupt for Timer1, which is triggered whenever the timer 
                                    // value matches the value stored in OCR1A (below).  TIMSK! is in 15.11.8 in the datasheet.
    
    OCR1A = 15624;                  //Output Compare Register A (OCR1A) for Timer1 to 15624, which takes one second to hit at 16mHz
                                    //OCR1A is in 1701104 in the datasheet.

    EICRA = (1 << ISC01) | (1 << ISC00);     // Sets the ISC01 and ISC00 bits of the External Interrupt Control Register A to 1.
                                             // This defines the trigger condition for INT0 (below) to its rising edge (LOW to HIGH)
                                             //EICRA is in 12.2.1 in the datasheet.
    
    EIMSK = (1 << INT0);             // Sets  EIMSK register INT0 to 1, which enbales external interrupts and triggers EICRA (above)
                                     // when the defined condition (rising edge, defined in AICRA above) is met. EIMSK is in 12.2.2 in the datasheet.

    sei();
    
    while (1)                        // Just an empty, endless loop. Kind of like life sometimes.
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
