#include "arm.h"

int32 arm_inst_mov(cpu_state_t *st, uint32 inst)
{
	loginst("mov", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 s = inst_b1(20);
	uint32 rd = inst_b4(12);
	bool carry;
	uint32 operand = addr_mode_1(st, inst, &carry);

	st->registers[rd] = operand;

	//TODO 这里暂且不处理
	if (s == 1 && rd == r_pc)
	{
	}
	else if (s == 1)
	{
		st->cpsr.n = operand >> 31;
		st->cpsr.z = operand == 0;
		st->cpsr.c = carry;
	}

	return EXEC_SUCCESS;
}
