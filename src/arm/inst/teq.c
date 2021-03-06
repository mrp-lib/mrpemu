#include "arm.h"

int32 arm_inst_teq(cpu_state_t *st, uint32 inst)
{
	loginst("teq", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_b4(12) != 0)
		return EXEC_UNPREDICTABLE;

	//必要值
	uint32 rn = inst_b4(16);
	bool carry;
	uint32 operand = addr_mode_1(st, inst, &carry);

	uint32 alu_out = regv(rn) ^ operand;

	st->cpsr.n = alu_out >> 31;
	st->cpsr.z = alu_out == 0;
	st->cpsr.c = carry;

	return EXEC_SUCCESS;
}
