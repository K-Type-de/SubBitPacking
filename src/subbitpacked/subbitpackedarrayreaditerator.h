#ifndef KT_SUBBITPACKEDARRAY_READ_ITERATOR_H
#define KT_SUBBITPACKEDARRAY_READ_ITERATOR_H

template <uint16_t NumStates, std::size_t NumValues>
struct kt::SubBitPackedArray<NumStates, NumValues>::ReadIterator
{
  using iterator_category = std::forward_iterator_tag;
  using value_type = PackedState;

  ReadIterator(const SubBitPackedArray& instance, uint32_t value_index)
      : instance_{instance},
        entry_index_{value_index / kStatesPer4ByteWord},
        chunk_value_index_{static_cast<uint8_t>(value_index % kStatesPer4ByteWord)}
  {
    if (entry_index_ >= instance_.getEntrySize())
    {
      return;
    }

    chunk_ = instance_.entries_[entry_index_];

    for (std::size_t i = 0; i < chunk_value_index_; ++i)
    {
      chunk_ /= NumStates;
    }
  }

  value_type operator*() const
  {
    return chunk_ % NumStates;
  }
  value_type operator->() const
  {
    return *this;
  }
  ReadIterator& operator++()
  {
    chunk_value_index_++;
    if (chunk_value_index_ == kStatesPer4ByteWord)
    {
      entry_index_++;
      chunk_value_index_ = 0;
      chunk_ = instance_.entries_[entry_index_];
    }
    else
    {
      chunk_ /= NumStates;
    }
    return *this;
  }
  ReadIterator operator++(int)
  {
    ReadIterator tmp = *this;
    ++(*this);
    return tmp;
  }
  friend bool operator==(const ReadIterator& a, const ReadIterator& b)
  {
    return a.entry_index_ == b.entry_index_ && a.chunk_value_index_ == b.chunk_value_index_ &&
           &a.instance_ == &b.instance_;
  }

  friend bool operator!=(const ReadIterator& a, const ReadIterator& b)
  {
    return a.entry_index_ != b.entry_index_ || a.chunk_value_index_ != b.chunk_value_index_ ||
           &a.instance_ != &b.instance_;
  }

private:
  const SubBitPackedArray& instance_;
  uint32_t entry_index_;
  uint32_t chunk_;
  uint8_t chunk_value_index_;
};

#endif  // KT_SUBBITPACKEDARRAY_READ_ITERATOR_H
