#include "mrp.h"

void swi_mr_div(vm_info_t *vm)
{
	//原型： (int32 a, int32 b) -> int32
	int32 a = vmreg(0);
	int32 b = vmreg(1);
	logsysc("mr_div(a=%d, b=%d)", a, b);

	mr_ret(a / b);
}

void swi_mr_mod(vm_info_t *vm)
{
	//原型： (int32 a, int32 b) -> int32
	int32 a = vmreg(0);
	int32 b = vmreg(1);
	logsysc("mr_mod(a=%d, b=%d)", a, b);

	mr_ret(a % b);
}