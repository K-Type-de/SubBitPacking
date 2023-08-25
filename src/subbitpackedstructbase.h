#ifndef _KT_SUBBITPACKESTRUCTBASE_H_
#define _KT_SUBBITPACKESTRUCTBASE_H_

#include <cstdint>

#include "utils/compiletime.h"

namespace kt
{

template <uint16_t... Ns>
class SubBitPackedStructBase
{
protected:
  SubBitPackedStructBase() {}

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
constexpr std::size_t SubBitPackedStructBase<Ns...>::kNumFields;

template <uint16_t... Ns>
constexpr std::array<uint32_t, SubBitPackedStructBase<Ns...>::kNumFields>
    SubBitPackedStructBase<Ns...>::kNumStates;

template <uint16_t... Ns>
constexpr std::array<uint32_t, SubBitPackedStructBase<Ns...>::kNumFields>
    SubBitPackedStructBase<Ns...>::kStatePowers;

template <uint16_t... Ns>
constexpr uint8_t SubBitPackedStructBase<Ns...>::kBitsUsed;

}  // namespace kt

#endif  // _KT_SUBBITPACKESTRUCTBASE_H_
