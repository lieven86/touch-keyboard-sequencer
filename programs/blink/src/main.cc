#include <avr/io.h>
#include <avr/interrupt.h>

#include <drivers/system_clock.h>
#include <drivers/adc.h>
#include <drivers/dac.h>
#include <ui.h>
#include <shift_register.h>
#include <app/app.h>

using namespace lst;

Dac dac;
Adc adc;
Ui ui;
ShiftRegister shiftregister;
App app_;

void Init() {
  cli();
  system_clock.Init();
  dac.Init();
  adc.Init();
  ui.Init(&adc, &app_);
  app_.Init(&ui, &adc, &dac);
  shiftregister.Init();
  sei();
}

ISR (TIMER0_OVF_vect, ISR_NOBLOCK) {
  // timer for ui
  system_clock.Tick();
  ui.Poll();

  cli();
  // get values from touch plates
  shiftregister.CloseLatchIn();
  shiftregister.OpenLatchIn();
  adc.setShiftIn(1, shiftregister.ShiftIn());
  adc.setShiftIn(0, shiftregister.ShiftIn());
  
  // update display and leds
  shiftregister.CloseLatch();
  shiftregister.Update(app_.getBinaryCounter()); // gate outputs
  shiftregister.Update(ui.getLedSymbol()); // leds
  shiftregister.Update(ui.getSymbol()); // 7 segment display
  shiftregister.OpenLatch();
  sei();
}

ISR (TIMER2_COMPA_vect) { 
  TIFR2 = (1 << OCF2B);
  // ADC enable, ADC start, manual trigger mode, ADC interrupt enable, prescaler = 128 
  ADCSRA = 0b11001111; 
}

ISR (ADC_vect, ISR_NOBLOCK) {
  adc.Read();
}

ISR (TIMER1_COMPA_vect, ISR_BLOCK) {
  app_.Process();
}

int main(void) {
  Init();

  while(1) {
    // ...
  }
}

