#include <gtest/gtest.h>
#include <endianness/endianness.h>
#include <vector>
#include <algorithm>

#define DEFINE_TEST(NAME, TYPE, VALUE, ...) \
TEST(Endianness, NAME) \
{ \
    TYPE expected_value = VALUE; \
    TYPE actual_value = 0; \
    std::vector<uint8_t> expected_data = __VA_ARGS__; \
    std::vector<uint8_t> actual_data(sizeof(expected_value)); \
 \
    write_ ## NAME ## _be(expected_value, actual_data.data()); \
    EXPECT_EQ(expected_data, actual_data); \
    actual_value = read_ ## NAME ## _be(expected_data.data()); \
    EXPECT_EQ(expected_value, actual_value); \
 \
    std::reverse(expected_data.begin(), expected_data.end()); \
    write_ ## NAME ## _le(expected_value, actual_data.data()); \
    EXPECT_EQ(expected_data, actual_data); \
    actual_value = read_ ## NAME ## _le(expected_data.data()); \
    EXPECT_EQ(expected_value, actual_value); \
}

DEFINE_TEST(int16, int16_t, -2, {0xff, 0xfe})
DEFINE_TEST(uint16, uint16_t, 0x1234, {0x12, 0x34})

DEFINE_TEST(int32, int32_t, -2, {0xff, 0xff, 0xff, 0xfe})
DEFINE_TEST(uint32, uint32_t, 0x12345678, {0x12, 0x34, 0x56, 0x78})

DEFINE_TEST(int64, int64_t, -2, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe})
DEFINE_TEST(uint64, uint64_t, 0x123456789abcdef0, {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0})

TEST(Endianness, primitives)
{
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    EXPECT_EQ(0x01, data[INDEX_BE(sizeof(data), 0)]);
    EXPECT_EQ(0x02, data[INDEX_BE(sizeof(data), 1)]);
    EXPECT_EQ(0x04, data[INDEX_LE(sizeof(data), 0)]);
    EXPECT_EQ(0x03, data[INDEX_LE(sizeof(data), 1)]);

    uint32_t value = 0x01020304;
    uint8_t* ptr = (uint8_t*)&value;
    EXPECT_EQ(0x01, ptr[INDEX_HB(sizeof(data), 0)]);
    EXPECT_EQ(0x02, ptr[INDEX_HB(sizeof(data), 1)]);
    EXPECT_EQ(0x04, ptr[INDEX_LB(sizeof(data), 0)]);
    EXPECT_EQ(0x03, ptr[INDEX_LB(sizeof(data), 1)]);
}
