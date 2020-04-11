#include "arm.h"

int32 arm_inst_clz(cpu_state_t *st, uint32 inst)
{
	uint32 sbo1 = (inst >> 16) & 0x000f;
	uint32 sbo2 = (inst >> 8) & 0x000f;
	if (sbo1 != 1 || sbo2 != 1)
		return EXEC_UNPREDICTABLE;

	uint32 rd = (inst >> 12) & 0x000f;
	uint32 rm = inst & 0x000f;
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
