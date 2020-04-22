#include "arm.h"

//adc指令
int32 arm_inst_adc(cpu_state_t *st, uint32 inst)
{
	loginst("adc", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//必要值
	uint32 s = inst_b1(20);
	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 operand = addr_mode_1(st, inst, null);
	//计算
	uint32 result = regv(rd) = regv(rn) + operand + st->cpsr.c;
	if (s == 1 && rd == 15)
	{
		//TODO 这种情况暂时不处理
	}
	else if (s == 1)
	{
		st->cpsr.n = result >> 31;
		st->cpsr.z = result == 0;
		st->cpsr.c = carry(result, regv(rn));
		st->cpsr.v = overflow_add(result, regv(rn), operand);
	}
	return EXEC_SUCCESS;
}
