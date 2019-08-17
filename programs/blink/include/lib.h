#ifndef LIB_LIB_H_
#define LIB_LIB_H_

#include <inttypes.h>
#include <stddef.h>

#ifndef NULL
#define NULL 0
#endif

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define CLIP(x) if (x < -32767) x = -32767; if (x > 32767) x = 32767;

#define CONSTRAIN(var, min, max) \
  if (var < (min)) { \
    var = (min); \
  } else if (var > (max)) { \
    var = (max); \
  }

#define abs(x) ((x)>0?(x):-(x)) 

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

namespace lib {
// 28 - 12 = 16
// inline int16_t Interpolate1228(const int16_t* table, uint32_t phase) {
//   int32_t a = table[phase >> 28];
//   int32_t b = table[(phase >> 28) + 1];
//   return a + ((b - a) * static_cast<int32_t>((phase >> 12) & 0xffff) >> 16);
// }

inline int16_t Interpolate824(const uint8_t* table, uint32_t phase)
  __attribute__((always_inline));

inline int16_t Interpolate824(const uint8_t* table, uint32_t phase) {
  int32_t a = table[phase >> 24];
  int32_t b = table[(phase >> 24) + 1];
  return (a << 8) + \
      ((b - a) * static_cast<int32_t>(phase & 0xffffff) >> 16) - 32768;
}

}

#endif
