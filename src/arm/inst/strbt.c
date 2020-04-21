#include "arm.h"

int32 arm_inst_strbt(cpu_state_t *st, uint32 inst)
{
	loginst("strbt", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//TODO 进行内存访问权限的检测

	uint32 address = addr_mode_2(st, inst);
	uint32 rd = inst_b4(12);

	println("strbt: r%d ->%d", rd, address);

	//这个和strb一样的吧？？

	mem_st8(st->mem, address, regv(rd) & 0x00ff);

	//TODO 同样，进行物理内存地址处理

	return EXEC_SUCCESS;
}
