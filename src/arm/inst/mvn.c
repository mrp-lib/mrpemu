#include "arm.h"

int32 arm_inst_mvn(cpu_state_t *st, uint32 inst)
{
	loginst("mvn", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_b4(16) != 0)
		return EXEC_UNPREDICTABLE;

	bool carry;
	uint32 operand = addr_mode_1(st, inst, &carry);
	uint32 rd = inst_b4(12);
	uint32 s = inst_b1(20);

	uint32 result = st->registers[rd] = ~operand;
	if (s == 1 && rd == r_pc)
	{
		//TODO 这里不处理了
	}
	else if (s == 1)
	{
		st->cpsr.n = result >> 31;
		st->cpsr.z = result == 0;
		st->cpsr.c = carry;
	}

	return EXEC_SUCCESS;
}
