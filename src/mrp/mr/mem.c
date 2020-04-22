#include <string.h>

#include "mrp.h"
#include "common/tools.h"

void swi_mr_malloc(vm_info_t *vm)
{
	//函数原型： (uint32 len) -> void*
	//取参数
	uint32 len = vmreg(0);
	logsysc("mr_malloc(len=%d)", len);

	//分配内存
	vmpt addr = mem_malloc(vm, len);

	//返回内存地址
	mr_ret(addr);
}

void swi_mr_free(vm_info_t *vm)
{
	//函数原型： (void* p, uint32 len) -> void
	//取参数
	vmpt p = vmreg(0);
	uint32 len = vmreg(1);
	logsysc("mr_free(p=0x%08x, len=%d)", p, len);

	mem_free(vm, p, len);
}

void swi_mr_realloc(vm_info_t *vm)
{
	//函数原型： (void* p, uint32 oldlen, uint32 len) -> void*
	//取参数
	vmpt p = vmreg(0);
	uint32 oldlen = vmreg(1);
	uint32 len = vmreg(2);
	logsysc("mr_realloc(p=0x%08x, oldlen=%d, len=%d)", p, oldlen, len);

	unsigned long minsize = (oldlen > len) ? len : oldlen;
	//如果没有分配过则直接分配
	if (p == 0)
	{
		mr_ret(mem_malloc(vm, len));
		return;
	}
	//如果新的长度为0，则直接释放
	if (len == 0)
	{
		mem_free(vm, p, oldlen);
		mr_ret(0); //返回空
		return;
	}
	//否则重新分配
	vmpt addr = mem_malloc(vm, len);
	if (addr == 0)
	{
		mr_ret(0);
		return;
	}

	memmove(vmpt_real(void, addr), vmpt_real(void, p), min(oldlen, len)); //拷贝内存
	mem_free(vm, p, oldlen);											  //释放旧的内存

	//返回
	mr_ret(addr);
}