#include "arm.h"

int32 arm_inst_qadd16(cpu_state_t *st, uint32 inst)
{
	loginst("qadd16", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbo
	if (inst_b4(8) != 0xf)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);
	bool issat;

	//pc
	if (rn == r_pc || rd == r_pc || rm == r_pc)
		return EXEC_UNPREDICTABLE;

	//低位结果
	int32 resL = (int32)(int16)bitm(regv(rm), 0, 15) + (int16)bitm(regv(rn), 0, 15);
	uint16 L = signed_sat(resL, 16, &issat);
	//高位结果
	int32 resH = (int32)(int16)bitm(regv(rm), 16, 31) + (int16)bitm(regv(rn), 16, 31);
	uint16 H = signed_sat(resH, 16, &issat);

	//结果综合
	regv(rd) = (H << 16) | L;

	return EXEC_SUCCESS;
}
