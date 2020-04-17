#include "arm.h"

int32 arm_inst_revsh(cpu_state_t *st, uint32 inst)
{
	loginst("revsh", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbo
	if (inst_b4(8) != 0xf || inst_b4(16) != 0xf)
		return EXEC_UNPREDICTABLE;

	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	//pc
	if (rd == r_pc || rm == r_pc)
		return EXEC_UNPREDICTABLE;

	uint32 l = (bitm(regv(rm), 0, 7) << 8) | bitm(regv(rm), 8, 15);
	uint32 h;
	if (bit1(regv(rm), 7) == 1)
		h = 0xffff0000; //这里赋值好避免移位
	else
		h = 0x00000000;

	regv(rd) = h | l;

	return EXEC_SUCCESS;
}
