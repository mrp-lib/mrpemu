#include "arm.h"

int32 arm_inst_cmp(cpu_state_t *st, uint32 inst)
{
	loginst("cmp", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 sbz = (inst >> 12) & 0x000f;
	if (sbz != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rn = (inst >> 16) & 0x000f;
	uint32 operand;
	bool carry = shifter_operand(st, inst, &operand);

	uint32 result = st->registers[rn] - operand;
	st->n = result >> 31;
	st->z = result == 0;
	st->c = result > st->registers[rn]; //减完之后结果反而大于了原来的数
	st->v = (st->registers[rn] ^ operand) & (st->registers[rn] ^ result);

	return EXEC_SUCCESS;
}