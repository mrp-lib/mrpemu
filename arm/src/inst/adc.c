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
	uint32 operand;
	bool carry = shifter_operand(st, inst, &operand);
	//计算
	uint32 result = st->registers[rd] = st->registers[rn] + operand + st->cpsr.c;
	if (s == 1 && rd == 15)
	{
		//TODO 这种情况暂时不处理
	}
	else if (s == 1)
	{
		st->cpsr.n = result >> 31;
		st->cpsr.z = result == 0;
		st->cpsr.c = result < st->registers[rn]; //加了一个数之后反而小于原来的数，就进位了
		st->cpsr.v = (st->registers[rn] ^ operand ^ -1) & (st->registers[rn] ^ result);
	}
	return EXEC_SUCCESS;
}
