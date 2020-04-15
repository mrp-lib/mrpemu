#include "arm.h"

int32 arm_inst_swi(cpu_state_t *st, uint32 inst)
{
	loginst("swi", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//这里不按照官方文档实现了，直接调用一下具体的中断处理函数就算了

	uint32 num = inst_bm(0, 23); //中断号

	//直接调用一下中断处理程序算了
	on_sorftware_interrupt(st, num);

	return EXEC_SUCCESS;
}
