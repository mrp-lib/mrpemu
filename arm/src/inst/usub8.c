#include "arm.h"

int32 arm_inst_usub8(cpu_state_t *st, uint32 inst)
{
	loginst("usub8", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbo
	if (inst_b4(8) != 0xf)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	uint32 ge;

	uint32 res0 = (bitm(regv(rn), 0, 7) - bitm(regv(rm), 0, 7));
	ge |= !borrow8(res0);
	uint32 res8 = bitm(regv(rn), 8, 15) - bitm(regv(rm), 8, 15);
	ge |= !borrow8(res8) << 1;
	uint32 res16 = bitm(regv(rn), 16, 23) - bitm(regv(rm), 16, 23);
	ge |= !borrow8(res16) << 2;
	uint32 res24 = bitm(regv(rn), 24, 31) - bitm(regv(rm), 24, 31);
	ge |= !borrow8(res24) << 3;

	regv(rd) = (res0 & 0x00ff) | ((res8 << 8) & 0x00ff) | ((res16 << 16) & 0x00ff) | ((res24 << 24) & 0x00ff);

	return EXEC_SUCCESS;
}
