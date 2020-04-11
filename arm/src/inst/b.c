#include "arm.h"

int32 arm_inst_b(cpu_state_t *st, uint32 inst)
{
	uint32 l = (inst >> 24) & 0b0001;
	//保存一下pc值
	//如果是bl的话，需要保存下一条指令的地址
	if (l == 1)
		st->registers[r_lr] = st->registers[r_pc]; //在执行指令时，经过了取指令的过程，那时就已经指向了下一条，因此直接赋值即可
	//跳转
	int32 offset = (((int32)inst << 8) >> 8);
	st->registers[r_pc] += (offset << 2);
	//OK
	return EXEC_SUCCESS;
}