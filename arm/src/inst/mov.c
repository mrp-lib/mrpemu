#include "arm.h"

int32 arm_inst_mov(cpu_state_t *st, uint32 inst)
{
	loginst("mov", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 s = (inst >> 20) & 0b0001;
	uint32 rd = (inst >> 12) & 0b1111;
	uint32 operand;
	bool carry = shifter_operand(st, inst, &operand);

	st->registers[rd] = operand;

	//TODO 这里暂且不处理
	if (s == 1 && rd == r_pc)
	{
	}
	else if (s == 1)
	{
		st->n = operand >> 31;
		st->z = operand == 0;
		st->c = carry;
	}

	return EXEC_SUCCESS;
}
