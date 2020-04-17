#include "arm.h"

int32 arm_inst_uxtab16(cpu_state_t *st, uint32 inst)
{
	loginst("uxtab16", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_bm(8, 9) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);
	uint32 rotate = inst_bm(10, 11);

	uint32 operand = ror(regv(rm), rotate << 3) & 0x00ff00ff;
	uint16 resl = (uint16)bitm(regv(rn), 0, 15) + (uint16)bitm(operand, 0, 15);
	uint16 resh = (uint16)bitm(regv(rn), 16, 31) + (uint16)bitm(operand, 16, 31);

	regv(rd) = (resh << 16) | resl;

	return EXEC_SUCCESS;
}
