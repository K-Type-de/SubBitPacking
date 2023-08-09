#include "subbitpackedarray.h"

kt::SubBitPackedArrayEntry::SubBitPackedArrayEntry() : data_{0} {}

inline uint16_t kt::SubBitPackedArrayEntry::getState(uint32_t divisor, uint16_t modulo) const
{
  return this->data_ / divisor % modulo;
}

void kt::SubBitPackedArrayEntry::setState(uint32_t power, uint16_t modulo, uint16_t state)
{
  uint8_t current_state = this->getState(power, modulo);
  this->data_ = this->data_ - (current_state * power) + (state * power);
}

uint32_t kt::SubBitPackedArrayEntry::getData() const
{
  return this->data_;
}
