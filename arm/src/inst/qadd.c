#include "arm.h"

int32 arm_inst_qadd(cpu_state_t *st, uint32 inst)
{
	loginst("qadd", inst);

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

	bool issat;

	int64 _res = regv(rm) + regv(rn);
	regv(rd) = signed_sat(_res, 32, &issat);

	if (issat)
	{
		st->cpsr.q = true;
	}

	return EXEC_SUCCESS;
}
