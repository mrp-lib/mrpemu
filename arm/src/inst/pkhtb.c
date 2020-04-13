#include "arm.h"

int32 arm_inst_pkhtb(cpu_state_t *st, uint32 inst)
{
	loginst("pkhtb", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);
	uint32 shift_imm = inst_bm(7, 11);

	//pc
	if (rn == r_pc || rd == r_pc || rm == r_pc)
		return EXEC_UNPREDICTABLE;

	uint32 l;
	if (shift_imm == 0)
	{
		if (bit1(regvl(rm), 31) == 0)
			l = 0;
		else
			l = 0xffff;
	}
	else
		l = asr(regvl(rm), shift_imm) & 0x0000ffff;
	uint32 h = regvl(rn) & 0xffff0000;

	regvs(rd, h | l);

	return EXEC_SUCCESS;
}
