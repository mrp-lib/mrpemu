#include "mrp.h"

void swi_mr_malloc(vm_info_t *vm)
{
	//取参数
	uint32 len = vm->cpu->registers[0];
	logsysc("mr_malloc(len=%d)", len);

	//分配内存
	vmpt addr = mem_malloc(vm, len);

	//返回内存地址
	mr_ret(addr);
}