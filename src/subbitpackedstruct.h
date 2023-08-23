#ifndef _KT_SUBBITPACKESTRUCT_H_
#define _KT_SUBBITPACKESTRUCT_H_

#include <cstdint>

#include "utils/compiletime.h"

namespace kt
{

template <uint16_t... Ns>
class TStruct
{
public:
  static constexpr std::size_t kNumFields = sizeof...(Ns);
  static constexpr std::array<uint32_t, kNumFields> kNumStates = {Ns...};
  static constexpr std::array<uint32_t, kNumFields> kStatePowers =
      CompileTime::GeneratePowLut<kNumFields>(CompileTime::VariadicStatePow<Ns...>);
};

// C++11 linker madness
template <uint16_t... Ns>
constexpr std::size_t TStruct<Ns...>::kNumFields;

template <uint16_t... Ns>
constexpr std::array<uint32_t, TStruct<Ns...>::kNumFields> TStruct<Ns...>::kNumStates;

template <uint16_t... Ns>
constexpr std::array<uint32_t, TStruct<Ns...>::kNumFields> TStruct<Ns...>::kStatePowers;

class SubBitPackedStruct
{
protected:
  template <uint16_t num_states>
  class Field
  {
  public:
    Field(SubBitPackedStruct& parent) : parent_(parent), index_(parent_.addField(*this)) {}
    Field(Field& parent) = delete;  // prevent copy construction
    Field& operator=(Field const&) = delete;

    Field& operator=(uint16_t state)
    {
      // Exception Handling

      parent_.setField(index_, state);

      return *this;
    }

    operator uint32_t()
    {
      return parent_.getField(index_);
    }

  private:
    SubBitPackedStruct& parent_;
    uint32_t const index_;
  };

private:
  // std::list<uint32_t> power_value_; // saves the exponential power for the all fields

  template <uint16_t num_states>
  uint32_t addField(Field<num_states>& field)
  {
    // add field magic and return index to identify field
    uint32_t states_to_register = num_states;

    // power_value_.push_back();

    return 0;
  }

  void setField(uint32_t index, uint32_t state)
  {
    // do the magic
  }

  uint32_t getField(uint32_t index)
  {
    // do the magic
    return 0;
  }

protected:
  SubBitPackedStruct() = default;  // force inheritance

public:
  virtual ~SubBitPackedStruct() = default;

  SubBitPackedStruct(const SubBitPackedStruct&) = delete;             // no copy construction
  SubBitPackedStruct& operator=(const SubBitPackedStruct&) = delete;  // no copy assignment
};

}  // namespace kt

#endif  // _KT_SUBBITPACKESTRUCT_H_
