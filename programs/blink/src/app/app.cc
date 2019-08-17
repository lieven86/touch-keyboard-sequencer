#include <app/app.h>

namespace lst {

const int16_t kUnsyncPotThreshold = INT16_MAX / 20;
const int16_t kResetThresholdLow = 100;
const int16_t kResetThresholdHigh = 200;

void App::Init(Ui *ui, Adc *adc, Dac *dac) {
    TCCR1B = (TCCR1B & ~(1 << WGM13)) | (1 << WGM12);
    TCCR1A = TCCR1A & ~((1 << WGM11) | (1 << WGM10));
    TCCR1B = (TCCR1B & ~((1 << CS12) | (1 << CS11))) | (1 << CS10); // No prescaler
    // Set the compare register (OCR1A) = F_CPU / SAMPLE_RATE; 
    // Enable interrupt when TCNT1 == OCR1A
    TIMSK1 |= (1 << OCIE1A);
    //OCR1A = 490; //40KHz Samplefreq
    OCR1A = 1960; 
    
    ui_ = ui;
    adc_ = adc; 
    dac_ = dac;

    binary_counter_.init();
    TRIGA_init;
    init_spi();

    delayCount = 0;
    dacCount = 0;
    value = false;
}



void App::Process() {
    if (ui_->mode() == UI_MODE_SPLASH) {
        return;
    }

    uint16_t cv1 = ui_->coarse(0) << 2;

    if (delayCount == 5000) {
        delayCount = 0;
        value = !value;
        binary_counter_.pulse();
    } else {
        delayCount++;
    }

    // set gate outputs
    dac_->Set(0, value ? UINT8_MAX : 0); 
    dac_->Set(1, value ? UINT8_MAX : 0);
    dac_->Set(2, value ? UINT8_MAX : 0);
    
    dac_->Write(0);
    dac_->Write(1);
    dac_->Write(2);

    // test mcp4921 dacs with a ramp
    output_dac(0, DAC_CS, UINT16_MAX - dacCount);
    output_dac(0, DAC_CS2, cv1);
    output_dac(0, DAC_CS3, dacCount);

    if (dacCount >= (4096)) {
        dacCount = 0;
    } else {
        dacCount++;
    }
}

}
