#include "arm.h"

int32 arm_inst_sub(cpu_state_t *st, uint32 inst)
{
	loginst("sub", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//必要值
	uint32 s = inst_b1(20);
	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 operand;
	bool carry = shifter_operand(st, inst, &operand);
	//计算
	uint32 result = regv(rd) = regv(rn) - operand;
	if (s == 1 && rd == 15)
	{
		//TODO 这种情况暂时不处理
	}
	else if (s == 1)
	{
		st->cpsr.n = result >> 31;
		st->cpsr.z = result == 0;
		st->cpsr.c = !borrow(result, regv(rn));
		st->cpsr.v = overflow_sub(result, regv(rn), operand);
	}
	return EXEC_SUCCESS;
}
