#include <drivers/adc.h>
#include <resources.h>
namespace lst {

void Adc::Init() {
    // multiplex select pins
    PORTD |= (1 << PIND2) | (1 << PIND1) | (1 << PIND0);
    DDRD |= (1 << PIND2) | (1 << PIND1) | (1 << PIND0);
    
    // Set up the ADC. We need to read the mains voltage frequently in order to get a sufficiently accurate RMS reading.
    // To avoid using too much CPU time, we use the conversion-complete interrupt. This means we can't use analogRead(),
    // we have to access the ADC ports directly.
    current_adc_channel = ADC_MULTIPLEX_1;      // set up which analog input we will read first
    ADMUX = 0b01000000 | current_adc_channel;
    ADCSRB = 0b00000000; // Analog Input bank 1
    ADCSRA = 0b10011111; // ADC enable, manual trigger mode, ADC interrupt enable, prescaler = 128
    
    // Set up a timer 2 interrupt every 1ms to kick off ADC conversions etc.
    // Do this last after we have initialized everything else
    ASSR = 0;
    TCCR2A = (1 << WGM21);    // CTC mode
    TCCR2B = (1 << CS22);     // prescaler = 64
    TCNT2 = 0;                // restart counter
    OCR2A = 249;              // compare register = 249, will be reached after 1ms
    //OCR2A = 128; 
    TIMSK2 = (1 << OCIE2A);

    shift_in_[0] = 0;
    shift_in_[1] = 0;
    divider = 0;
    current_multiplex_channel = 0;

    for (int8_t i = 0; i < 8; i++) {
        values_multi_1_[i] = 0;
    }
    // set multiplex input to read channel 0
    for (int j = 0; j < 3; j++) {
        uint8_t b = binary8[current_multiplex_channel][j];
        if (b == 0x0) {
            PORTD &= ~(1 << j);
        } else {
            PORTD |= (1 << j);
        }
    }
}

void Adc::Read() {
    uint8_t adcl = ADCL;
    uint8_t adch = ADCH;
    uint16_t adcVal = (adch << 8) | adcl;

    switch (current_adc_channel) {
        case ADC_MULTIPLEX_1:
            values_multi_1_[current_multiplex_channel] = adcVal;
            // read multiplexer
            current_multiplex_channel++;
            if (current_multiplex_channel == kNumMuxChannels) {
                current_multiplex_channel = 0;
                current_adc_channel = ADC_MULTIPLEX_1;
            }
  
            break;
        default:
            break;
    }

    for (int j = 0; j < 3; j++) {
        uint8_t b = binary8[current_multiplex_channel][j];
        if (b == 0x0) {
            PORTD &= ~(1 << j);
        } else {
            PORTD |= (1 << j);
        }
    }
    
    ADMUX = (0b01000000 | current_adc_channel);   // Vcc reference, select current channel
}
}  // namespace lst
