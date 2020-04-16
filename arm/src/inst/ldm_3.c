#include "arm.h"

int32 arm_inst_ldm_3(cpu_state_t *st, uint32 inst)
{
	loginst("ldm3", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//rn不能是pc
	ldm_rn_pc_check();

	//TODO 内存权限检测

	//计算地址
	uint32 start_address, end_address;
	addr_mode_4(st, inst, &start_address, &end_address);

	uint32 address = start_address;
	for (uint32 i = 0; i <= 14; i++)
	{
		if (has_reg_m4(i))
		{
			st->registers[i] = mem_ld32(st->mem, address);
			address += 4;
		}
	}
	//TODO 如果有spsr，则拷贝到cpsr，否则出错

	//设置pc
	st->registers[r_pc] = mem_ld32(st->mem, address);

	return EXEC_SUCCESS;
}
