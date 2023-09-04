#ifndef KT_SUPERBITPACKEDARRAYENTRYMETADATA_
#define KT_SUPERBITPACKEDARRAYENTRYMETADATA_

#include <cstddef>

namespace kt
{
namespace internal
{
struct SuperBitPackedArrayEntryMetadata
{
  std::size_t start_index;
  std::size_t bit_shift;
};
}  // namespace internal
}  // namespace kt

#endif  // KT_SUPERBITPACKEDARRAYENTRYMETADATA_
