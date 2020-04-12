#include "arm.h"

//adc指令
int32 arm_inst_bic(cpu_state_t *st, uint32 inst)
{
	loginst("bic", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//必要值
	uint32 s = (inst >> 20) & 0x0001;
	uint32 rn = (inst >> 16) & 0x000f;
	uint32 rd = (inst >> 12) & 0x000f;
	uint32 operand;
	bool carry = shifter_operand(st, inst, &operand);
	//计算
	uint32 result = st->registers[rd] = st->registers[rn] & (~operand);
	if (s == 1 && rd == 15)
	{
		//TODO 这种情况暂时不处理
	}
	else if (s == 1)
	{
		st->n = result >> 31;
		st->z = result == 0;
		st->c = carry;
	}
	return EXEC_SUCCESS;
}
