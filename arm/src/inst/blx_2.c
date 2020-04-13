#include "arm.h"

int32 arm_inst_blx_2(cpu_state_t *st, uint32 inst)
{
	loginst("blx(2)", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//处理sbo1-3
	if (inst_bm(8, 19) != 0x0fff)
		return EXEC_UNPREDICTABLE;

	uint32 rm = inst_b4(0);
	//跳转
	uint32 target = st->registers[rm];
	st->registers[r_lr] = st->registers[r_pc]; //保存pc
	st->cpsr.t = target & 0x0001;			   //将target第0位设置到thumb标志
	st->registers[r_pc] = target & 0xFFFFFFFE; //跳转
	//OK
	return EXEC_SUCCESS;
}