#ifndef __MRP_SYS_MEM_H__
#define __MRP_SYS_MEM_H__

#include "mrp/common/helper.h"

#define MEM_MIN_PIECE_SIZE 16 //内存碎片的最小值，保证产生的碎片不会小于这个值，从而保证碎片可以再次利用。这里我就要求分配的内存大小不低于16字节吧。

/*空闲内存链表*/
typedef struct mr_free_mem_st
{
	uint32 size; //该段空闲内存的大小
	uint32 next; //下个节点位置，这个位置是相对于基地址的
} mr_free_mem_t;

/*内存状态*/
typedef struct
{
	uint32 base;		//内存基址
	uint32 len;			//内存长度
	uint32 end;			//内存终址
	uint32 left;		//剩余内存
	uint32 min;			//内存底址
	uint32 top;			//内存峰值
	mr_free_mem_t free; //空闲内存存链表，它指向的是空闲链表的开始节点
} mr_mem_state_t;

/*
内存初始化
参数：
	vm		虚拟机
*/
void mem_init(vm_info_t *vm);

/*
从虚拟机分配内存，得到相对位置
参数：
	vm		虚拟机
	len		所需长度
返回：
	>0		成功
	其他	失败
*/
vmpt mem_malloc(vm_info_t *vm, uint32 len);

/*
释放虚拟机中给定偏移处的一段内存
参数：
	vm		虚拟机
	addr	内存偏移地址
	len		要释放的内存长度
*/
void mem_free(vm_info_t *vm, vmpt addr, uint32 len);

#endif