#include "arm.h"

int32 arm_inst_smul_x_y(cpu_state_t *st, uint32 inst)
{
	loginst("smul_x_y", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_b4(12) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rd = inst_b4(16);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);
	uint32 x = inst_b1(5);
	uint32 y = inst_b1(6);

	int16 operand1 = (x == 0) ? sign_extend(bitm(rm, 0, 15), 16) : sign_extend(bitm(rm, 16, 31), 16);
	int16 operand2 = (y == 0) ? sign_extend(bitm(rs, 0, 15), 16) : sign_extend(bitm(rs, 16, 31), 16);

	regv(rd) = operand1 * operand2;

	return EXEC_SUCCESS;
}