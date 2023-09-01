#ifndef KT_UINT24_H
#define KT_UINT24_H

#include <cstdint>
#include <cstring>

namespace kt
{
namespace internal
{

enum class endian
{
#if defined(_MSC_VER) && !defined(__clang__)
  kLittle = 0,
  kBig = 1,
  kNative = kLittle
#else
  kLittle = __ORDER_LITTLE_ENDIAN__,
  kBig = __ORDER_BIG_ENDIAN__,
  kNative = __BYTE_ORDER__
#endif
};

class Uint24Kt
{
  uint8_t data_[3];

  template <int N = endian::kNative == endian::kBig,
            typename std::enable_if<(N > 0), int>::type = 0>
  inline uint32_t get() const
  {
    return (data_[2] << 0) | (data_[1] << 8) | (data_[0] << 16);
  }

  template <int N = endian::kNative == endian::kLittle,
            typename std::enable_if<(N > 0), int>::type = 0>
  inline typename std::enable_if<(N > 0), uint32_t>::type get() const
  {
    return (data_[0] << 0) | (data_[1] << 8) | (data_[2] << 16);
  }

  template <int N = endian::kNative == endian::kBig,
            typename std::enable_if<(N > 0), int>::type = 0>
  inline void set(uint32_t value)
  {
    memcpy(data_, &reinterpret_cast<uint8_t *>(&value)[1], 3);
  }

  template <int N = endian::kNative == endian::kLittle,
            typename std::enable_if<(N > 0), int>::type = 0>
  inline typename std::enable_if<(N > 0), void>::type set(uint32_t value)
  {
    memcpy(data_, &value, 3);
  }

public:
  Uint24Kt(uint32_t value)
  {
    this->set(value);
  }

  Uint24Kt &operator=(uint32_t value)
  {
    this->set(value);
    return *this;
  }

  operator uint32_t() const
  {
    return this->get();
  }
};

}  // namespace internal
}  // namespace kt

#endif  // KT_UINT24_H
