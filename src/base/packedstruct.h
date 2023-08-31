#ifndef _KT_SUBBITPACKEDSTRUCTBASE_H_
#define _KT_SUBBITPACKEDSTRUCTBASE_H_

#ifdef KT_ENABLE_EXCEPTIONS
#include <stdexcept>
#endif  // KT_ENABLE_EXCEPTIONS

#include <cstdint>

#include "utils/compiletime.h"

namespace kt
{

template <uint16_t... Ns>
class PackedStruct
{
protected:
  PackedStruct() {}

  inline void checkStateBoundries(std::size_t state_value_index) const
  {
#ifdef KT_ENABLE_EXCEPTIONS
    if (state_value_index >= kNumFields)
    {
      throw std::out_of_range{"[PackedStruct] State value index out of range"};
    }
#endif  // KT_ENABLE_EXCEPTIONS
  }

public:
  static constexpr std::size_t kNumFields = sizeof...(Ns);
  static constexpr std::array<uint32_t, kNumFields> kNumStates = {Ns...};
  static constexpr std::array<uint32_t, kNumFields> kStatePowers =
      CompileTime::GeneratePowLut<kNumFields>(CompileTime::VariadicStatePow<Ns...>);
  static constexpr uint8_t kBitsUsed = 64 - CompileTime::NumberOfUnusedUpperBits<uint64_t>(
                                                CompileTime::HighestVariadicValue<Ns...>());
  static_assert(kBitsUsed <= 32, "[SubBitPackedStruct] Number of states exceed 32-bit limit");
};

// C++11 linker madness
template <uint16_t... Ns>
constexpr std::size_t PackedStruct<Ns...>::kNumFields;

template <uint16_t... Ns>
constexpr std::array<uint32_t, PackedStruct<Ns...>::kNumFields> PackedStruct<Ns...>::kNumStates;

template <uint16_t... Ns>
constexpr std::array<uint32_t, PackedStruct<Ns...>::kNumFields> PackedStruct<Ns...>::kStatePowers;

template <uint16_t... Ns>
constexpr uint8_t PackedStruct<Ns...>::kBitsUsed;

}  // namespace kt

#endif  // _KT_SUBBITPACKEDSTRUCTBASE_H_
