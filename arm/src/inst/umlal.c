#include "arm.h"

int32 arm_inst_umlal(cpu_state_t *st, uint32 inst)
{
	loginst("umlal", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rdh = inst_b4(16);
	uint32 rdl = inst_b4(12);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);
	uint32 s = inst_b1(20);

	uint64 resl = (uint64)regv(rm) * (uint64)regv(rs) + (uint64)regv(rdl);
	regv(rdl) = resl & 0x00000000ffffffff;
	regv(rdh) = (resl >> 32) + regv(rdh);

	if (s == 1)
	{
		st->cpsr.n = regv(rdh) >> 31;
		st->cpsr.z = regv(rdh) == 0 && regv(rdl) == 0;
	}

	return EXEC_SUCCESS;
}
