#include "mrp.h"
#include "common/tools.h"

//这个宏用来从某个地址处的空闲链表指针
#define p_llist(addr) ((mr_free_mem_t *)(vm_mem_buf + (addr)))
//这个宏用来取得内存状态
#define memst (mrst.mem_state)

void mem_init(vm_info_t *vm)
{
	logi("mem_init(vm=%p)", vm);

	uint32 base = memst.base = (VM_MEM_OFFSET + sizeof(vm_mem_map_t) + 3) & 0xfffffffc;
	uint32 len = memst.len = vm->cpu->mem->memSize & 0xfffffffc;
	memst.end = base + len;

	//将整个内存初始化成空闲块，作为第一个空闲块
	mr_free_mem_t *head = p_llist(base);
	head->next = len;
	head->size = len;

	//指向了第一个空闲块
	memst.free.size = 0;
	memst.free.next = 0;

	memst.left = len; //剩余量当然就是所有内存了
	memst.min = len;  //底值也是一样，开始是最大内存，随着分配将会逐渐减少
	memst.top = 0;	  //峰值的话与底值相反啦。
}

vmpt mem_malloc(vm_info_t *vm, uint32 len)
{
	logi("mem_malloc(vm=%p, len=%d)", vm, len);

	//先检测一下给定的参数是否正确
	if (len <= 0)
		return 0;

	//检测一下内存是否正确
	if (memst.base + memst.free.size > memst.end)
		return 0;

	len = max(len, MEM_MIN_PIECE_SIZE); //要求分配的内存不能小于最小碎片值，以免产生无法利用的碎片
	len = (len + 7) & 0xfffffff8;		//内存对齐

	//检测一下剩余空间是否足够
	if (len > memst.left)
		return 0;

	//查找内存链表，找到合适的内存块
	mr_free_mem_t *prev = &memst.free;							 //上一块
	mr_free_mem_t *next = p_llist(memst.base + memst.free.next); //下一块
	mr_free_mem_t *end = p_llist(memst.end);					 //结束位置

	//查找可用内存直到全部找完
	while (next < end)
	{
		//找到了合适的块
		if (next->size >= len)
		{
			memst.left -= len; //剩余内存减小
			//如果找到的空闲块比所需的块大得多了，那么对空闲块进行拆分
			if (next->size - len > MEM_MIN_PIECE_SIZE)
			{
				//处理截断后的内存
				mr_free_mem_t *cut = (mr_free_mem_t *)((uint8 *)next + len);
				cut->next = next->next;
				cut->size = next->size - len;
				//把截断后的前面一块从空闲链表中移除
				prev->next = vm_mem_offset(cut) - memst.base;
			}
			//大得不多的话，直接就全部给分配了。
			else
			{
				prev->next = next->next; //把当前块从空闲链表中去除
			}
			//计算一下峰值和底值
			memst.top = max(memst.top, prev->next);
			memst.min = min(memst.min, memst.left);

			//把空闲块返回出去
			return vm_mem_offset(next);
		}
		//继续
		prev = next;
		next = p_llist(memst.base + next->next);
	}

	//如果一直没有找到则分配失败
	return 0;
}

void mem_free(vm_info_t *vm, vmpt addr, uint32 len)
{
	logi("mem_free(vm=%p, addr=0x%08x, len=%d)", vm, addr, len);

	//检测参数是否正确
	if (len <= 0 || addr < memst.base || addr >= memst.end)
		return;

	len = max(len, MEM_MIN_PIECE_SIZE); //不能小于最小的碎片长度
	len = (len + 7) & 0xfffffff8;		//对齐一下

	//检测是否超过了内存空间
	if (addr + len > memst.end)
		return;

	//检测一下要释放的内存块的前面有没有空闲的内存块，因为如果前面一块如果空闲的话，就应该和当前释放掉的块合并成一块空闲块。
	mr_free_mem_t *free = &memst.free;
	mr_free_mem_t *next = p_llist(memst.base + free->next);
	mr_free_mem_t *curr = p_llist(addr); //要释放的块
	while (next < curr && next < curr)
	{
		free = next;
		next = p_llist(memst.base + next->next);
	}
	//如果当前要释放的内存块是一个空闲块则不处理
	if ((curr == free) || (curr == next))
		return;

	//如果前面一块正好和这块内存相邻则合并
	if ((free != &memst.free) && ((uint8 *)(free) + free->size == (uint8 *)curr))
	{
		free->size += len;
	}
	//如果不相邻的话则将当前块插入到链表中
	else
	{
		curr->next = free->next;
		curr->size = len;
		free->next = vm_mem_offset(curr) - memst.base;
	}
	//完成后再看看是否和后面一块相邻，如果相邻的话进行合并
	if ((free->next < memst.end) && (free->next == addr + len - memst.base))
	{
		curr->next = next->next;
		curr->size += next->size;
	}
	//释放后恢复长度
	memst.left += len;
}