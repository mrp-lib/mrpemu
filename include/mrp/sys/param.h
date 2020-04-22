#ifndef __MRP_SYS_PARAM_H__
#define __MRP_SYS_PARAM_H__

#include "mrp/vm.h"

/*
获取函数参数
这个函数只能获取32位的参数，如果参数长度超过了32位，则需要调用多次这个函数来获取，并把获取到的结果进行合并，例如：
uint64 p1 = ((uint64)mr_get_param(vm, 0)<<32) | mr_get_param(vm, 1);

参数：
	vm		虚拟机
	n		第几个参数，根据参数的长度会有不同
*/
inline uint32 mr_get_param(vm_info_t *vm, uint32 n)
{
	//4个参数内是通过寄存器传参的
	if (n <= 3)
		return vm->cpu->registers[n];
	/*
	超过4个参数是通过栈来传参的
		栈的增长是从高地址往低地址增长的，而且参数是倒序压栈的，所以前面的参数会在低地址区域，后面的参数会在高地址区域。
		对于32位及以下的数据类型（例如：char short int long bool等）均按照一个字进行保存
		对于32位以上的数据类型，则会占用多个字来保存
	*/
	return vm->mem->stack[vm->cpu->registers[r_sp] + n];
}

#endif