#ifndef KT_SUBBITPACKEDARRAY_CLEANWRITE_ITERATOR_H
#define KT_SUBBITPACKEDARRAY_CLEANWRITE_ITERATOR_H

template <uint16_t NumStates, std::size_t NumValues>
struct kt::SubBitPackedArray<NumStates, NumValues>::CleanWriteIterator
{
private:
  struct PackedStateWriteProxy
  {
    CleanWriteIterator* iterator_;
    PackedStateWriteProxy(CleanWriteIterator* iterator) : iterator_{iterator} {}

    inline void setValue(uint16_t val)
    {
      iterator_->instance_.entries_[iterator_->entry_index_] +=
          val * kPowerLookupTable[iterator_->chunk_value_index_];
    }

    void operator=(uint16_t val)
    {
      this->setValue(val);
    }
  };

  using iterator_category = std::output_iterator_tag;
  using value_type = PackedStateWriteProxy;

public:
  CleanWriteIterator(SubBitPackedArray& instance, uint32_t value_index)
      : instance_{instance},
        entry_index_{value_index / kStatesPer4ByteWord},
        chunk_value_index_{static_cast<uint8_t>(value_index % kStatesPer4ByteWord)}
  {
  }

  value_type operator*()
  {
    return PackedStateWriteProxy{this};
  }
  value_type operator->()
  {
    return *this;
  }

  CleanWriteIterator& operator++()
  {
    chunk_value_index_++;
    if (chunk_value_index_ == kStatesPer4ByteWord)
    {
      entry_index_++;
      chunk_value_index_ = 0;
    }
    return *this;
  }

  CleanWriteIterator operator++(int)
  {
    CleanWriteIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator==(const CleanWriteIterator& a, const CleanWriteIterator& b)
  {
    return a.entry_index_ == b.entry_index_ && a.chunk_value_index_ == b.chunk_value_index_ &&
           &a.instance_ == &b.instance_;
  }

  friend bool operator!=(const CleanWriteIterator& a, const CleanWriteIterator& b)
  {
    return a.entry_index_ != b.entry_index_ || a.chunk_value_index_ != b.chunk_value_index_ ||
           &a.instance_ != &b.instance_;
  }

private:
  SubBitPackedArray& instance_;
  uint32_t entry_index_;
  uint8_t chunk_value_index_;
};

#endif  // KT_SUBBITPACKEDARRAY_CLEANWRITE_ITERATOR_H
