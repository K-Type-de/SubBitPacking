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

  const uint_packed_t& getData() const
  {
    return this->data_;
  }

protected:
  class Field
  {
  public:
    constexpr Field(SubBitPackedStruct<Ns...>& parent)
        : parent_(parent), index_(parent_.addField())
    {
    }
    Field(Field& parent) = delete;  // prevent copy construction

    Field& operator=(Field const& f)
    {
      parent_.set(index_, f.parent_.get(index_));
      return *this;
    }

    Field& operator=(uint16_t state)
    {
      parent_.set(index_, state);
      return *this;
    }

    operator uint32_t() const
    {
      return parent_.get(index_);
    }

  private:
    SubBitPackedStruct<Ns...>& parent_;
    uint32_t const index_;
  };

private:
  // field methods
  uint16_t fieldIndex_ = 0;

  constexpr uint16_t addField()
  {
    return fieldIndex_++;
  }
};

}  // namespace kt

#endif  // _KT_SUBBITPACKEDSTRUCT_H_
