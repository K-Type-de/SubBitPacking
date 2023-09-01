#ifndef KT_UINTPACKED_H
#define KT_UINTPACKED_H

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
namespace internal
{

template <uint32_t Bits>
struct UintPacked
{
  template <typename T>
  UintPacked& operator=(T value)
  {
    this->value = value;
    return *this;
  }

  operator uint32_t() const
  {
    return this->value;
  }
#ifdef UINTPACKED_COMPILER_PACKING
  unsigned int value : (Bits / 8 + (Bits % 8 != 0 ? 1 : 0)) *
                       8;  // Set bitfield size to next multiple of 8
  UintPacked(uint32_t value) : value{value} {}
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
  static internal::Uint24Kt DataSize();
  template <int N = bits, typename std::enable_if<(N > 24), int>::type = 0>
  static uint32_t DataSize();
  /***************************************************************************/

  decltype(DataSize()) value;
  UintPacked(decltype(DataSize()) value) : value{value} {}
};
#endif

}  // namespace internal
}  // namespace kt
#endif  // KT_UINTPACKED_H
