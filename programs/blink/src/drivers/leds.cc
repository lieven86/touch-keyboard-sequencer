//#include <Arduino.h>
#include <avr/io.h>
#include <drivers/leds.h>

namespace lst {
  
using namespace std;
  
//const uint16_t led1 = PINB1;
//const uint16_t led2 = PINB0;

void Leds::Init() {
  	DDRB |= 1 << PINB2 | 1 << PINB1; // set pin 1 2 as outputs
	//PORTB |= 1 << PINB1;
	counter_ = 0;
	animation_counter_ = 0;
	resetAnimationValue();
}

void Leds::Write() {
	// todo: writefunction: check value high or low to determine shift unshift port
	// if (leds_[0] == LOW) {
	// 	PORTB &= ~PINB1;
	// } else {
	// 	PORTB |= PINB1;
	// }
}

uint8_t Leds::animate() {
	if (counter_ == 300) {
		counter_ = 0;
		animation_value_ &= ~(1 << (animation_counter_));
		if (animation_counter_ >= 8) {
			animation_counter_ = 0;
			resetAnimationValue();
		} else {
			animation_counter_++;
		}
    }
    counter_++;
	return animation_value_;
}

}  // namespace lst
