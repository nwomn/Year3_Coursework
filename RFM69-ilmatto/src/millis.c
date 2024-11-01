// Modified from: https://gist.github.com/adnbr/2439125#file-counting-millis-c

#include "millis.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

volatile uint32_t timer1_millis;

// Value needed to trigger match value every millisecond
#define CTC_MATCH_OVERFLOW (F_CPU/(1000*8))

void millis_init()
{
    // CTC mode, Clock/8
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);
    TCCR1B |= (1 << CS11);
    // Load count value into overflow register
    OCR1A = CTC_MATCH_OVERFLOW;

    
    // Enable the compare match interrupt
    TIMSK1 |= (1 << OCIE1A);
    timer1_millis = 0;
    sei();
}

uint32_t millis()
{
    uint32_t millis_return;
    // ensure this cannnot be disrupted
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        millis_return = timer1_millis;
    }
    return millis_return;
}

ISR (TIMER1_COMPA_vect)
{
    timer1_millis++;
}
