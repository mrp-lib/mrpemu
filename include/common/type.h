#ifndef __COMMON_TYPE_H__
#define __COMMON_TYPE_H__

#include <stdbool.h>

#ifndef __cplusplus
#define null ((void *)0)
#else
#define null 0
#endif

typedef long long int64;
typedef unsigned long long uint64;

typedef int int32;
typedef unsigned int uint32;

typedef short int16;
typedef unsigned short uint16;

typedef char int8;
typedef unsigned char uint8;

//虚拟内存地址指针，这种指针指向的是虚拟机中的内存地址。
typedef uint32 vmpt;

//用于将一个虚拟内存地址指针转换为实际指针
#define vmpt_real(type, p) ((type *)((vm->cpu->mem->buffer) + (p)))

#endif