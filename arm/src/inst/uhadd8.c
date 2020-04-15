#include "arm.h"

int32 arm_inst_uhadd8(cpu_state_t *st, uint32 inst)
{
	loginst("uhadd8", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbo
	if (inst_b4(8) != 0xf)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	uint32 res0 = bitm(regv(rn), 0, 7) + bitm(regv(rm), 0, 7);
	uint32 res8 = bitm(regv(rn), 8, 15) + bitm(regv(rm), 8, 15);
	uint32 res16 = bitm(regv(rn), 16, 23) + bitm(regv(rm), 16, 23);
	uint32 res24 = bitm(regv(rn), 24, 31) + bitm(regv(rm), 24, 31);

	regv(rd) = (res0 >> 1) | ((res8 >> 1) << 8) | ((res16 >> 1) << 16) | ((res24 >> 1) << 24);

	return EXEC_SUCCESS;
}
