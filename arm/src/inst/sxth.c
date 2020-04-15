#include "arm.h"

int32 arm_inst_sxth(cpu_state_t *st, uint32 inst)
{
	loginst("sxth", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_bm(8, 9) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);
	uint32 rotate = inst_bm(10, 11);

	uint32 operand = ror(regv(rm), rotate << 3);
	regv(rd) = sign_extend(operand, 16);

	return EXEC_SUCCESS;
}
