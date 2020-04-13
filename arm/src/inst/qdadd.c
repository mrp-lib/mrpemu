#include "arm.h"

int32 arm_inst_qdadd(cpu_state_t *st, uint32 inst)
{
	loginst("qdadd", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_b4(8) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	//pc
	if (rn == r_pc || rd == r_pc || rd == r_pc)
		return EXEC_UNPREDICTABLE;

	bool issat1, issat2;

	int64 _rn2 = regvl(rn) * 2;
	int32 rn2 = signed_sat(_rn2, int_min(32), int_max(32), &issat1);

	int64 _res = regvl(rm) + rn2;
	int32 res = signed_sat(_res, int_min(32), int_max(32), &issat2);

	regvs(rd, res);

	if (issat1 || issat2)
	{
		st->cpsr.q = true;
	}

	return EXEC_SUCCESS;
}
