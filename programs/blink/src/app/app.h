#ifndef LST_APP_H
#define LST_APP_H

#include <lib.h>
#include <resources.h>
#include <drivers/adc.h>
#include <drivers/dac.h>
#include <drivers/mcp4921.h>
#include <ui.h>
#include <app/binaryCounter.h>

namespace lst {

#define TRIGA_pin 4
#define TRIGA_init DDRD &= ~(1<<TRIGA_pin); PORTD |= (1<<TRIGA_pin)
#define TRIGA (PIND & (1<<TRIGA_pin))

const uint8_t kNumGateTracks = 8;
const uint8_t kNumCvs = 3;

class App {
    public:
        App() { }
        ~App() { }

        void Init(Ui *ui, Adc *adc, Dac *dac);
        void Process();

        uint8_t getBinaryCounter() { return binary_counter_.getCounter(); }

    private:
        Ui *ui_;
        Adc *adc_;
        Dac *dac_;

        BinaryCounter binary_counter_;

        int16_t delayCount;
        uint16_t dacCount;
        bool value;

        DISALLOW_COPY_AND_ASSIGN(App);
};
}

#endif // LST_APP_H
