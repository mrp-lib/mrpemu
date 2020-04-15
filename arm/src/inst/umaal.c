#include "arm.h"

int32 arm_inst_umaal(cpu_state_t *st, uint32 inst)
{
	loginst("umaal", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rdh = inst_b4(16);
	uint32 rdl = inst_b4(12);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);

	uint64 result = (uint64)regv(rm) * (uint64)regv(rs) + (uint64)regv(rdl) + (uint64)regv(rdh);
	regv(rdl) = result & 0x00000000ffffffff;
	regv(rdh) = result >> 32;

	return EXEC_SUCCESS;
}
