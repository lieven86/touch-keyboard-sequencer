#ifndef LST_BINARY_COUNTER_H_
#define LST_BINARY_COUNTER_H_

#include <lib.h>

namespace lst {

class BinaryCounter {
 public:
  BinaryCounter() { }
  ~BinaryCounter() { }

    void init() {
        reset();
    }
  
    void pulse() { count++; }
    void reset() { count = 0; }

    bool getStage(uint8_t s) {
        return count & (1 << s) ? true : false;
    }
    uint8_t getCounter() { return count; }

 private:
    uint8_t count;
    DISALLOW_COPY_AND_ASSIGN(BinaryCounter);
};

}  // namespace lst

#endif  // LST_BINARY_COUNTER_H_
