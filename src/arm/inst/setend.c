#include "arm.h"

int32 arm_inst_setend(cpu_state_t *st, uint32 inst)
{
	loginst("setend", inst);

	//sbz
	if (inst_bm(10, 15) != 0 || inst_b1(8) != 0 || inst_b4(0) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 e = inst_b1(9);

	st->cpsr.e = e;

	return EXEC_SUCCESS;
}
