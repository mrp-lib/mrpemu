#include <stdlib.h>
#include <string.h>

#include "arm.h"

#define mem_ld(len, addr) (*(len *)(mem->buffer + (addr)))
#define mem_st(len, addr, value) (*(len *)(mem->buffer + (addr)) = (value))

//创建内存
memory_t *mem_create(uint32 size)
{
	logi("mem_create(size=0x%08x)", size);

	uint8 *buffer = (uint8 *)malloc(size);
	if (buffer == null)
		return null;

	memory_t *mem = (memory_t *)malloc(sizeof(memory_t));
	if (mem == null)
	{
		free(buffer);
		return null;
	}

	memset(buffer, 0, size);
	memset(mem, 0, sizeof(memory_t));
	mem->buffer = buffer;
	mem->memSize = size;

	return mem;
}

//销毁内存
void mem_destory(memory_t *mem)
{
	logi("mem_destory(mem=%p)", mem);
	if (mem != null)
	{
		if (mem->buffer != null)
			free(mem->buffer);
		free(mem);
	}
}

//获取一个32位的数据
uint32 mem_ld32(memory_t *mem, uint32 addr)
{
	uint32 addr_align = (addr >> 2) << 2;	//对齐的地址(32位(4字节，所以除以4)对齐)
	uint32 addr_shift = (addr & 0b11) << 3; //需要左移的位数 = 偏移的地址*8 (?一个字节8位)
	//从该地址取出数据
	uint32 result = mem_ld(uint32, addr_align);
	//进行循环左移
	return ror(result, addr_shift);
}

//获取一个16位的数据
uint16 mem_ld16(memory_t *mem, uint32 addr)
{
	uint32 addr_align = (addr >> 1) << 1;	//对齐的地址(16位(2字节，所以除以2)对齐)
	uint32 addr_shift = (addr & 0b01) << 3; //需要左移的位数 = 偏移的地址*8 (?一个字节8位)
	//从该地址取出数据
	uint16 result = mem_ld(uint16, addr_align);
	//进行循环左移
	return ror16(result, addr_shift);
}

//获取一个8位的数据
uint8 mem_ld8(memory_t *mem, uint32 addr)
{
	return mem_ld(uint8, addr);
}

//存储一个32位的数据
void mem_st32(memory_t *mem, uint32 addr, uint32 value)
{
	mem_st(uint32, (addr >> 2) << 2, value);
}

//存储一个16位的数据
void mem_st16(memory_t *mem, uint32 addr, uint16 value)
{
	mem_st(uint16, (addr >> 1) << 1, value);
}

//存储一个8位的数据
void mem_st8(memory_t *mem, uint32 addr, uint8 value)
{
	mem_st(uint8, addr, value);
}