#ifndef _COMPILETIMEUTILS_H_
#define _COMPILETIMEUTILS_H_

#include <array>
#include <cstdint>

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

/**
 * Returns the number of states you can pack into a 32-bit word
 */
static constexpr uint8_t NumberOfStatesPer4ByteWord(uint16_t num_states)
{
  return num_states <= 2      ? 32
         : num_states == 3    ? 20
         : num_states == 4    ? 16
         : num_states == 5    ? 13
         : num_states == 6    ? 12
         : num_states == 7    ? 11
         : num_states <= 9    ? 10
         : num_states <= 11   ? 9
         : num_states <= 16   ? 8
         : num_states <= 23   ? 7
         : num_states <= 40   ? 6
         : num_states <= 84   ? 5
         : num_states <= 256  ? 4
         : num_states <= 1625 ? 3
                              : 2;
}

#endif  //_COMPILETIMEUTILS_H_
