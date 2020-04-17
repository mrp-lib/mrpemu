#include "arm.h"

int32 arm_inst_ldm_1(cpu_state_t *st, uint32 inst)
{
	loginst("ldm(1)", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//rn不能是pc
	ldm_rn_pc_check();

	//TODO 先进行内存检测

	//计算地址
	uint32 start_address, end_address;
	addr_mode_4(st, inst, &start_address, &end_address);

	uint32 address = start_address;
	for (uint32 i = 0; i <= 14; i++)
	{
		if (has_reg_m4(i))
		{
			st->registers[i] = mem_ld32(st->mem, address);
			address = address + 4;
		}
	}
	if (has_reg_m4(r_pc))
	{
		uint32 value = mem_ld32(st->mem, address);
		//对值进行处理
		//不使用这种方式
		// st->registers[r_pc] = value & 0xFFFFFFFE;
		// st->t = value & 0x0001;
		//否则呢
		st->registers[r_pc] = value & 0xFFFFFFFC;
	}

	return EXEC_SUCCESS;
}