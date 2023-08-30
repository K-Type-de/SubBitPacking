#ifndef _KT_UINTPACKED_H_
#define _KT_UINTPACKED_H_

#if defined __has_attribute
#if __has_attribute(packed)
#define UINTPACKED_COMPILER_PACKING
#endif
#endif

#ifndef UINTPACKED_COMPILER_PACKING
#include "uint24.h"
#endif

#include <cstdint>
namespace kt
{
namespace Internal
{

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
#ifdef UINTPACKED_COMPILER_PACKING
  unsigned int value : (bits / 8 + (bits % 8 != 0)) *
                       8;  // Set bitfield size to next multiple of 8
  uintPacked(uint32_t value) : value{value} {}
} __attribute__((packed));
#else
  /*
   * Dummy functions to get the smallest possible data type for the state values
   * Not as good as UINTPACKED_COMPILER_PACKING, since the implementation for 24-bit is slower than
   * compiler packing
   */
  template <int N = bits, typename std::enable_if<(N <= 8), int>::type = 0>
  static uint8_t DataSize();
  template <int N = bits, typename std::enable_if<(N > 8 && N <= 16), int>::type = 0>
  static uint16_t DataSize();
  template <int N = bits, typename std::enable_if<(N > 16 && N <= 24), int>::type = 0>
  static Internal::uint24_kt DataSize();
  template <int N = bits, typename std::enable_if<(N > 24), int>::type = 0>
  static uint32_t DataSize();
  /***************************************************************************/

  decltype(DataSize()) value;
  uintPacked(decltype(DataSize()) value) : value{value} {}
};
#endif

}  // namespace Internal
}  // namespace kt
#endif  //_KT_UINTPACKED_H_