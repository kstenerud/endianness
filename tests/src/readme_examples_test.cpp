#include <gtest/gtest.h>
#include <endianness/endianness.h>
#include <stdio.h>

static void demonstrate_endianness()
{
    uint8_t data[] = {0x12, 0x34};

    printf("Read [%02x %02x] in BE order: %02x %02x\n",
    	data[0], data[1],
    	data[INDEX_BE(sizeof(data), 0)],
    	data[INDEX_BE(sizeof(data), 1)]);

    printf("Read [%02x %02x] in LE order: %02x %02x\n",
    	data[0], data[1],
    	data[INDEX_LE(sizeof(data), 0)],
    	data[INDEX_LE(sizeof(data), 1)]);

    printf("Read [%02x %02x] as BE int: %04x\n", data[0], data[1],read_uint16_be(data));
    printf("Read [%02x %02x] as LE int: %04x\n", data[0], data[1],read_uint16_le(data));

    uint16_t value = 0x1234;

    write_uint16_be(value, data);
    printf("Write int %04x as BE: %02x %02x\n", value, data[0], data[1]);

    write_uint16_le(value, data);
    printf("Write int %04x as LE: %02x %02x\n", value, data[0], data[1]);

    // These will always give the same result, regardless of host endianness.
    uint8_t* ptr = (uint8_t*)&value;
    printf("Get from high byte of int %04x, index 0: %02x\n", value, ptr[INDEX_HB(sizeof(value), 0)]);
    printf("Get from high byte of int %04x, index 1: %02x\n", value, ptr[INDEX_HB(sizeof(value), 1)]);
    printf("Get from low byte of int %04x, index 0: %02x\n", value, ptr[INDEX_LB(sizeof(value), 0)]);
    printf("Get from low byte of int %04x, index 1: %02x\n", value, ptr[INDEX_LB(sizeof(value), 1)]);
}

TEST(Readme, example)
{
	demonstrate_endianness();
}
