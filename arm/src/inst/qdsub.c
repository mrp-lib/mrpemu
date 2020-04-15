#include "arm.h"

int32 arm_inst_qdsub(cpu_state_t *st, uint32 inst)
{
	loginst("qdsub", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_b4(8) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	//pc
	if (rn == r_pc || rd == r_pc || rm == r_pc)
		return EXEC_UNPREDICTABLE;

	bool issat1, issat2;

	int64 _rn2 = regv(rn) * 2;
	int32 rn2 = signed_sat(_rn2, 32, &issat1);

	int64 _res = regv(rm) - rn2;
	regv(rd) = signed_sat(_res, 32, &issat2);

	if (issat1 || issat2)
	{
		st->cpsr.q = true;
	}

	return EXEC_SUCCESS;
}
