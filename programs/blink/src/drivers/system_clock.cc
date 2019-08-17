#include <avr/io.h>
#include <drivers/system_clock.h>

namespace lst {

void SystemClock::Init() {
    // setup timer0
    //TCCR0B |= (1 << CS01) | (1 << CS10); // prescaler of 8
    TCCR0B |= (1 << CS10) | (1 << CS11); // prescaler of 64
    //TCCR0B |= (1 << CS12); // prescaler of 128
    TIMSK0 |= (1 << TOIE0); // enable overflow interrupt
}

SystemClock system_clock;

}  // namespace lst
