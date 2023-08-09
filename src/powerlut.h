#ifndef _POWERLUT_H_
#define _POWERLUT_H_

/**
 * C++11 compatible way to calculate a lookup table of various powers at compile time.
 * A more readable version for C++17 would look something like this:
 *
 *  constexpr auto PowerLookupTable{[]() constexpr
 *                       {
 *                          std::array<uint32_t, length> result{};
 *                          result[0] = 1;
 *                          for (int i = 1; i < kStatesPer4ByteWord; ++i)
 *                          {
 *                              result[i] = result[i - 1] * base;
 *                          }
 *                          return result;
 *                          }()
 *                       };
 **/

#include <array>
#include <cstdint>

template <uint16_t base>
constexpr uint32_t pow(uint8_t n)
{
  return (n == 0) ? 1 : (base * pow<base>(n - 1));
}

template <std::size_t Size, typename T, typename Generator, typename... Values>
constexpr auto lut(Generator&& generator_function, Values... values) ->
    typename std::enable_if<sizeof...(Values) == (Size - 1), std::array<T, Size>>::type
{
  return {values..., std::forward<Generator>(generator_function)(sizeof...(Values))};
}

template <std::size_t Size, typename T, typename Generator, typename... Values>
constexpr auto lut(Generator&& generator_function, Values... values) ->
    typename std::enable_if<sizeof...(Values) < (Size - 1), std::array<T, Size>>::type
{
  return lut<Size, T, Generator, Values..., decltype(generator_function(uint8_t{0}))>(
      std::forward<Generator>(generator_function), values...,
      generator_function(sizeof...(Values)));
}

template <std::size_t Size, typename Generator>
constexpr auto generate_pow_lut(Generator&& generator_function)
    -> std::array<decltype(generator_function(uint8_t{0})), Size>
{
  return lut<Size, decltype(generator_function(uint8_t{0}))>(
      std::forward<Generator>(generator_function));
}

#endif  //_POWERLUT_H_
