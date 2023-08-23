#ifndef _KT_SUBBITPACKEDDATA_H_
#define _KT_SUBBITPACKEDDATA_H_

#include <cstdint>

namespace kt
{
/**
 * Static class to get/set data in an arithmetically packed 32-bit data block
 */
class SubBitPackedData
{
private:
  SubBitPackedData() {}

public:
  static inline uint16_t Get(const uint32_t &data, uint32_t divisor, uint16_t modulo)
  {
    return data / divisor % modulo;
  }

  static inline void Set(uint32_t &data, uint32_t power, uint16_t modulo, uint16_t state)
  {
    uint16_t current_state = Get(data, power, modulo);
    data = data - (current_state * power) + (state * power);
  }
};

}  // namespace kt

#endif  //_KT_SUBBITPACKEDDATA_H_
