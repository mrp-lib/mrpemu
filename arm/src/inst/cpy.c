#include "arm.h"

int32 arm_inst_cpy(cpu_state_t *st, uint32 inst)
{
	loginst("cpy", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_b4(16) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	st->registers[rd] = st->registers[rm];

	return EXEC_SUCCESS;
}