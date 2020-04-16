#include "arm.h"

int32 arm_inst_smlad(cpu_state_t *st, uint32 inst)
{
	loginst("smlad", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rd = inst_b4(16);
	uint32 rn = inst_b4(12);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);

	uint32 x = inst_b1(5);

	uint32 operand = (x == 1) ? ror(regv(rs), 16) : regv(rs);
	uint32 product1 = (int32)(int16)bitm(regv(rm), 0, 15) * (int16)bitm(operand, 0, 15);
	uint32 product2 = (int32)(int16)bitm(regv(rm), 16, 31) * (int16)bitm(operand, 16, 31);

	uint32 result = regv(rd) = regv(rn) + product1 + product2;

	if (overflow_add(result, regv(rn), product1))
		st->cpsr.q = 1;

	return EXEC_SUCCESS;
}