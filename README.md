Endianness Macros and Functions for C and C++
=============================================

Utilities for reading and writing multibyte values in big or little endian format.

Implemented as a header only ([`endianness.h`](https://github.com/kstenerud/endianness/blob/master/include/endianness/endianness.h)).



Header Dependencies
-------------------

 * stdint.h: For standard integer types
 * math.h: To test for `__STDC_IEC_559__`


Usage
-----

```c
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
```

Result:

    Read [12 34] in BE order: 12 34
    Read [12 34] in LE order: 34 12
    Read [12 34] as BE int: 1234
    Read [12 34] as LE int: 3412
    Write int 1234 as BE: 12 34
    Write int 1234 as LE: 34 12
    Get from high byte of int 1234, index 0: 12
    Get from high byte of int 1234, index 1: 34
    Get from low byte of int 1234, index 0: 34
    Get from low byte of int 1234, index 1: 12



Requirements
------------

  * Meson 0.49 or newer
  * Ninja 1.8.2 or newer
  * A C compiler
  * A C++ compiler (for the tests)



Building
--------

    meson build
    ninja -C build



Running Tests
-------------

    ninja -C build test

For the full report:

    ./build/run_tests



Installing
----------

    ninja -C build install



License
-------

Copyright 2019 Karl Stenerud

Released under MIT License https://opensource.org/licenses/MIT
