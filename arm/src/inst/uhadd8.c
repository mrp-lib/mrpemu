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

	uint32 res0 = (bitm(regv(rn), 0, 7) + bitm(regv(rm), 0, 7)) & 0x000001fe; //与上0x000001fe的目的是，去除第4-31位，在去除低字节的0位，这样，在后面就能减少移位操作了。
	uint32 res8 = (bitm(regv(rn), 8, 15) + bitm(regv(rm), 8, 15)) & 0x000001fe;
	uint32 res16 = (bitm(regv(rn), 16, 23) + bitm(regv(rm), 16, 23)) & 0x000001fe;
	uint32 res24 = (bitm(regv(rn), 24, 31) + bitm(regv(rm), 24, 31)) & 0x000001fe;

	// regv(rd) = (res0 >> 1) | ((res8 >> 1) << 8) | ((res16 >> 1) << 16) | ((res24 >> 1) << 24);
	regv(rd) = (res0 >> 1) | (res8 << 7) | (res16 << 15) | (res24 << 23);

	return EXEC_SUCCESS;
}
