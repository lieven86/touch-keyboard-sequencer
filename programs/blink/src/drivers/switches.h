#ifndef LST_DRIVERS_SWITCHES_H_
#define LST_DRIVERS_SWITCHES_H_

namespace lst {

const uint8_t kNumSwitches = 8;

class Switches {
 public:
  Switches() { }
  ~Switches() { }
  
  void Init();
  void Debounce();
  
  inline bool released(uint8_t index) const {
    return switch_state_[index] == 0x7f;
  }
  
  inline bool just_pressed(uint8_t index) const {
    return switch_state_[index] == 0x80;
  }

  inline bool pressed(uint8_t index) const {
    return switch_state_[index] == 0x00;
  }
  
 private:
  uint8_t switch_state_[kNumSwitches];
  
  //DISALLOW_COPY_AND_ASSIGN(Switches);
};

}  // namespace lst

#endif  // LST_DRIVERS_SWITCHES_H_
