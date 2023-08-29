#ifndef _KT_BITPACKEDARRAY_ITERATOR_H_
#define _KT_BITPACKEDARRAY_ITERATOR_H_

#include <cstdint>

template <uint16_t num_states, std::size_t num_values>
struct kt::BitPackedArray<num_states, num_values>::Iterator
{
  using iterator_category = std::forward_iterator_tag;
  using value_type = uint16_t;

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

#endif  //_KT_BITPACKEDARRAY_ITERATOR_H_