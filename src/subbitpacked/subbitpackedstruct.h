#ifndef _KT_SUBBITPACKEDSTRUCT_H_
#define _KT_SUBBITPACKEDSTRUCT_H_

#include <cstdint>

#include "../base/packedstruct.h"
#include "../utils/subbitpackeddata.h"
#include "../utils/uintpacked.h"

namespace kt
{
template <uint16_t... Ns>
class SubBitPackedStruct : protected PackedStruct<Ns...>
{
  typedef PackedStruct<Ns...> Super;
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
    this->checkStateBoundries(index);

    return SubBitPackedData::Get(this->data_, Super::kStatePowers[index],
                                 Super::kNumStates[index]);
  }

  void set(size_t index, uint16_t state)
  {
    this->checkStateBoundries(index);

    SubBitPackedData::Set(this->data_, Super::kStatePowers[index], Super::kNumStates[index],
                          state);
  }

  std::size_t getDataSize() const
  {
    return sizeof(this->data_) * 8;
  }

  const uint_packed_t& getData() const
  {
    return this->data_;
  }
};

}  // namespace kt

#endif  // _KT_SUBBITPACKEDSTRUCT_H_
