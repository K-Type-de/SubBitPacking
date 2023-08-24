#ifndef _KT_UINTPACKED_H_
#define _KT_UINTPACKED_H_

#if defined __has_attribute
#if __has_attribute(packed) && __has_attribute(gcc_struct)
#define UINTPACKED_GCC_PACKING
#define UINTPACKED_GCC_PACKING_ATTRIBUTES __attribute__((packed)) __attribute__((gcc_struct))
#endif
#else
#define UINTPACKED_GCC_PACKING_ATTRIBUTES
#endif

#include <cstdint>

template <uint32_t bits>
struct uintPacked
{
  template <typename T>
  uintPacked& operator=(T value)
  {
    this->value = value;
    return *this;
  }

  operator uint32_t() const
  {
    return this->value;
  }
#ifdef UINTPACKED_GCC_PACKING
  unsigned int value : bits;
  uintPacked(uint32_t value) : value{value} {}
#else
  /*
   * Dummy functions to get the smallest possible data type for the state values
   */
  template <int N = bits, typename std::enable_if<(N <= 8), int>::type = 0>
  static uint8_t DataSize();
  template <int N = bits, typename std::enable_if<(N > 8 && N <= 16), int>::type = 0>
  static uint16_t DataSize();
  template <int N = bits, typename std::enable_if<(N > 16), int>::type = 0>
  static uint32_t DataSize();
  /***************************************************************************/

  decltype(DataSize()) value;
  uintPacked(decltype(DataSize()) value) : value{value} {}
#endif
} UINTPACKED_GCC_PACKING_ATTRIBUTES;

// };

#endif  //_KT_UINTPACKED_H_