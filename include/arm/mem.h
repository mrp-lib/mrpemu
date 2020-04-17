#ifndef __ARM_MEM_H__
#define __ARM_MEM_H__

#include "common/type.h"

//内存定义
typedef struct
{
	//内存缓冲
	uint8 *buffer;
	//内存大小
	uint32 memSize;

} memory_t;

//创建内存
memory_t *mem_create(uint32 size);

//销毁内存
void mem_destory(memory_t *mem);

//获取一个32位的数据
uint32 mem_ld32(memory_t *mem, uint32 addr);

//获取一个16位的数据
uint16 mem_ld16(memory_t *mem, uint32 addr);

//获取一个8位的数据
uint8 mem_ld8(memory_t *mem, uint32 addr);

//存储一个32位的数据
void mem_st32(memory_t *mem, uint32 addr, uint32 value);

//存储一个16位的数据
void mem_st16(memory_t *mem, uint32 addr, uint16 value);

//存储一个8位的数据
void mem_st8(memory_t *mem, uint32 addr, uint8 value);

#endif