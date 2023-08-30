#ifndef _KT_SUPERBITPACKEDSTRUCTARRAY_ITERATOR_H_
#define _KT_SUPERBITPACKEDSTRUCTARRAY_ITERATOR_H_

#include <cstdint>

template <typename StructEntry, std::size_t num_entries>
struct kt::SuperBitPackedStructArray<StructEntry, num_entries>::Iterator
{
  using iterator_category = std::forward_iterator_tag;
  using value_type = StructEntry;

  Iterator(SuperBitPackedStructArray& instance, uint32_t value_index)
      : instance_{instance}, value_index_{value_index}
  {
  }

  value_type operator*() const
  {
    return instance_.getEntryCopy(value_index_);
  }
  value_type operator->() const
  {
    return *this;
  }
  Iterator& operator++()
  {
    value_index_++;
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
  SuperBitPackedStructArray& instance_;
  uint32_t value_index_;
};

#endif  // _KT_SUPERBITPACKEDSTRUCTARRAY_ITERATOR_H_
