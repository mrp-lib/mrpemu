#include "arm.h"

int32 arm_inst_smulw_y(cpu_state_t *st, uint32 inst)
{
	loginst("smulw_y", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_b4(12) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rd = inst_b4(16);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);
	uint32 y = inst_b1(6);

	int16 operand = (y == 0) ? bitm(regv(rs), 0, 15) : bitm(regv(rs), 16, 31);

	uint64 mul_res = (int64)(int32)regv(rm) * operand;

	regv(rd) = bitm64(mul_res, 16, 47);

	return EXEC_SUCCESS;
}
