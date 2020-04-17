#include "arm.h"

int32 arm_inst_orr(cpu_state_t *st, uint32 inst)
{
	loginst("orr", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 operand;
	bool carry = shifter_operand(st, inst, &operand);
	uint32 s = inst_b1(20);
	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);

	uint32 result = st->registers[rd] = st->registers[rn] | operand;
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
