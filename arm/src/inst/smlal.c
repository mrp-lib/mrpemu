#include "arm.h"

int32 arm_inst_smlal(cpu_state_t *st, uint32 inst)
{
	loginst("smlal", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rdh = inst_b4(16);
	uint32 rdl = inst_b4(12);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);
	uint32 s = inst_b1(20);

	int64 res = (int64)(int32)regv(rs) * (int64)(int32)regv(rm) + (uint32)regv(rdl);
	uint32 resl = regv(rdl) = res;
	uint32 resh = regv(rdh) = (res >> 32) + (int32)regv(rdh);

	if (s == 1)
	{
		st->cpsr.n = resh >> 31;
		st->cpsr.z = (resl == 0) && (resh == 0);
	}

	return EXEC_SUCCESS;
}
