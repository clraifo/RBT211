#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Pulse values defined as per the datasheet (page 102)
#define PULSE_MIN 999U
#define PULSE_MAX 4799U
#define PULSE_MID 2899U

// Top value to achieve 50Hz frequency with 16MHz/8 = 2MHz timer clock
#define TOP_VALUE 39999

// Loop increment/decrement step
#define STEP 20

// Delay between each servo position update
#define UPDATE_DELAY_MS 10

// Delay between servo moves
#define MOVE_DELAY_MS 500

int main(void)
{
    ICR1 = TOP_VALUE;
    
    // Set timer/counter mode to fast PWM, with TOP defined by ICR1
    TCCR1A |= (1 << WGM11);
    TCCR1A |= (1 << COM1A1); // Set OC1A (PB1) as output compare pin
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); // Prescaler of 8

    DDRB |= (1 << PB1);

    OCR1A = PULSE_MIN;

    while (1) 
    {
        // Move servo from minimum to maximum position
        for (uint16_t i = PULSE_MIN; i < PULSE_MAX; i += STEP)
        {
            OCR1A = i;
            _delay_ms(UPDATE_DELAY_MS);
        }
        
        // Move servo from maximum to minimum position
        for (uint16_t i = PULSE_MAX; i > PULSE_MIN; i -= STEP)
        {
            OCR1A = i;
            _delay_ms(UPDATE_DELAY_MS);
        }
        
        // Move servo to different positions with delay in between
        OCR1A = PULSE_MIN;
        _delay_ms(MOVE_DELAY_MS);
        OCR1A = PULSE_MID;
        _delay_ms(MOVE_DELAY_MS);
        OCR1A = PULSE_MAX;
        _delay_ms(MOVE_DELAY_MS);
        OCR1A = PULSE_MID;
        _delay_ms(MOVE_DELAY_MS);
    }
}
