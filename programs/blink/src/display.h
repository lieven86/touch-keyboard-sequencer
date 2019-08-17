#ifndef LST_DISPLAY_H_
#define LST_DISPLAY_H_

#include <lib.h>
#include <resources.h>

namespace lst {

enum DisplayStates {
  MAIN,
  TRANSITION,
  PARAMETER,
  INTRO,
  FLASH,
  STATE_LAST
};

enum DisplayActions {
  ANIMATION,
  POPUP,
  NONE,
  ACTION_LAST
};

class Display {
 public:
  Display() { }
  ~Display() { }
  
  void Init() {
    popupTime = 500;
    counter_ = 0;
    display_state_ = INTRO;
    canDisplayPopup = false;
    mainSymbol_ = 1;
    animationSymbol_ = 0;
    current_word_ = 0;
    letterIndex = 0;
    word_length = 5;

    words[0] = 10; // a
    words[1] = 11; // b
    words[2] = 12; // c
    words[3] = 13; // d
    words[4] = 14; // e
    words[5] = 15; // f
    words[6] = 16; // g
    words[7] = 17; // h
    words[8] = 18; // i
    words[9] = 29; // time
    words[10] = 24; // oct amount
    words[11] = 13; // distance
    words[12] = 28; // style
    words[13] = 28; // scale
    words[14] = 12; // chord type

  }
  
  void Tick() {
    if (canDisplayPopup) {
      counter_++;
      if (display_action_ == POPUP) {
        if (counter_ >= popupTime) {
          ResetCounter();
          canDisplayPopup = false;
          display_state_ = next_display_state_;
        }
      } else if (display_action_ == ANIMATION) {
        if (counter_ >= 150) {
          ResetCounter();
          letterIndex++;
          if (letterIndex >= word_length) {
            canDisplayPopup = false;
            letterIndex = 0;
            display_state_ = next_display_state_;
          } 
          if (letterIndex % 2 == 0) {
            animationSymbol_ = words[current_word_];
          } else {
            animationSymbol_ = 36;
          }
        }
      }
    }
  }

  uint8_t Flash() {
    uint8_t loopSequence[16] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    if (counter_ == 100) {
      animation_counter_++;
      counter_ = 0;
    }
    counter_++;
    uint8_t position = (animation_counter_ % 16);
    uint8_t out;
    if (loopSequence[position] == 1) {
      out = mainSymbol_;
    } else {
      out = 36;
    }
    return symbols[out];
  }
 
  uint8_t Bar() {
    if (counter_ == 100) {
      animation_counter_++;
      counter_ = 0;
    }
    counter_++;
    return bar[(animation_counter_ % 7)];
  }

  uint8_t Intro() {
    if (counter_ == 100) {
      animation_counter_++;
      counter_ = 0;
    }
    counter_++;
    return masks[(animation_counter_ % 6)];
  }

  uint8_t CircleAnimation() {
    if (counter_ == 50) {
      counter_ = 0;
      animation_counter_++;
    }
    counter_++;
    return circel_animation[animation_counter_ % 16];
  }

  void ResetCounter() {counter_ = 0; }
  void Set(DisplayActions symbol, uint8_t animation_value, DisplayStates state, uint8_t main_value) {
    mainSymbol_ = main_value;
    next_display_state_ = state;
    display_action_ = symbol;

    switch (symbol) {
      case ANIMATION: {
        current_word_ = animation_value;
        letterIndex = 0;
        ResetCounter();
        canDisplayPopup = true;
        animationSymbol_ = words[current_word_];
      } break;
      case POPUP: {
        ResetCounter();
        canDisplayPopup = true;
        animationSymbol_ = animation_value;
        display_state_ = next_display_state_;
      } break;
      case NONE: {
        display_state_ = next_display_state_;
      } break;
      case ACTION_LAST:
        break;
    }
  }

  void toggleGateView(bool m) {
    if (!m) {
      display_state_ = MAIN;
    } else {
      display_state_ = TRANSITION;
    }
  }

  DisplayStates getState() { return display_state_; }
  void setState(DisplayStates s) { display_state_ = s; }

  uint8_t GetSymbol() {
    uint8_t out;
    switch (display_state_) {
      case MAIN:
        if (canDisplayPopup) {
          out = symbols[animationSymbol_];
        } else {
          out = symbols[mainSymbol_];
        }
        break;
      case TRANSITION:
        out = mainSymbol_;
        break;
      case PARAMETER:
        if (canDisplayPopup) {
          out = symbols[animationSymbol_];
        } else if (current_word_ == 2 || current_word_ == 6) {
          out = mainSymbol_;
        } else {
          out = symbols[mainSymbol_];
        }
        break; 
      case FLASH:
        out = Flash();
        break;
      case INTRO:
        if (canDisplayPopup) {
          out = symbols[animationSymbol_];
        } else {
          out = CircleAnimation();
        }
        break;
      case STATE_LAST:
        break;
    }
    return out; 
  }

 private:
  DisplayStates display_state_, next_display_state_;
  DisplayActions display_action_;
  uint8_t mainSymbol_, animationSymbol_;
  uint16_t counter_;
  uint16_t animation_counter_;
  uint16_t popupTime;
  bool canDisplayPopup;
  uint8_t layout_view_;

  uint16_t words[15];
  uint8_t word_length;
  uint8_t current_word_;
  uint8_t letterIndex;
  DISALLOW_COPY_AND_ASSIGN(Display);
};

}  // namespace lst

#endif  // LST_DRIVERS_LEDS_H_
