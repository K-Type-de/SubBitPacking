#ifndef _KT_COMPILETIMEUTILS_H_
#define _KT_COMPILETIMEUTILS_H_

#include <array>
#include <cstdint>

namespace kt
{
namespace CompileTime
{
/*
 * Generator function to calculate normal powers
 */
template <uint16_t base>
constexpr uint32_t Pow(uint8_t exponent)
{
  return (exponent == 0) ? 1 : (base * Pow<base>(exponent - 1));
}
/*****************************************************************************************/

/*
 * Generator functions to calculate powers with shifting base
 */
template <uint16_t base>
constexpr uint32_t VariadicStatePow(uint8_t exponent)
{
  return 1;
}

template <uint16_t base, uint16_t... extra>
constexpr auto VariadicStatePow(uint8_t exponent) ->
    typename std::enable_if<sizeof...(extra) != 0, uint32_t>::type
{
  return (exponent == 0) ? 1 : base * VariadicStatePow<extra...>(exponent - 1);
}

/*****************************************************************************************/

/*
 * Recursive Lookup table generator (C++11 compatible)
 * Uses above generator functions
 */
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
constexpr auto GeneratePowLut(Generator&& generator_function)
    -> std::array<decltype(generator_function(uint8_t{0})), Size>
{
  return lut<Size, decltype(generator_function(uint8_t{0}))>(
      std::forward<Generator>(generator_function));
}
/*****************************************************************************************/

/*
 * Calculate the highest possible number that the variadic arguments cam be summed up to
 */
template <uint64_t prev, uint64_t current>
constexpr uint64_t _HighestVariadicValue(uint64_t value, uint8_t exponent)
{
  return value + (current - 1) * prev;
}

template <uint64_t prev, uint64_t current, uint64_t... others>
constexpr auto _HighestVariadicValue(uint64_t value, uint8_t exponent) ->
    typename std::enable_if<sizeof...(others) != 0, uint64_t>::type
{
  return _HighestVariadicValue<current * prev, others...>(value + (current - 1) * prev,
                                                          exponent + 1);
}

template <uint64_t current>
constexpr uint64_t HighestVariadicValue()
{
  return current - 1;
}

template <uint64_t current, uint64_t... others>
constexpr auto HighestVariadicValue() ->
    typename std::enable_if<sizeof...(others) != 0, uint64_t>::type
{
  return _HighestVariadicValue<current, others...>(current - 1, 1);
}
/*****************************************************************************************/

/**
 * Returns the number of sub-bit packed states can fit into a 32-bit word
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

/**
 * Returns the number of bits needed to store a value
 * ...0001 => 1
 * ...0010 => 2
 * ...0011 => 2
 * ...0110 => 3
 * [...]
 */
static constexpr uint8_t NumberOfBitsPerValue(uint16_t value, uint8_t shift = 0)
{
  return (0x1 << shift) > value ? shift : NumberOfBitsPerValue(value, shift + 1);
}

/**
 * Returns the bitmask needed to mask the value
 * ...0001 => 1
 * ...0010 => 11
 * ...0011 => 11
 * ...0110 => 111
 * [...]
 */
static constexpr uint16_t BitMaskForValue(uint16_t value, uint16_t mask = 0x1)
{
  return (value & mask) == value ? mask : BitMaskForValue(value, (mask << 1) | 0x1);
}

/**
 * Recursively calculates the number of unused upper bits of a value:
 * 01100110... => 1
 * 00001010... => 4
 * [...]
 */
template <typename T>
static constexpr uint8_t NumberOfUnusedUpperBits(T value, int8_t shift = sizeof(T) * 8 - 1)
{
  return shift < 0                       ? 0
         : ((value >> shift) & 0x1) == 0 ? 1 + NumberOfUnusedUpperBits(value, shift - 1)
                                         : 0;
}
}  // namespace CompileTime
}  // namespace kt
#endif  //_KT_COMPILETIMEUTILS_H_
