#include "arm.h"

int32 arm_inst_usat(cpu_state_t *st, uint32 inst)
{
	loginst("usat", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 sat_imm = inst_bm(16, 20);
	uint32 rd = inst_b4(12);
	uint32 shift_imm = inst_bm(7, 11);
	uint32 shift = inst_b1(6);
	uint32 rm = inst_b4(0);

	int64 operand;
	if (shift == 1)
	{
		if (shift_imm == 0)
			operand = regv(rm); //asr(regv(rm), 32);
		else
			operand = asr64(regv(rm), shift_imm);
	}
	else
		operand = lsl64(regv(rm), shift_imm);

	bool issat;
	regv(rd) = unsigned_sat(operand, sat_imm, &issat);

	if (issat)
		st->cpsr.q = 1;

	return EXEC_SUCCESS;
}
