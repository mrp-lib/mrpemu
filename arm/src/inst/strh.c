#include "arm.h"

int32 arm_inst_strh(cpu_state_t *st, uint32 inst)
{
	loginst("strh", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//TODO 内存访问权限校验

	uint32 addr = addr_mode_3(st, inst);
	uint32 rd = inst_b4(12);

	//TODO 其他一些条件不处理了
	mem_st16(st->mem, addr, regv(rd));

	return EXEC_SUCCESS;
}
