#include "arm.h"

int32 arm_inst_blx_2(cpu_state_t *st, uint32 inst)
{
	loginst("blx(2)", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//必要的值
	uint32 sbo1 = (inst >> 16) & 0x000f;
	uint32 sbo2 = (inst >> 12) & 0x000f;
	uint32 sbo3 = (inst >> 8) & 0x000f;
	uint32 rm = inst & 0x000f;
	//sbo应该是1
	if (sbo1 != 0x000f || sbo2 != 0x000f || sbo3 != 0x000f)
		return EXEC_UNPREDICTABLE;
	//跳转
	uint32 target = st->registers[rm];
	st->registers[r_lr] = st->registers[r_pc]; //保存pc
	st->t = target & 0x0001;				   //将target第0位设置到thumb标志
	st->registers[r_pc] = target & 0xFFFFFFFE; //跳转
	//OK
	return EXEC_SUCCESS;
}