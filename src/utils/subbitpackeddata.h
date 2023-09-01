#ifndef KT_SUBBITPACKEDDATA_H
#define KT_SUBBITPACKEDDATA_H

#include <cstdint>

#include "packedstate.h"

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
  template <typename T>
  static inline PackedState Get(const T &data, uint32_t divisor, uint16_t modulo)
  {
    return data / divisor % modulo;
  }

  template <typename T>
  static inline void Set(T &data, uint32_t power, uint16_t modulo, PackedState state)
  {
    PackedState current_state = Get(data, power, modulo);
    data = power * (state - current_state) + data;
  }
};

}  // namespace kt

#endif  // KT_SUBBITPACKEDDATA_H
