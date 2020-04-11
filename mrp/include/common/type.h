#ifndef __COMMON_TYPE_H__
#define __COMMON_TYPE_H__

#include <stdbool.h>

#define null ((void *)0)

#define MR_SUCCESS 0 //成功
#define MR_FAILED -1 //失败
#define MR_IGNORE 1	 //不关心
#define MR_WAITING 2 //异步(非阻塞)模式

typedef int int32;
typedef unsigned int uint32;

typedef short int16;
typedef unsigned short uint16;

typedef char int8;
typedef unsigned char uint8;

#endif