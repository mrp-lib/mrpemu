#include "arm.h"

int32 arm_inst_ssub16(cpu_state_t *st, uint32 inst)
{
	loginst("ssub16", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbo
	if (inst_b4(8) != 0xf)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	int16 diff;
	uint32 result = 0x00000000;
	uint32 ge = 0x0;

	diff = (int16)bitm(regv(rn), 0, 15) - (int16)bitm(regv(rm), 0, 15);
	result |= ((uint16)diff);
	if (diff >= 0)
		ge |= 0b0011;

	diff = (int16)bitm(regv(rn), 16, 31) - (int16)bitm(regv(rm), 16, 31);
	result |= ((uint16)diff) << 16;
	if (diff >= 0)
		ge |= 0b1100;

	regv(rd) = result;
	st->cpsr.ge = ge;

	return EXEC_SUCCESS;
}
