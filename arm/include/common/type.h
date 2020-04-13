#ifndef __COMMON_TYPE_H__
#define __COMMON_TYPE_H__

#include <stdbool.h>

#define null ((void *)0)

typedef long long int64;
typedef unsigned long long uint64;

typedef int int32;
typedef unsigned int uint32;

typedef short int16;
typedef unsigned short uint16;

typedef char int8;
typedef unsigned char uint8;


#define int32_min (-2147483648)
#define int32_max (2147483647)
#define uint32_max (2147483647)
#define int16_min (-32768)
#define int16_max (32767)
#define uint16_max (65535)
#define int8_min (-128)
#define int8_max (127)
#define uint8_max (255)
#define int_max(bits) int##bits##_max
#define int_min(bits) int##bits##_min
#define uint_max(bits) uint##bits##_max

#endif