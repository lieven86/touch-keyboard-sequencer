#include <ui.h>
#include <math.h>
#include <app/app.h>
#include <resources.h>

namespace lst {

using namespace std;
using namespace lib;

const uint8_t kMaxOctaves = 5;
const int32_t kLongPressDuration = 500;
const int32_t kVeryLongPressDuration = 2000;
const int32_t kPotMoveThreshold = 3; 
const uint16_t kCatchupThreshold = 1 << 10;
//const uint8_t kNumLeds = 2;

void Ui::Init(Adc* adc, App* app) {
  adc_ = adc;
  app_ = app;
  leds_.Init();
  switches_.Init();
  display_.Init();

  feat_mode_ = KEYBOARD;
  mode_ = UI_MODE_SPLASH;

  animation_counter_ = 0;
  blink_counter_ = 0;
  ignore_releases_ = 0;
  display_state = 0;
  led_state = 0;
  
  switchState = false;
  for (uint8_t i = 0; i < 6; ++i) {
    pressed[i] = false;
    press_time_[i] = 0;
    detect_very_long_press_[i] = false;
  }
}

void Ui::Poll() {
  // switches_.Debounce();
  // check digital ins
  for (uint8_t i = 0; i < 2; i++) {
    //adc_->getShiftBit(0, i + 6);
  }
  // if button is pressed & check how long
  // 2nd row of touch plates
  for (uint8_t i = 0; i < 6; i++) {
    //switchState = PINB & (0x01<<0);
    switchState = adc_->getShiftBit(0, i);
    if (switchState && !pressed[i]) { // justPressed
      press_time_[i] = system_clock.milliseconds();
      detect_very_long_press_[i] = false;
      pressed[i] = true;
    }

    if (!switchState && press_time_[i]) {
      int32_t pressed_time = system_clock.milliseconds() - press_time_[i];
      
      if (!detect_very_long_press_[i]) {
        if (pressed_time > kLongPressDuration) { // longPressed
          OnSwitchReleased(i, pressed_time);
          detect_very_long_press_[i] = true;
        }
      } else {
        if (pressed_time > kVeryLongPressDuration) { // veryLongPressed
          OnSwitchReleased(i, pressed_time);
          detect_very_long_press_[i] = false;
          press_time_[i] = 0;
          pressed[i] = false;
        }
      }
    }
    //if ((switchState == 0x01) && press_time_[i] != 0 && !detect_very_long_press_[i]) { 
    if ((!switchState) && press_time_[i] != 0 && !detect_very_long_press_[i]) { // pressRelease
      OnSwitchReleased(i, system_clock.milliseconds() - press_time_[i] + 1);
      press_time_[i] = 0;
      detect_very_long_press_[i] = false;
      pressed[i] = false;
    }
  }

  // convert first row of touch plates into gates
  for (int8_t i = 0; i < 8; i++) {
    switchState = adc_->getShiftBit(1, i);
    if (switchState && !pressed_plate_[i]) { // justPressed
      press_time_plate_[i] = system_clock.milliseconds();
      detect_very_long_press_plate_[i] = false;
      pressed_plate_[i] = true;
    }

    if (!switchState && press_time_plate_[i]) {
      int32_t pressed_time = system_clock.milliseconds() - press_time_plate_[i];
      if (!detect_very_long_press_plate_[i]) {
        if (pressed_time > kLongPressDuration) { // longPressed
          //OnSwitchReleased(i, pressed_time);
          detect_very_long_press_plate_[i] = true;
          pressed_plate_[i] = false;
        }
      } else {
        if (pressed_time > kVeryLongPressDuration) { // veryLongPressed
          //OnSwitchReleased(i, pressed_time);
          detect_very_long_press_plate_[i] = false;
          press_time_plate_[i] = 0;
          //pressed_plate_[i] = false;
        }
      }
    }
    //if ((switchState == 0x01) && press_time_plate_[i] != 0 && !detect_very_long_press_plate_[i]) { 
    if ((!switchState) && press_time_plate_[i] != 0 && !detect_very_long_press_plate_[i]) { // pressRelease
      OnPressureReleased(i, system_clock.milliseconds() - press_time_plate_[i] + 1);
      press_time_plate_[i] = 0;
      detect_very_long_press_plate_[i] = false;
      pressed_plate_[i] = false;
    }
  }

  if (mode_ == UI_MODE_SPLASH) {
    if (animation_counter_ == 2560) {
        animation_counter_ = 0;
      	mode_ = UI_MODE_NORMAL;
        display_.Set(ANIMATION, 0, MAIN, 10);
    }
    animation_counter_++;
  } else {
    led_state = app_->getBinaryCounter();
    // filter the pot values and emit events when changed
    for (uint8_t i = 0; i < 8; ++i) {
      uint16_t adc_value = adc_->value_multi_1(i);
      int32_t value = (31 * pot_filtered_value_[i] + adc_value) >> 5;
      pot_filtered_value_[i] = value;
      int32_t current_value = static_cast<int32_t>(pot_value_[i]);
      if ((value >= current_value + kPotMoveThreshold) || (value <= current_value - kPotMoveThreshold)) {
        //if (abs(value - pot_coarse_value_[i]) < kCatchupThreshold) {
          pot_coarse_value_[i] = value;
          display_.Set(POPUP, value >> 6, MAIN, lastDisplayValue);
          //catchup_state_[i] = false;
        // }
        pot_value_[i] = value;
      }
    }
  }

  display_.Tick();
}

void Ui::OnSwitchPressed() {
  //
}

void Ui::OnPressureReleased(uint8_t id, int32_t t) {
  if (t > kVeryLongPressDuration) {
    //
  } else if (t > kLongPressDuration) {
    //
  } else {
    switch (mode_) {
      case UI_MODE_SPLASH:
        break;
      case UI_MODE_NORMAL: {
        display_.Set(NONE, 0, MAIN, id);
        lastDisplayValue = id;
      } break;
    }
  }
}

void Ui::OnSwitchReleased(uint8_t id, int32_t t) {
  if (t > kVeryLongPressDuration) {
    //
  } else if (t > kLongPressDuration) {  
    //
  } else {
    switch (mode_) {
      case UI_MODE_SPLASH:
        break;
      case UI_MODE_NORMAL: {
        switch (id) {
          case SWITCH_GATE: {
            display_.Set(NONE, 0, MAIN, 13);
            lastDisplayValue = 13;
          } break;
          case SWITCH_ARP: {
            display_.Set(NONE, 0, MAIN, 12);
            lastDisplayValue = 12;
          } break;
          case SWITCH_CLOCK: {
            display_.Set(NONE, 0, MAIN, 11);
            lastDisplayValue = 11;
          } break;
          case SWITCH_FUNC_1: {
            display_.Set(NONE, 0, MAIN, 14);
            lastDisplayValue = 14;
          } break;
          case SWITCH_VOICE: {
            display_.Set(NONE, 0, MAIN, 10);
            lastDisplayValue = 10;
          } break;
          case SWITCH_FUNC_2: {
            display_.Set(NONE, 0, MAIN, 15);
            lastDisplayValue = 15;
          } break;
        }
      } break;
    }
  }
}

void Ui::OnPotChanged() {

}


uint8_t Ui::getLedSymbol() {
  // paint leds
  switch (mode_) {
    case UI_MODE_SPLASH:
      return leds_.animate(); 
    
    case UI_MODE_NORMAL:
      return (0b11111111 & ~(led_state));

  }
  return 0;
}

uint8_t Ui::getSymbol() { 
  uint8_t page_states[2] = {0b00000000, 0b10000000};
  uint8_t mode_mask = page_states[0];
  uint8_t mask = display_.getState() == MAIN ? mode_mask : 0b00000000;
  display_state = display_.GetSymbol(); 
  return (display_state | mask); 
}

}  // namespace lst
