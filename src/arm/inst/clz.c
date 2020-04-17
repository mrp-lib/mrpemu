#include "arm.h"

int32 arm_inst_clz(cpu_state_t *st, uint32 inst)
{
	loginst("clz", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbo1-2
	if (inst_b4(8) != 0x0f || inst_b4(16) != 0xf)
		return EXEC_UNPREDICTABLE;

	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);
	//计算前导0数量
	if (st->registers[rm] == 0) //如果都是0的话，自然就是32咯
		st->registers[rd] = 32;
	else //否则的话就要逐个计算了
	{
		int count;
		uint32 rmv = st->registers[rm];
		for (count = 32; rmv > 0; count--)
			rmv = rmv >> 1;
		st->registers[rd] = count;
	}
	return EXEC_SUCCESS;
}
