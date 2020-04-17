#include "arm.h"

int32 arm_inst_mrs(cpu_state_t *st, uint32 inst)
{
	loginst("mrs", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rd = inst_b4(12);
	uint32 r = inst_b1(22);

	if (r == 1)
	{
		//TODO 目前没有spsr，暂不处理
	}
	else
	{
		//取出cpsr
		st->registers[rd] = psr_ldval(&st->cpsr);
	}

	return EXEC_SUCCESS;
}
