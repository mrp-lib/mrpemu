#include "arm.h"

int32 arm_inst_mla(cpu_state_t *st, uint32 inst)
{
	loginst("mla", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rd = (inst >> 16) & 0b1111;
	uint32 rn = (inst >> 12) & 0b1111;
	uint32 rs = (inst >> 8) & 0b1111;
	uint32 rm = inst & 0b1111;

	//不能是pc
	if (rd == r_pc || rn == r_pc || rs == r_pc || rm == r_pc)
		return EXEC_UNPREDICTABLE;

	uint32 s = (inst >> 20) & 0b0001;

	uint32 result = st->registers[rd] = (uint32)(st->registers[rm] * st->registers[rs] + st->registers[rn]);
	if (s == 1)
	{
		st->n = result >> 31;
		st->z = result == 0;
	}

	return EXEC_SUCCESS;
}
