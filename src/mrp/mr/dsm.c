#include <string.h>
#include "mrp.h"

void swi_mr_c_function_new(vm_info_t *vm)
{
	//取得参数
	uint32 func = vm->cpu->registers[0]; //这时一个函数指针，类型:MR_C_FUNCTION
	uint32 len = vm->cpu->registers[1];
	logsysc("mr_c_function_new(func=0x%08x, len=%d)", func, len);

	//如果有的话先释放它
	if (mrst.mr_c_function_P && mrst.mr_c_function_P_Len)
	{
		mem_free(vm, mrst.mr_c_function_P, mrst.mr_c_function_P_Len);
		mrst.mr_c_function_P = mrst.mr_c_function_P_Len = 0;
	}

	//分配内存
	vmpt addr = mrst.mr_c_function_P = mem_malloc(vm, len);
	if (addr)
	{
		memset(vmpt_real(uint8, addr), 0, len);
		mrst.mr_c_function_P_Len = len;
		mrst.mr_c_function = func;
	}
	else
	{
		mrst.mr_state = 5;
		mr_ret(MR_FAILED);
		return;
	}

	//设置指针
	*vmpt_real(uint32, mrst.mr_c_function_load - 4) = addr;

	mr_ret(MR_SUCCESS);
}

void swi_mr_testcom(vm_info_t *vm)
{
	/*
	原型： int mr_TestCom(mrp_State* L, int input0, int input1)
	*/
	vmpt st = vmreg(0);
	uint32 input0 = vmreg(1);
	uint32 input1 = vmreg(2);
	logsysc("mr_testcom(state=0x%08x, input0=%d, input1=%d)", st, input0, input1);

	//这个直接调用系统的就可以
	mr_ret(mr_testCom(vm, st, input0, input1));
}

// void swi_mr_testcom1(vm_info_t *vm)
// {
// }