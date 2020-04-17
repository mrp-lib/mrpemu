#include "arm.h"

int32 arm_inst_smmls(cpu_state_t *st, uint32 inst)
{
	loginst("smmls", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rd = inst_b4(16);
	uint32 rn = inst_b4(12);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);
	uint32 r = inst_b1(5);

	int64 value = (int64)(int32)regv(rm) * (int32)regv(rs);
	if (r == 1)
		regv(rd) = (((int64)(int32)regv(rn) << 32) - value + 0x80000000) >> 32;
	else
		regv(rd) = (((int64)(int32)regv(rn) << 32) - value) >> 32;

	return EXEC_SUCCESS;
}
