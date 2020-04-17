#include "arm.h"

int32 arm_inst_b_bl(cpu_state_t *st, uint32 inst)
{
	uint32 l = inst_b1(24);

	loginst(l ? "bl" : "b", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//如果是bl的话，需要保存下一条指令的地址
	if (l == 1)
		st->registers[r_lr] = st->registers[r_pc]; //在执行指令时，经过了取指令的过程，那时就已经指向了下一条，因此直接赋值即可
	//跳转
	int32 offset = sign_extend(inst, 24);	  //扩展为有符号32位证书
	st->registers[r_pc] += (offset << 2) + 4; //注意需要+4
	//OK
	return EXEC_SUCCESS;
}