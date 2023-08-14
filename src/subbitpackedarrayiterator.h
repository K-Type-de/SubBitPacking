#ifndef _SUBBITPACKEDARRAY_ITERATOR_H_
#define _SUBBITPACKEDARRAY_ITERATOR_H_

#include <cstdint>

template <uint16_t num_states, std::size_t num_values>
struct kt::SubBitPackedArray<num_states, num_values>::Iterator
{
  using iterator_category = std::forward_iterator_tag;
  using value_type = uint16_t;

  Iterator(SubBitPackedArray* ptr, uint32_t value_index)
      : ptr_{ptr},
        entry_index_{value_index / kStatesPer4ByteWord},
        chunk_value_index_{static_cast<uint8_t>(value_index % kStatesPer4ByteWord)}
  {
    if (entry_index_ >= ptr_->getEntrySize())
    {
      return;
    }

    chunk_ = ptr_->entries_[entry_index_].getData();

    for (size_t i = 0; i < chunk_value_index_; ++i)
    {
      chunk_ /= num_states;
    }
  }

  value_type operator*() const
  {
    return chunk_ % num_states;
  }
  value_type operator->() const
  {
    return *this;
  }
  Iterator& operator++()
  {
    chunk_value_index_++;
    if (chunk_value_index_ == kStatesPer4ByteWord)
    {
      entry_index_++;
      chunk_value_index_ = 0;
      chunk_ = ptr_->entries_[entry_index_].getData();
    }
    else
    {
      chunk_ /= num_states;
    }
    return *this;
  }
  Iterator operator++(int)
  {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }
  friend bool operator==(const Iterator& a, const Iterator& b)
  {
    return a.ptr_ == b.ptr_ && a.entry_index_ == b.entry_index_ &&
           a.chunk_value_index_ == b.chunk_value_index_;
  }

  friend bool operator!=(const Iterator& a, const Iterator& b)
  {
    return !(a == b);
  }

private:
  SubBitPackedArray* ptr_;
  uint32_t entry_index_;
  uint32_t chunk_;
  uint8_t chunk_value_index_;
};

#endif  // _SUBBITPACKEDARRAY_ITERATOR_H_