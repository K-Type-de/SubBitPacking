#ifndef _SUBBITPACKEDARRAY_ITERATOR_H_
#define _SUBBITPACKEDARRAY_ITERATOR_H_

#include <cstdint>

template <uint16_t num_states, std::size_t num_values>
struct kt::SubBitPackedArray<num_states, num_values>::Iterator
{
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = uint16_t;

  Iterator(SubBitPackedArray* ptr, uint32_t index) : ptr_{ptr}, index_{index} {}

  value_type operator*() const
  {
    return ptr_->entries_[index_ / kStatesPer4ByteWord].getState(
        kPowerLookupTable[index_ % kStatesPer4ByteWord], num_states);
  }
  value_type operator->() const
  {
    return *this;
  }
  Iterator& operator++()
  {
    index_++;
    return *this;
  }
  Iterator operator++(int)
  {
    Iterator tmp = *this;
    ++index_;
    return tmp;
  }
  Iterator& operator--()
  {
    index_--;
    return *this;
  }
  Iterator operator--(int)
  {
    Iterator tmp = *this;
    --index_;
    return tmp;
  }
  friend bool operator==(const Iterator& a, const Iterator& b)
  {
    return a.ptr_ == b.ptr_ && a.index_ == b.index_;
  };

  friend bool operator!=(const Iterator& a, const Iterator& b)
  {
    return a.ptr_ != b.ptr_ || a.index_ != b.index_;
  };

private:
  SubBitPackedArray* ptr_;
  uint32_t index_;
};

#endif  // _SUBBITPACKEDARRAY_ITERATOR_H_