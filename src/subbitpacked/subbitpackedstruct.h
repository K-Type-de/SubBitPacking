#ifndef _KT_SUBBITPACKEDSTRUCT_H_
#define _KT_SUBBITPACKEDSTRUCT_H_

#include <stdarg.h>

#include <cstdint>

#include "../base/packedstruct.h"
#include "../utils/packedstate.h"
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

  template <typename... States>
  void set(size_t index, PackedState state, States... other_states)
  {
    this->set(index, state);
    this->set(index + 1, other_states...);
  }

  inline void initialValueCalculation(std::size_t index, PackedState state)
  {
    this->data_ = this->data_ + state * Super::kStatePowers[index];
  }

  template <typename... State>
  void initialValueCalculation(std::size_t index, PackedState state, State... other_states)
  {
    this->initialValueCalculation(index, state);
    this->initialValueCalculation(index + 1, other_states...);
  }

public:
  SubBitPackedStruct() : data_{0} {}
  SubBitPackedStruct(const SubBitPackedStruct &other) : data_{other.data_} {}

  template <typename... State>
  SubBitPackedStruct(State... states) : data_{0}
  {
    this->checkStateBoundries(sizeof...(states) ? sizeof...(states) - 1 : 0);
    this->initialValueCalculation(0, states...);
  }

  static constexpr uint8_t GetBitsUsed()
  {
    return Super::kBitsUsed;
  }

  PackedState get(size_t index) const
  {
    this->checkStateBoundries(index);

    return SubBitPackedData::Get(this->data_, Super::kStatePowers[index],
                                 Super::kNumStates[index]);
  }

  void set(size_t index, PackedState state)
  {
    this->checkStateBoundries(index);

    SubBitPackedData::Set(this->data_, Super::kStatePowers[index], Super::kNumStates[index],
                          state);
  }

  std::size_t getDataSize() const
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
