#include "arm.h"

int32 arm_inst_ssub8(cpu_state_t *st, uint32 inst)
{
	loginst("ssub8", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbo
	if (inst_b4(8) != 0xf)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	int8 diff;
	uint32 result = 0x00000000;
	uint32 ge = 0x0;

	diff = (int8)bitm(regv(rn), 0, 7) - (int8)bitm(regv(rm), 0, 7);
	result |= ((uint8)diff);
	if (diff >= 0)
		ge |= 0b0001;

	diff = (int8)bitm(regv(rn), 8, 15) - (int8)bitm(regv(rm), 8, 15);
	result |= ((uint8)diff) << 8;
	if (diff >= 0)
		ge |= 0b0010;

	diff = (int8)bitm(regv(rn), 16, 23) - (int8)bitm(regv(rm), 16, 23);
	result |= ((uint8)diff) << 16;
	if (diff >= 0)
		ge |= 0b0100;

	diff = (int8)bitm(regv(rn), 24, 31) - (int8)bitm(regv(rm), 24, 31);
	result |= ((uint8)diff) << 24;
	if (diff >= 0)
		ge |= 0b1000;

	regv(rd) = result;
	st->cpsr.ge = ge;

	return EXEC_SUCCESS;
}
