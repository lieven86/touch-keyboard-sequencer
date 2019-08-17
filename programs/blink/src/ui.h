#ifndef LST_UI_H_
#define LST_UI_H_

#include <avr/io.h>
#include <lib.h>
#include <drivers/system_clock.h>
#include <drivers/adc.h>
#include <drivers/leds.h>
#include <drivers/switches.h>
#include <display.h>
//#include <app/app.h>

namespace lst {

const uint8_t kFinePotDivider = 8;
const uint8_t kNumPots = 8;

class App;

enum FeatureMode {
  KEYBOARD = 0,
  FUNCTION_LAST
};

enum SwitchIndex {
  SWITCH_GATE = 0,
  SWITCH_ARP = 1,
  SWITCH_CLOCK = 2,
  SWITCH_FUNC_1 = 3,
  SWITCH_VOICE = 4,
  SWITCH_FUNC_2 = 5
};

enum DigitalIn {
  RESET_POSITION = 0,
  START_STOP = 1
};

enum UiMode {
  UI_MODE_SPLASH,
  UI_MODE_NORMAL
};

class Ui {
 public:
  Ui() { }
  ~Ui() { }
  
  void Init(Adc* adc, App* app);
  void Poll();
  void DoEvents();

  uint16_t coarse(uint8_t channel) {
    return pot_coarse_value_[channel];
  }

  inline FeatureMode feat_mode() const { return feat_mode_; }
  inline UiMode mode() const { return mode_; }
  inline bool pressed_plates(uint8_t position) const { return pressed_plate_[position]; }

  uint8_t getSymbol();
  uint8_t getLedSymbol();

 private:
  void OnSwitchPressed();
  void OnSwitchReleased(uint8_t id, int32_t t);
  void OnPotChanged();
  void OnPressureReleased(uint8_t id, int32_t t);

  uint16_t pot_value_[kNumMuxChannels];
  uint16_t pot_filtered_value_[kNumMuxChannels];
  uint16_t pot_coarse_value_[kNumMuxChannels];

  // 2nd row of plates and 2 buttons
  uint32_t press_time_[kNumSwitches - 2];
  bool detect_very_long_press_[kNumSwitches - 2];
  bool catchup_state_[kNumSwitches - 2];
  bool pressed[kNumSwitches - 2];

  // first row of plates
  uint32_t press_time_plate_[kNumSwitches];
  bool detect_very_long_press_plate_[kNumSwitches];
  bool catchup_state_plate_[kNumSwitches];
  bool pressed_plate_[kNumSwitches];

  int32_t animation_counter_;
  uint8_t blink_counter_;
  int8_t ignore_releases_;
  bool switchState;
  
  SwitchIndex lastPressed;
  uint16_t lastDisplayValue;

  Leds leds_;
  Switches switches_;
  Display display_;
  Adc *adc_;
  App* app_;

  UiMode mode_;
  FeatureMode feat_mode_, last_feature_mode;

  uint8_t display_state;
  uint8_t led_state;
  uint8_t padding[2];

  uint16_t version_token_;

  DISALLOW_COPY_AND_ASSIGN(Ui);
};

}  // namespace lst

#endif  // LST_UI_H_
