#include "arm.h"

int32 arm_inst_ldrh(cpu_state_t *st, uint32 inst)
{
	loginst("ldrh", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//TODO 内存访问权限校验

	uint32 rd = (inst >> 12) & 0x000f;
	if (rd == r_pc)
		return EXEC_UNPREDICTABLE;

	uint32 rn = (inst >> 16) & 0x000f;
	uint32 addr = addr_mode_3(st, inst);

	//TODO 其他一些条件不处理了，这里直接加载半字
	uint16 data = mem_ld16(st->mem, addr);
	st->registers[rd] = (uint32)data;

	return EXEC_SUCCESS;
}
