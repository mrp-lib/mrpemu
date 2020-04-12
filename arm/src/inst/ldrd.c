#include "arm.h"

int32 arm_inst_ldrd(cpu_state_t *st, uint32 inst)
{
	//TODO 内存访问校验

	//p为0时w不能为1
	if ((inst & 0x01200000) == 0x00200000)
		return EXEC_UNPREDICTABLE;

	uint32 rn = (inst >> 16) & 0x000f;
	uint32 rd = (inst >> 12) & 0x000f;
	uint32 addr = addr_mode_3(st, inst);

	//要求rd是偶数，并且不能是r14
	if (even(rd) && rd != 14 && (addr & 0b0011) == 0b0000)
	{
		st->registers[rd] = mem_ld32(st->mem, addr);
		println("addr+4 : 0x%08x", mem_ld32(st->mem, addr + 4));
		st->registers[rd + 1] = mem_ld32(st->mem, addr + 4);
	}
	else
		return EXEC_UNPREDICTABLE;

	return EXEC_SUCCESS;
}