#ifndef LST_DRIVERS_DAC_H_
#define LST_DRIVERS_DAC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <lib.h>

namespace lst {

const uint8_t kNumDacChannels = 3;

class Dac {
    public:
        Dac() { }
        ~Dac() { }
    
        void Init();
        inline void Set(uint8_t channel, uint8_t value) {
            value_[channel] = value;
        }

        void Write();
        void Write(uint8_t channel);
        inline uint8_t Get() {
            return value_[0];
        }
  
    private:
        uint8_t value_[kNumDacChannels];
        DISALLOW_COPY_AND_ASSIGN(Dac);
};

}  // namespace lst

#endif  // LST_DRIVERS_DAC_H_
