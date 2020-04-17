#include "arm.h"

int32 arm_inst_cmn(cpu_state_t *st, uint32 inst)
{
	loginst("cmn", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_b4(12) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 operand;
	bool carry = shifter_operand(st, inst, &operand);

	uint32 result = regv(rn) + operand;
	st->cpsr.n = result >> 31;
	st->cpsr.z = result == 0;
	st->cpsr.c = carry(result, regv(rn));
	st->cpsr.v = overflow_add(result, regv(rn), operand);

	return EXEC_SUCCESS;
}