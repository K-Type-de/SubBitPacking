#ifndef KT_SUBBITPACKEDSTRUCT_H
#define KT_SUBBITPACKEDSTRUCT_H

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
  using Super = PackedStruct<Ns...>;
  using uint_packed_t = internal::UintPacked<Super::kBitsUsed>;

  uint_packed_t data_ = {0};

  template <typename... States>
  void set(std::size_t index, PackedState state, States... other_states)
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
  SubBitPackedStruct() = default;
  SubBitPackedStruct(const SubBitPackedStruct &other) : data_{other.data_} {}

  template <typename... State>
  SubBitPackedStruct(State... states)
  {
    this->checkStateBoundries(sizeof...(states) ? sizeof...(states) - 1 : 0);
    this->initialValueCalculation(0, states...);
  }

  static constexpr uint8_t GetBitsUsed()
  {
    return Super::kBitsUsed;
  }

  PackedState get(std::size_t index) const
  {
    this->checkStateBoundries(index);

    return SubBitPackedData::Get(this->data_, Super::kStatePowers[index],
                                 Super::kNumStates[index]);
  }

  void set(std::size_t index, PackedState state)
  {
    this->checkStateBoundries(index);

    SubBitPackedData::Set(this->data_, Super::kStatePowers[index], Super::kNumStates[index],
                          state);
  }

  std::size_t getNumberOfValues() const
  {
    return Super::kNumFields;
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

#endif  // KT_SUBBITPACKEDSTRUCT_H
