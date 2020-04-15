#include "arm.h"

int32 arm_inst_smlaw_y(cpu_state_t *st, uint32 inst)
{
	loginst("smlaw_y", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rd = inst_b4(16);
	uint32 rn = inst_b4(12);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);
	uint32 y = inst_b1(6);

	int64 operand = (y == 0) ? sign_extend(bitm(regv(rs), 0, 15), 16) : sign_extend(bitm(regv(rs), 16, 31), 16);
	int64 res_mul = (int64)regv(rm) * operand;
	uint32 res16 = bitm64(res_mul, 16, 47);

	uint32 result = regv(rd) = res16 + rn;

	if (overflow_add(result, res16, rn))
		st->cpsr.q = 1;

	return EXEC_SUCCESS;
}
