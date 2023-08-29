#ifndef _KT_UINT24_H_
#define _KT_UINT24_H_

#include <cstdint>
#include <cstring>

namespace kt
{
namespace Internal
{

enum class endian
{
#if defined(_MSC_VER) && !defined(__clang__)
  little = 0,
  big = 1,
  native = little
#else
  little = __ORDER_LITTLE_ENDIAN__,
  big = __ORDER_BIG_ENDIAN__,
  native = __BYTE_ORDER__
#endif
};

class uint24_kt
{
  uint8_t _data[3];

  template <int N = endian::native == endian::big, typename std::enable_if<(N > 0), int>::type = 0>
  inline uint32_t get() const
  {
    return (_data[2] << 0) | (_data[1] << 8) | (_data[0] << 16);
  }

  template <int N = endian::native == endian::little,
            typename std::enable_if<(N > 0), int>::type = 0>
  inline typename std::enable_if<(N > 0), uint32_t>::type get() const
  {
    return (_data[0] << 0) | (_data[1] << 8) | (_data[2] << 16);
  }

  template <int N = endian::native == endian::big, typename std::enable_if<(N > 0), int>::type = 0>
  inline void set(uint32_t value)
  {
    memcpy(_data, &reinterpret_cast<uint8_t *>(&value)[1], 3);
  }

  template <int N = endian::native == endian::little,
            typename std::enable_if<(N > 0), int>::type = 0>
  inline typename std::enable_if<(N > 0), void>::type set(uint32_t value)
  {
    memcpy(_data, &value, 3);
  }

public:
  uint24_kt(uint32_t value)
  {
    this->set(value);
  }

  uint24_kt &operator=(uint32_t value)
  {
    this->set(value);
    return *this;
  }

  operator uint32_t() const
  {
    return this->get();
  }
};

}  // namespace Internal
}  // namespace kt

#endif  //_KT_UINT24_H_