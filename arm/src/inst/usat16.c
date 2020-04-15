#include "arm.h"

int32 arm_inst_usat16(cpu_state_t *st, uint32 inst)
{
	loginst("usat16", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbo
	if (inst_b4(8) != 0xf)
		return EXEC_UNPREDICTABLE;

	uint32 sat_imm = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	bool issat1, issat2;
	uint16 rdl = unsigned_sat(bitm(regv(rm), 0, 15), sat_imm, &issat1);
	uint16 rdh = unsigned_sat(bitm(regv(rm), 16, 31), sat_imm, &issat2);

	regv(rd) = (rdh << 16) | rdl;

	if (issat1 || issat2)
		st->cpsr.q = 1;

	return EXEC_SUCCESS;
}
