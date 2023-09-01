#ifndef KT_BITPACKEDARRAY_ITERATOR_H
#define KT_BITPACKEDARRAY_ITERATOR_H

template <uint16_t NumStates, std::size_t NumValues>
struct kt::BitPackedArray<NumStates, NumValues>::Iterator
{
  using iterator_category = std::forward_iterator_tag;
  using value_type = PackedState;

  Iterator(BitPackedArray& instance, uint32_t value_index)
      : instance_{instance}, value_index_{value_index}
  {
  }

  value_type operator*() const
  {
    return this->instance_.get(this->value_index_);
  }
  value_type operator->() const
  {
    return *this;
  }
  Iterator& operator++()
  {
    this->value_index_++;
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
    return a.value_index_ == b.value_index_ && &a.instance_ == &b.instance_;
  }

  friend bool operator!=(const Iterator& a, const Iterator& b)
  {
    return a.value_index_ != b.value_index_ || &a.instance_ != &b.instance_;
  }

private:
  BitPackedArray& instance_;
  uint32_t value_index_;
};

#endif  // KT_BITPACKEDARRAY_ITERATOR_H
