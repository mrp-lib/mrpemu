#include "arm.h"

int32 arm_inst_eor(cpu_state_t *st, uint32 inst)
{
	loginst("eor", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//必要值
	uint32 s = inst_b1(20);
	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	bool carry;
	uint32 operand = addr_mode_1(st, inst, &carry);
	//计算
	uint32 result = st->registers[rd] = st->registers[rn] ^ operand;
	if (s == 1 && rd == 15)
	{
		//TODO 这种情况暂时不处理
	}
	else if (s == 1)
	{
		st->cpsr.n = result >> 31;
		st->cpsr.z = result == 0;
		st->cpsr.c = carry;
	}
	return EXEC_SUCCESS;
}