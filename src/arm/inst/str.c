#include "arm.h"

int32 arm_inst_str(cpu_state_t *st, uint32 inst)
{
	loginst("str", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//TODO 进行内存访问权限的检测

	uint32 address = addr_mode_2(st, inst);
	uint32 rd = inst_b4(12);

	println("str: %d ->r %d", rd, address);

	mem_st32(st->mem, address, regv(rd));

	//TODO 同样，进行物理内存地址处理

	return EXEC_SUCCESS;
}
