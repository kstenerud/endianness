// Copyright 2018 Karl Stenerud
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef BYTE_ORDER_IS_BIG_ENDIAN


// ===========================================================================

// Host Endianness Macros

#include <sys/param.h>

// This should get it right on all major platforms
#if defined(__LITTLE_ENDIAN__) || defined(__AARCH64EL__) || defined(__THUMBEL__) || \
defined(__ARMEL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || \
__BYTE_ORDER__ ==  __ORDER_LITTLE_ENDIAN__
    #define BYTE_ORDER_IS_BIG_ENDIAN 0
    #define BYTE_ORDER_IS_LITTLE_ENDIAN 1
#else
    #define BYTE_ORDER_IS_BIG_ENDIAN 1
    #define BYTE_ORDER_IS_LITTLE_ENDIAN 0
#endif


// ===========================================================================

// Get an index into a value whose address has been cast to a byte pointer.
//
// These macros will always return the same index value, regardless of
// host endianness. These are most useful when you want to read or write
// to a buffer in a consistent endianness.
//
// The following code will print "12, 34" on a big endian machine,
// and "34, 12" on a little endian machine:
//
//     uint16_t value = 0x1234;
//     uint8_t* ptr = (uint8_t*)&value;
//     printf("%02x, ", ptr[INDEX_BE(sizeof(value), 0)]);
//     printf("%02x\n", ptr[INDEX_LE(sizeof(value), 0)]);
#define INDEX_BE(TOTAL_SIZE, INDEX) (INDEX)
#define INDEX_LE(TOTAL_SIZE, INDEX) ((TOTAL_SIZE) - (INDEX) - 1)


// ===========================================================================

// Get an index into a value whose address has been cast to a byte pointer.
//
// INDEX_HB() gets the index starting at the "high byte" of the value going
// downwards, and INDEX_LB() starts at the "low byte" going upwards.
//
// "High byte" and "low byte" are fabrications to give a consistent "C" view
// of things across architectures, such that "high byte" is the first byte on
// a big endian machine, and the last byte on a little endian machine. These
// are most useful when you want to read or write to portions of a value in
// memory in a consistent manner regardless of host endianness.
//
// The following code will always print "12, 34", regardless
// of the endianness of the system it runs on:
//
//     uint16_t value = 0x1234;
//     uint8_t* ptr = (uint8_t*)&value;
//     printf("%02x, ", ptr[INDEX_HB(sizeof(value), 0)]);
//     printf("%02x\n", ptr[INDEX_LB(sizeof(value), 0)]);
#if BYTE_ORDER_IS_BIG_ENDIAN
#define INDEX_HB(TOTAL_SIZE, INDEX) INDEX_BE(TOTAL_SIZE, INDEX)
#define INDEX_LB(TOTAL_SIZE, INDEX) INDEX_LE(TOTAL_SIZE, INDEX)
#else
#define INDEX_HB(TOTAL_SIZE, INDEX) INDEX_LE(TOTAL_SIZE, INDEX)
#define INDEX_LB(TOTAL_SIZE, INDEX) INDEX_BE(TOTAL_SIZE, INDEX)
#endif


// ===========================================================================

// Code generators for reading and writing multibyte values in
// big and little endian order.

#include <stdint.h>

// Generate a reader function for TYPE, of the specified endianness.
// The function will be named read_[NAME]
// Endianness is BE or LE
#define DEFINE_ENDIAN_READ_FUNCTION(NAME, TYPE, ENDIAN) \
static inline TYPE read_ ## NAME(uint8_t* src) \
{ \
    TYPE value = 0; \
    uint8_t* dst = (uint8_t*)&value; \
    for(unsigned i = 0; i < sizeof(value); i++) \
    { \
        dst[INDEX_HB(sizeof(value), i)] = src[INDEX_ ## ENDIAN(sizeof(value), i)]; \
    } \
    return value; \
}

// Generate a writer function for TYPE, of the specified endianness.
// The function will be named write_[NAME]
// Endianness is BE or LE
#define DEFINE_ENDIAN_WRITE_FUNCTION(NAME, TYPE, ENDIAN) \
static inline void write_ ## NAME(TYPE value, uint8_t* dst) \
{ \
    uint8_t* src = (uint8_t*)&value; \
    for(unsigned i = 0; i < sizeof(value); i++) \
    { \
        dst[INDEX_ ## ENDIAN(sizeof(value), i)] = src[INDEX_HB(sizeof(value), i)]; \
    } \
}

// Generate a reader and writer function for TYPE, of the specified endianness.
// The functions will be named read_[NAME] and write_[NAME]
// Endianness is BE or LE
#define DEFINE_ENDIAN_READ_WRITE_FUNCTIONS(NAME, TYPE, ENDIAN) \
DEFINE_ENDIAN_READ_FUNCTION(NAME, TYPE, ENDIAN) \
DEFINE_ENDIAN_WRITE_FUNCTION(NAME, TYPE, ENDIAN)

// Generate reader and writer functions for TYPE, for both endianness.
// The functions will be named read_[NAME_BASE]_be, read_[NAME_BASE]_le,
// write_[NAME_BASE]_le, write_[NAME_BASE]_be
#define DEFINE_ENDIAN_READ_WRITE_FUNCTION_SET(NAME_BASE, TYPE) \
DEFINE_ENDIAN_READ_WRITE_FUNCTIONS(NAME_BASE ## _le, TYPE, LE) \
DEFINE_ENDIAN_READ_WRITE_FUNCTIONS(NAME_BASE ## _be, TYPE, BE)


// ===========================================================================

// Default integer and float read/write functions

DEFINE_ENDIAN_READ_WRITE_FUNCTION_SET(int16, int16_t)
DEFINE_ENDIAN_READ_WRITE_FUNCTION_SET(uint16, uint16_t)
DEFINE_ENDIAN_READ_WRITE_FUNCTION_SET(int32, int32_t)
DEFINE_ENDIAN_READ_WRITE_FUNCTION_SET(uint32, uint32_t)
DEFINE_ENDIAN_READ_WRITE_FUNCTION_SET(int64, int64_t)
DEFINE_ENDIAN_READ_WRITE_FUNCTION_SET(uint64, uint64_t)

// Float sizes can only be guaranteed if __STDC_IEC_559__ is set.
#include <math.h>
#ifdef __STDC_IEC_559__
    DEFINE_ENDIAN_READ_WRITE_FUNCTION_SET(float32, float)
    DEFINE_ENDIAN_READ_WRITE_FUNCTION_SET(float64, double)
#endif


#endif
