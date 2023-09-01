#ifndef KT_SUBBITPACKEDSTRUCTARRAY_ITERATOR_H
#define KT_SUBBITPACKEDSTRUCTARRAY_ITERATOR_H

#include <cstdint>

template <typename StructEntry, std::size_t NumEntries>
struct kt::SubBitPackedStructArray<StructEntry, NumEntries>::Iterator
{
  using iterator_category = std::forward_iterator_tag;
  using value_type = StructEntry;

  Iterator(SubBitPackedStructArray& instance, uint32_t entry_index)
      : instance_{instance}, entry_index_{entry_index}
  {
  }

  value_type operator*() const
  {
    return instance_.entries_[entry_index_];
  }
  value_type operator->() const
  {
    return *this;
  }
  Iterator& operator++()
  {
    ++entry_index_;
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
    return a.entry_index_ == b.entry_index_ && &a.instance_ == &b.instance_;
  }

  friend bool operator!=(const Iterator& a, const Iterator& b)
  {
    return a.entry_index_ != b.entry_index_ || &a.instance_ != &b.instance_;
  }

private:
  SubBitPackedStructArray& instance_;
  uint32_t entry_index_;
};

#endif  // KT_SUBBITPACKEDSTRUCTARRAY_ITERATOR_H
