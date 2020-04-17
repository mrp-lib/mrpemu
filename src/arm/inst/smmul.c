#include "arm.h"

int32 arm_inst_smmul(cpu_state_t *st, uint32 inst)
{
	loginst("smmul", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rd = inst_b4(16);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);
	uint32 r = inst_b1(5);

	uint64 value;
	if (r == 1)
		value = (int64)(int32)regv(rm) * (int32)regv(rs) + 0x80000000;
	else
		value = (int64)(int32)regv(rm) * (int32)regv(rs);

	regv(rd) = value >> 32;

	return EXEC_SUCCESS;
}
