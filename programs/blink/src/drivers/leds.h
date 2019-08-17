#ifndef LST_DRIVERS_LEDS_H_
#define LST_DRIVERS_LEDS_H_

#include <lib.h>

namespace lst {

const uint8_t kNumLeds = 2;

class Leds {
 public:
  Leds() { }
  ~Leds() { }
  
  void Init();
  
  void set(uint8_t channel, bool value) {
    leds_[channel] = value;
  }

  void Write();

  uint8_t animate();
  void resetAnimationValue() { animation_value_ = 0b11111111; }

 private:
  bool leds_[kNumLeds];
  uint8_t animation_counter_;
  uint16_t counter_;
  uint8_t animation_value_;
  DISALLOW_COPY_AND_ASSIGN(Leds);
};

}  // namespace lst

#endif  // LST_DRIVERS_LEDS_H_
