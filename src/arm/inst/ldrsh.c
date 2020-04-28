#include "arm.h"

int32 arm_inst_ldrsh(cpu_state_t *st, uint32 inst)
{
	loginst("ldrsh", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//TODO 内存访问权限校验

	uint32 rd = inst_b4(12);
	if (rd == r_pc)
		return EXEC_UNPREDICTABLE;

	uint32 rn = inst_b4(16);
	uint32 addr = addr_mode_3(st, inst);

	//TODO 其他一些条件不处理了
	int16 data = mem_ld16(st->mem, addr);
	// st->registers[rd] = (uint32)sign_extend_e(data);
	st->registers[rd] = (int32)data;

	return EXEC_SUCCESS;
}
