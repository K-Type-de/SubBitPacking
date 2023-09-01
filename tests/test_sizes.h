#ifndef KT_TEST_SIZES_SHORT_H
#define KT_TEST_SIZES_SHORT_H

#ifdef TEST_SIZES_SHORT
using test_state_sizes =
    ::testing::Types<std::integral_constant<uint8_t, 2>, std::integral_constant<uint8_t, 3>,
                     std::integral_constant<uint8_t, 4>, std::integral_constant<uint8_t, 5>,
                     std::integral_constant<uint8_t, 6>, std::integral_constant<uint8_t, 7>,
                     std::integral_constant<uint8_t, 8>, std::integral_constant<uint8_t, 9>,
                     std::integral_constant<uint8_t, 10>, std::integral_constant<uint8_t, 11>,
                     std::integral_constant<uint8_t, 12>, std::integral_constant<uint8_t, 13>,
                     std::integral_constant<uint8_t, 14>, std::integral_constant<uint8_t, 15>,
                     std::integral_constant<uint8_t, 16>, std::integral_constant<uint8_t, 17>,
                     std::integral_constant<uint8_t, 18>, std::integral_constant<uint8_t, 19>,
                     std::integral_constant<uint8_t, 20>, std::integral_constant<uint8_t, 21>,
                     std::integral_constant<uint8_t, 22>, std::integral_constant<uint8_t, 23>,
                     std::integral_constant<uint8_t, 24>, std::integral_constant<uint8_t, 25>,
                     std::integral_constant<uint8_t, 26>, std::integral_constant<uint8_t, 27>,
                     std::integral_constant<uint8_t, 28>, std::integral_constant<uint8_t, 29>,
                     std::integral_constant<uint8_t, 30>, std::integral_constant<uint8_t, 31>,
                     std::integral_constant<uint8_t, 32>, std::integral_constant<uint16_t, 1025>>;

#else
using test_state_sizes =
    ::testing::Types<std::integral_constant<uint8_t, 2>, std::integral_constant<uint8_t, 3>,
                     std::integral_constant<uint8_t, 4>, std::integral_constant<uint8_t, 5>,
                     std::integral_constant<uint8_t, 6>, std::integral_constant<uint8_t, 7>,
                     std::integral_constant<uint8_t, 8>, std::integral_constant<uint8_t, 9>,
                     std::integral_constant<uint8_t, 10>, std::integral_constant<uint8_t, 11>,
                     std::integral_constant<uint8_t, 12>, std::integral_constant<uint8_t, 13>,
                     std::integral_constant<uint8_t, 14>, std::integral_constant<uint8_t, 15>,
                     std::integral_constant<uint8_t, 16>, std::integral_constant<uint8_t, 17>,
                     std::integral_constant<uint8_t, 18>, std::integral_constant<uint8_t, 19>,
                     std::integral_constant<uint8_t, 20>, std::integral_constant<uint8_t, 21>,
                     std::integral_constant<uint8_t, 22>, std::integral_constant<uint8_t, 23>,
                     std::integral_constant<uint8_t, 24>, std::integral_constant<uint8_t, 25>,
                     std::integral_constant<uint8_t, 26>, std::integral_constant<uint8_t, 27>,
                     std::integral_constant<uint8_t, 28>, std::integral_constant<uint8_t, 29>,
                     std::integral_constant<uint8_t, 30>, std::integral_constant<uint8_t, 31>,
                     std::integral_constant<uint8_t, 32>, std::integral_constant<uint8_t, 33>,
                     std::integral_constant<uint8_t, 34>, std::integral_constant<uint8_t, 35>,
                     std::integral_constant<uint8_t, 36>, std::integral_constant<uint8_t, 37>,
                     std::integral_constant<uint8_t, 38>, std::integral_constant<uint8_t, 39>,
                     std::integral_constant<uint8_t, 40>, std::integral_constant<uint8_t, 65>,
                     std::integral_constant<uint8_t, 66>, std::integral_constant<uint8_t, 67>,
                     std::integral_constant<uint8_t, 68>, std::integral_constant<uint8_t, 69>,
                     std::integral_constant<uint8_t, 70>, std::integral_constant<uint8_t, 71>,
                     std::integral_constant<uint8_t, 72>, std::integral_constant<uint8_t, 73>,
                     std::integral_constant<uint8_t, 74>, std::integral_constant<uint8_t, 75>,
                     std::integral_constant<uint8_t, 76>, std::integral_constant<uint8_t, 77>,
                     std::integral_constant<uint8_t, 78>, std::integral_constant<uint8_t, 79>,
                     std::integral_constant<uint8_t, 80>, std::integral_constant<uint8_t, 81>,
                     std::integral_constant<uint8_t, 82>, std::integral_constant<uint8_t, 83>,
                     std::integral_constant<uint8_t, 84>, std::integral_constant<uint16_t, 1025>,
                     std::integral_constant<uint16_t, 65535>>;
#endif

#endif  // KT_TEST_SIZES_SHORT_H
