#include "arm.h"

int32 arm_inst_sel(cpu_state_t *st, uint32 inst)
{
	loginst("sel", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbo
	if (inst_b4(8) != 0xf)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	uint32 result = 0;

	result |= (bit1(st->cpsr.ge, 0) ? bitm(regv(rn), 0, 7) : bitm(regv(rm), 0, 7));
	result |= (bit1(st->cpsr.ge, 1) ? bitm(regv(rn), 8, 15) : bitm(regv(rm), 8, 15));
	result |= (bit1(st->cpsr.ge, 2) ? bitm(regv(rn), 16, 23) : bitm(regv(rm), 16, 23));
	result |= (bit1(st->cpsr.ge, 3) ? bitm(regv(rn), 24, 31) : bitm(regv(rm), 24, 31));

	regv(rd) = result;

	return EXEC_SUCCESS;
}
