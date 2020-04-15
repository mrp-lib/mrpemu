#include "arm.h"

int32 arm_inst_smlald(cpu_state_t *st, uint32 inst)
{
	loginst("smlald", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rdh = inst_b4(16);
	uint32 rdl = inst_b4(12);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);
	uint32 x = inst_b1(5);

	uint32 operand = (x == 1) ? ror(regv(rs), 16) : regv(rs);
	int64 accvalue = (((uint64)regv(rdh)) << 32) | regv(rdl);

	int32 product1 = (int16)bitm(regv(rm), 0, 15) * (int16)bitm(operand, 0, 15);
	int32 product2 = (int16)bitm(regv(rm), 16, 31) * (int16)bitm(operand, 16, 31);

	int64 result = accvalue + product1 + product2;

	regv(rdl) = result & 0x00000000ffffffff;
	regv(rdh) = result >> 32;

	return EXEC_SUCCESS;
}
