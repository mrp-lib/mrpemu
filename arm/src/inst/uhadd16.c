#include "arm.h"

int32 arm_inst_uhadd16(cpu_state_t *st, uint32 inst)
{
	loginst("uhadd16", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbo
	if (inst_b4(8) != 0xf)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	uint32 res0 = (bitm(regv(rn), 0, 15) + bitm(regv(rm), 0, 15)) & 0x0001fffe; //与上0x0001fffe的目的是，去除高字节的第1-31位(需要保留0位)，在去除低字节的0位，这样，在后面就能减少移位操作了。
	uint32 res16 = (bitm(regv(rn), 16, 31) + bitm(regv(rm), 16, 31)) & 0x0001fffe;

	// regv(rd) = (res0 >> 1) | ((res16 >> 1) << 16);
	regv(rd) = (res0 >> 1) | (res16 << 15);

	return EXEC_SUCCESS;
}
