#ifndef KT_SUBBITPACKING_H
#define KT_SUBBITPACKING_H

#include "bitpacked.h"
#include "subbitpacked.h"
#include "superbitpacked.h"

namespace kt
{

template <uint16_t NumStates, std::size_t NumValues>
class SubBitPacking
{
  static constexpr std::size_t kBitPackingArraySize = sizeof(BitPackedArray<NumStates, NumValues>);
  static constexpr std::size_t kSubBitPackingArraySize =
      sizeof(SubBitPackedArray<NumStates, NumValues>);
  static constexpr std::size_t kSuperBitPackinArraygSize =
      sizeof(SuperBitPackedArray<NumStates, NumValues>);

public:
  template <int N = kBitPackingArraySize,
            typename std::enable_if<
                (N <= kSubBitPackingArraySize && N <= kSuperBitPackinArraygSize), int>::type = 0>
  static PackedStateArray<NumStates, NumValues> GetPackedStateArrayStack()
  {
    return BitPackedArray<NumStates, NumValues>{};
  }

  template <int N = kSubBitPackingArraySize,
            typename std::enable_if<(N < kBitPackingArraySize && N <= kSuperBitPackinArraygSize),
                                    int>::type = 0>
  static PackedStateArray<NumStates, NumValues> GetPackedStateArrayStack()
  {
    return SubBitPackedArray<NumStates, NumValues>{};
  }

  template <int N = kSuperBitPackinArraygSize,
            typename std::enable_if<(N < kBitPackingArraySize && N < kSubBitPackingArraySize),
                                    int>::type = 0>
  static PackedStateArray<NumStates, NumValues> GetPackedStateArrayStack()
  {
    return SuperBitPackedArray<NumStates, NumValues>{};
  }

  template <int N = kBitPackingArraySize,
            typename std::enable_if<
                (N <= kSubBitPackingArraySize && N <= kSuperBitPackinArraygSize), int>::type = 0>
  static PackedStateArray<NumStates, NumValues>* GetPackedStateArrayHeap()
  {
    return new BitPackedArray<NumStates, NumValues>{};
  }

  template <int N = kSubBitPackingArraySize,
            typename std::enable_if<(N < kBitPackingArraySize && N <= kSuperBitPackinArraygSize),
                                    int>::type = 0>
  static PackedStateArray<NumStates, NumValues>* GetPackedStateArrayHeap()
  {
    return new SubBitPackedArray<NumStates, NumValues>{};
  }

  template <int N = kSuperBitPackinArraygSize,
            typename std::enable_if<(N < kBitPackingArraySize && N < kSubBitPackingArraySize),
                                    int>::type = 0>
  static PackedStateArray<NumStates, NumValues>* GetPackedStateArrayHeap()
  {
    return new SuperBitPackedArray<NumStates, NumValues>{};
  }
};

}  // namespace kt

#endif  // KT_SUBBITPACKING_H