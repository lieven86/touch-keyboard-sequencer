#ifndef LST_DRIVERS_ADC_H_
#define LST_DRIVERS_ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <lib.h>

namespace lst {

const uint8_t kNumMuxChannels = 8;

enum AdcChannel {
  ADC_MULTIPLEX_1 = 0,
  ADC_CHANNEL_LAST
};

class Adc {
 public:
  Adc() { }
  ~Adc() { }
  
  void Init();
  void Read();
  inline uint16_t value(uint8_t channel) const { return values_[channel]; } // 10 bit value
  inline uint8_t value8(uint8_t channel) const { return values_[channel] >> 2; } // 8 bit value
  inline float float_value(uint8_t channel) const {
    return static_cast<float>(values_[channel] >> 2) / 256.0f;
  }
  inline const uint16_t* values() { return &values_[0]; }
  inline uint16_t value_multi_1(uint8_t channel) const { return values_multi_1_[channel]; } // 10 bit value
  void setShiftIn(uint8_t channel, uint8_t si) {
      shift_in_[channel] = si;
  }
  bool getShiftBit(uint8_t channel, uint8_t b) {
    bool out = (shift_in_[channel] & (1 << (b))) ? true : false;
    return out;
  }
 private:
  uint16_t values_[ADC_CHANNEL_LAST];
  uint16_t values_multi_1_[kNumMuxChannels];
  uint8_t current_multiplex_channel;
  uint8_t current_adc_channel;
  uint8_t divider;
  uint8_t shift_in_[2];
  DISALLOW_COPY_AND_ASSIGN(Adc);
};

}  // namespace lst

#endif  // LST_DRIVERS_ADC_H_
