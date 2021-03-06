#include "arm.h"

int32 arm_inst_rev(cpu_state_t *st, uint32 inst)
{
	loginst("rev", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbo
	if (inst_b4(8) != 0xf || inst_b4(16) != 0xf)
		return EXEC_UNPREDICTABLE;

	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	//pc
	if (rd == r_pc || rm == r_pc)
		return EXEC_UNPREDICTABLE;

	regv(rd) = (bitm(regv(rm), 0, 7) << 24) |
			   (bitm(regv(rm), 8, 15) << 16) |
			   (bitm(regv(rm), 16, 23) << 8) |
			   bitm(regv(rm), 24, 31);

	return EXEC_SUCCESS;
}
