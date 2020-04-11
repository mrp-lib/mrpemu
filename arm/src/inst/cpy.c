#include "arm.h"

int32 arm_inst_cpy(cpu_state_t *st, uint32 inst)
{
	uint32 sbz = (inst >> 16) & 0x000f;
	if (sbz != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rd = (inst >> 12) & 0x000f;
	uint32 rm = inst & 0x000f;

	st->registers[rd] = st->registers[rm];

	return EXEC_SUCCESS;
}