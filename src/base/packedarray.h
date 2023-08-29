#ifndef _KT_PACKEDARRAY_H_
#define _KT_PACKEDARRAY_H_

#ifdef KT_PACKEDARRAY_ENABLE_EXCEPTIONS
#include <stdexcept>
#endif  // KT_PACKEDARRAY_ENABLE_EXCEPTIONS

#include <cstdint>

namespace kt
{

template <uint16_t num_states, std::size_t num_values>
class PackedArray
{
  static_assert(num_states > 1 && num_states < 65536,
                "[PackedArray] Number of states must be between 2 and 65535");

protected:
  PackedArray() {}

  inline void checkValueBoundries(std::size_t value_index) const
  {
#ifdef KT_PACKEDARRAY_ENABLE_EXCEPTIONS
    if (value_index >= num_values)
    {
      throw std::out_of_range{"[PackedArray] Value index out of range"};
    }
#endif  // KT_PACKEDARRAY_ENABLE_EXCEPTIONS
  }

public:
  virtual uint16_t get(std::size_t value_index) const = 0;
  virtual void set(std::size_t value_index, uint16_t state) = 0;
  virtual std::size_t getEntrySize() const = 0;
  virtual std::size_t getByteSize() const = 0;
};

}  // namespace kt
#endif  //_KT_PACKEDARRAY_H_
