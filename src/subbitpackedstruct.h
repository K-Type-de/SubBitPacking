#ifndef _KT_SUBBITPACKESTRUCT_H_
#define _KT_SUBBITPACKESTRUCT_H_

#include <utils/uintpacked.h>

#include <cstdint>

#include "utils/compiletime.h"
#include "utils/subbitpackeddata.h"

namespace kt
{

template <uint16_t... Ns>
class SubBitPackedStructBase
{
protected:
  SubBitPackedStructBase() {}
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
template <uint16_t... Ns>
class SubBitPackedStruct : SubBitPackedStructBase<Ns...>
{
  typedef SubBitPackedStructBase<Ns...> Super;

  uintPacked<Super::kBitsUsed> data_;

public:
  SubBitPackedStruct() : data_{0} {};

  uint16_t get(size_t index) const
  {
    return SubBitPackedData::Get(this->data_, Super::kStatePowers[index],
                                 Super::kNumStates[index]);
  }

  void set(size_t index, uint16_t state)
  {
    SubBitPackedData::Set(this->data_, Super::kStatePowers[index], Super::kNumStates[index],
                          state);
  }

  uint8_t get_bits_used()
  {
    return Super::kBitsUsed;
  }

  uint8_t get_data_size()
  {
    return sizeof(this->data_) * 8;
  }
};

template <std::size_t num_values, uint16_t... Ns>
class SubBitPackedStructArray : SubBitPackedStructBase<Ns...>
{
  typedef SubBitPackedStructBase<Ns...> Super;
};

}  // namespace kt

#endif  // _KT_SUBBITPACKESTRUCT_H_
