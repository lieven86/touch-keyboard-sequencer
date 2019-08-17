#ifndef LST_SHIFTREGISTER_H_
#define LST_SHIFTREGISTER_H_

#include <avr/io.h>
#include <util/delay.h>
#include <lib.h>

namespace lst {

class ShiftRegister {
 public:
  ShiftRegister() { }
  ~ShiftRegister() { }
  
  void Init() {
    // PIND7 = ST_CP = latchpin
    // PIND5 = DS = datapin
    // PIND6 = CH_CP = clockpin
    DDRD |= (1 << PIND7) | (1 << PIND5) | (1 << PIND6);
    PORTD &= ~(1 << PIND5);
    PORTD &= ~(1 << PIND6);
    PORTD &= ~(1 << PIND7);

    // PINC1 = data
    // PINC2 = latch
    DDRC |= (1 << PINC2);
    DDRC &= ~(1 << PINC1);
    PORTC &= ~(1 << PINC2);
  }

  inline void OpenLatch() {
    PORTD |= (1 << PIND7);
  }

  inline void CloseLatch() {
    PORTD &= ~(1 << PIND7);
  }

  inline void OpenLatchIn() {
    PORTC |= (1 << PINC2);
  }

  inline void CloseLatchIn() {
    PORTC &= ~(1 << PINC2);
    _delay_us(5);
  }

  inline void Pulse() {
    PORTD &= ~(1 << PIND6);
    _delay_us(5);
    PORTD |= (1 << PIND6);
    
  }

  inline void Update(uint8_t v) {
    uint8_t j;
    uint8_t value = v;
    for (j = 0; j < 8; j++) {
        //PORTD |= (1 << PIND6);
        // if (((value >> j) & 1) == 0x0) {
        //   //PORTD &= ~(1 << PIND5);
        //   PORTD |= (1 << PIND5);
        // } else {
        //   //PORTD |= (1 << PIND5);
        //   PORTD &= ~(1 << PIND5);
        // }
        //PORTD &= ~(1 << PIND6);

        if (value & 0b10000000) {
          PORTD &= ~(1 << PIND5);
        } else {
          PORTD |= (1 << PIND5);
        }
        Pulse();
        value = value << 1;
    }
  }

  inline uint8_t ShiftIn(void) {
    uint8_t byte = 0;
    uint8_t pin_value;
    
    for (int i = 0; i < 8; i++) {
        pin_value = (PINC & (1 << PINC1)) >> PINC1;  
        byte |= (pin_value << ((8 - 1) - i));
        
        /* Pulse clock to write next bit. */
        Pulse();
    }
    return byte;
}

 private:
    DISALLOW_COPY_AND_ASSIGN(ShiftRegister);
};

}  // namespace lst

#endif  // LST_SHIFTREGISTER_H_
