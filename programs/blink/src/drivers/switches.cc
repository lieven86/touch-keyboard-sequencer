#include <avr/io.h>
#include <drivers/switches.h>

namespace lst {
  
using namespace std;

void Switches::Init() {
    DDRB &= ~(1 << PINB0); // set b0 as intput, DDRB = 0b00000000;
    PORTB |= 1 << PINB0;
    for (int8_t i = 0; i < kNumSwitches; ++i) {
      switch_state_[i] = 0xff;
    }
}

void Switches::Debounce() {
  switch_state_[kNumSwitches - 1] = (switch_state_[kNumSwitches - 1] << 1) | \
      (PINB & (0x01<<0));
}

}  // namespace lst
