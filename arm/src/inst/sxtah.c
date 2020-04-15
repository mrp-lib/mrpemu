#include "arm.h"

int32 arm_inst_sxtah(cpu_state_t *st, uint32 inst)
{
	loginst("sxtah", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_bm(8, 9) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);
	uint32 rotate = inst_bm(10, 11);

	uint32 operand = ror(regv(rm), rotate << 3);
	regv(rd) = regv(rn) + sign_extend(operand, 16);

	return EXEC_SUCCESS;
}
