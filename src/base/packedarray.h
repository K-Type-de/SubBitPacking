#ifndef KT_PACKEDARRAY_H
#define KT_PACKEDARRAY_H

#ifdef KT_ENABLE_EXCEPTIONS
#include <stdexcept>
#endif  // KT_ENABLE_EXCEPTIONS

#include <cstdint>

#include "../utils/packedstate.h"

namespace kt
{

template <std::size_t NumValues>
class PackedArray
{
protected:
  PackedArray() = default;

  inline void checkAllBoundries(std::size_t array_index, std::size_t array_size,
                                std::size_t value_index = 0) const
  {
#ifdef KT_ENABLE_EXCEPTIONS
    this->checkValueBoundries(value_index);
    this->checkArrayBoundries(array_index, array_size);
#endif  // KT_ENABLE_EXCEPTIONS
  }

  inline void checkValueBoundries(std::size_t value_index) const
  {
#ifdef KT_ENABLE_EXCEPTIONS
    if (value_index >= NumValues)
    {
      throw std::out_of_range{"[PackedArray] Value index out of range"};
    }
#endif  // KT_ENABLE_EXCEPTIONS
  }

  inline void checkArrayBoundries(std::size_t array_index, std::size_t array_size) const
  {
#ifdef KT_ENABLE_EXCEPTIONS
    if (array_index >= array_size)
    {
      throw std::out_of_range{"[PackedArray] Array index out of range"};
    }
#endif  // KT_ENABLE_EXCEPTIONS
  }
};

template <uint16_t NumStates, std::size_t NumValues>
class PackedStateArray : public PackedArray<NumValues>
{
  static_assert(NumStates > 1 && NumStates < 65536,
                "[PackedStateArray] Number of states must be between 2 and 65535");

protected:
  PackedStateArray() {}

public:
  virtual PackedState get(std::size_t value_index) const = 0;
  virtual void set(std::size_t value_index, PackedState state) = 0;
  virtual std::size_t getEntrySize() const = 0;
  virtual std::size_t getByteSize() const = 0;
};

template <std::size_t NumStructs>
class PackedStructArray : public PackedArray<NumStructs>
{
protected:
  PackedStructArray() {}

public:
  virtual PackedState get(std::size_t entry_index, std::size_t state_index) const = 0;
  virtual void set(std::size_t entry_index, std::size_t state_index, PackedState state) = 0;
  virtual std::size_t getEntrySize() const = 0;
  virtual std::size_t getByteSize() const = 0;
};

}  // namespace kt
#endif  // KT_PACKEDARRAY_H
