#include "arm.h"

int32 arm_inst_strd(cpu_state_t *st, uint32 inst)
{
	loginst("strd", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//TODO 进行内存访问权限的检测

	uint32 address = addr_mode_3(st, inst);
	uint32 rd = inst_b4(12);

	println("strd: r%d ->%d", rd, address);

	if (even(regv(rd)) && rd != 14 && bitm(address, 0, 1) == 0b00)
	{
		mem_st32(st->mem, address, regv(rd));
		mem_st32(st->mem, address + 4, regv(rd + 1));
	}
	else
		return EXEC_UNPREDICTABLE;

	//TODO 进行物理内存地址处理

	return EXEC_SUCCESS;
}