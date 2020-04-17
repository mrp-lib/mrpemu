#include "arm.h"

int32 arm_inst_mul(cpu_state_t *st, uint32 inst)
{
	loginst("mul", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_b4(12) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 s = inst_b1(20);
	uint32 rd = inst_b4(16);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);

	//不能是r15
	if (rd == r_pc || rs == r_pc || rm == r_pc)
		return EXEC_UNPREDICTABLE;

	uint32 result = st->registers[rd] = (uint32)(st->registers[rm] * st->registers[rs]);

	if (s)
	{
		st->cpsr.n = result >> 31;
		st->cpsr.z == result == 0;
	}

	return EXEC_SUCCESS;
}
