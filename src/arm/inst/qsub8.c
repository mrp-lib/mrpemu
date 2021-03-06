#include "arm.h"

int32 arm_inst_qsub8(cpu_state_t *st, uint32 inst)
{
	loginst("qsub8", inst);

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
	int16 resLL = (int16)(int8)bitm(regv(rn), 0, 7) - (int8)bitm(regv(rm), 0, 7);
	uint8 LL = signed_sat(resLL, 8, &issat);
	int16 resLH = (int16)(int8)bitm(regv(rn), 8, 15) - (int8)bitm(regv(rm), 8, 15);
	uint8 LH = signed_sat(resLH, 8, &issat);
	//高位结果
	int16 resHL = (int16)(int8)bitm(regv(rn), 16, 23) - (int8)bitm(regv(rm), 16, 23);
	uint8 HL = signed_sat(resHL, 8, &issat);
	int16 resHH = (int16)(int8)bitm(regv(rn), 24, 31) - (int8)bitm(regv(rm), 24, 31);
	uint8 HH = signed_sat(resHH, 8, &issat);

	//结果综合
	regv(rd) = (HH << 24) | (HL << 16) | (LH << 8) | LL;

	return EXEC_SUCCESS;
}
