#include "arm.h"

int32 arm_inst_swp(cpu_state_t *st, uint32 inst)
{
	loginst("swp", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_b4(8) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	//TODO 这里先直接交换吧，其他的以后再说
	uint32 tmp = mem_ld32(st->mem, regv(rn));
	mem_st32(st->mem, regv(rn), regv(rm));
	regv(rd) = tmp;

	return EXEC_SUCCESS;
}
