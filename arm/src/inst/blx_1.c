#include "arm.h"

int32 arm_inst_blx_1(cpu_state_t *st, uint32 inst)
{
	loginst("blx(1)", inst);

	uint32 h = (inst >> 24) & 0x00000001;
	//保存地址
	st->registers[r_lr] = st->registers[r_pc];
	//切换到thumb模式
	st->t = true;
	//跳转
	int32 offset = (((int32)inst << 8) >> 8);
	st->registers[r_pc] += (offset << 2) + (h << 1) + 4; //要+4
	//OK
	return EXEC_SUCCESS;
}