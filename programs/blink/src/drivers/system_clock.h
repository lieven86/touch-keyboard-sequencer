#ifndef LST_SYSTEM_CLOCK_H_
#define LST_SYSTEM_CLOCK_H_

#include <lib.h>

namespace lst {

class SystemClock {
 public:
  SystemClock() { }
  ~SystemClock() { }
  
  void Init();
  inline void Tick() { ++count_; }
  inline volatile uint32_t milliseconds() const { return count_; }

 private:
  volatile uint32_t count_;

  DISALLOW_COPY_AND_ASSIGN(SystemClock);
};

extern SystemClock system_clock;

}  // namespace lst

#endif  // LST_SYSTEM_CLOCK_H_
