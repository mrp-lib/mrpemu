#include "arm.h"

int32 arm_inst_sxtab16(cpu_state_t *st, uint32 inst)
{
	loginst("sxtab16", inst);

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
	uint32 resl = bitm(regv(rn), 0, 15) + sign_extend(bitm(operand, 0, 7), 8);
	uint32 resh = bitm(regv(rn), 16, 31) + sign_extend(bitm(operand, 16, 23), 8);

	regv(rd) = (resh << 16) | resl;

	return EXEC_SUCCESS;
}
