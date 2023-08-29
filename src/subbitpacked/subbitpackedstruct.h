#ifndef _KT_SUBBITPACKEDSTRUCT_H_
#define _KT_SUBBITPACKEDSTRUCT_H_

#include <cstdint>

#include "../base/subbitpackedstructbase.h"
#include "../utils/subbitpackeddata.h"
#include "../utils/uintpacked.h"


namespace kt
{
template <uint16_t... Ns>
class SubBitPackedStruct : protected SubBitPackedStructBase<Ns...>
{
  typedef SubBitPackedStructBase<Ns...> Super;
  typedef Internal::uintPacked<Super::kBitsUsed> uint_packed_t;

  uint_packed_t data_;

public:
  SubBitPackedStruct() : data_{0} {};
  SubBitPackedStruct(uint_packed_t data) : data_{data} {};

  static constexpr uint8_t GetBitsUsed()
  {
    return Super::kBitsUsed;
  }

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

  uint8_t getDataSize()
  {
    return sizeof(this->data_) * 8;
  }

  const uint_packed_t &getData() const
  {
    return this->data_;
  }
};

}  // namespace kt

#endif  // _KT_SUBBITPACKEDSTRUCT_H_
