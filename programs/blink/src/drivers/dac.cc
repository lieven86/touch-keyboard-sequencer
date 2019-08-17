#include <drivers/dac.h>

namespace lst {
  
void Dac::Init() {
    // PORTC pin 3, 4, 5 as digital output
    //PORTC |= (1 << PINC3) | (1 << PINC4) | (1 << PINC5);
    PORTC = 0b001111100;
    DDRC |= (1 << PINC3) | (1 << PINC4) | (1 << PINC5);
}

void Dac::Write() {
    if (value_[0] > 127) {
        PORTC |= (1 << PINC3);
    } else {
        PORTC &= ~(1 << PINC3);
    }
    if (value_[1] > 127) {
        PORTC |= (1 << PINC4);
    } else {
        PORTC &= ~(1 << PINC4);
    }
    if (value_[2] > 127) {
        PORTC |= (1 << PINC5);
    } else {
        PORTC &= ~(1 << PINC5);
    }
}

void Dac::Write(uint8_t channel) {
    switch (channel) {
        case 0:
            if (value_[0] > 127) {
                PORTC |= (1 << PINC3);
            } else {
                PORTC &= ~(1 << PINC3);
            }
            break;
        case 1:
            if (value_[1] > 127) {
                PORTC |= (1 << PINC4);
            } else {
                PORTC &= ~(1 << PINC4);
            }
            break;
        case 2:
            if (value_[2] > 127) {
                PORTC |= (1 << PINC5);
            } else {
                PORTC &= ~(1 << PINC5);
            }
            break;
    }
}

}  // namespace lst
