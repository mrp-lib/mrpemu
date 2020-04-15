#include "arm.h"

int32 arm_inst_qadd8(cpu_state_t *st, uint32 inst)
{
	loginst("qadd8", inst);

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
	int64 resLL = bitm(regv(rm), 0, 7) + bitm(regv(rn), 0, 7);
	uint32 LL = signed_sat(resLL, 8, &issat);
	int64 resLH = bitm(regv(rm), 8, 15) + bitm(regv(rn), 8, 15);
	uint32 LH = signed_sat(resLH, 8, &issat);
	//高位结果
	int64 resHL = bitm(regv(rm), 16, 23) + bitm(regv(rn), 16, 23);
	uint32 HL = signed_sat(resHL, 8, &issat);
	int64 resHH = bitm(regv(rm), 24, 31) + bitm(regv(rn), 24, 31);
	uint32 HH = signed_sat(resHH, 8, &issat);

	//结果综合
	regv(rd) = (HH << 24) | (HL << 16) | (LH << 8) | LL;

	return EXEC_SUCCESS;
}
