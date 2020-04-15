#include "arm.h"

int32 arm_inst_stm_2(cpu_state_t *st, uint32 inst)
{
	loginst("stm_1", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//TODO 内存访问检测
	uint32 start_address, end_address;
	addr_mode_4(st, inst, &start_address, &end_address);
	
	//TODO 这个就按照stm(1)来实现吧，目前不区分模式

	uint16 register_list = inst_b16(0);

	uint32 address = start_address;
	for (uint32 i = 0; i <= 15; i++)
	{
		if (has_reg_m4(i))
		{
			mem_st32(st->mem, address, regv(i));
			address += 4;
			//TODO 从ARMv6开始还会进行物理内存的检测
		}
	}

	return EXEC_SUCCESS;
}
