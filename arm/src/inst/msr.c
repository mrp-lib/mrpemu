#include "arm.h"

// cond 0 0 1 1 0 R 1 0 field_mask SBO rotate_imm 8_bit_immediate
// cond 0 0 0 1 0 R 1 0 field_mask SBO SBZ 0 0 0 0 Rm

int32 arm_inst_msr(cpu_state_t *st, uint32 inst)
{
	loginst("msr", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//sbz
	if (inst_b4(12) != 0)
		return EXEC_UNPREDICTABLE;

	uint32 i = inst_b1(25); //立即数标识位
	uint32 r = inst_b1(22); //r位

	uint32 operand;

	if (i == 1)
	{
		uint32 imm8 = inst_b8(0);
		uint32 rotate_imm = inst_b4(8);
		operand = ror(imm8, rotate_imm << 1); // 8_bit_immediate Rotate_Right (rotate_imm * 2)
	}
	else
	{
		//sbz
		if (inst_b4(8) != 0xf)
			return EXEC_UNPREDICTABLE;
		operand = st->registers[inst_b4(0)]; //rm
	}

	uint32 field_mask = inst_b4(16);
	uint32 bm1 = (bit1(field_mask, 0) == 1) ? 0x000000ff : 0x00000000;
	uint32 bm2 = (bit1(field_mask, 1) == 1) ? 0x0000ff00 : 0x00000000;
	uint32 bm3 = (bit1(field_mask, 2) == 1) ? 0x00ff0000 : 0x00000000;
	uint32 bm4 = (bit1(field_mask, 3) == 1) ? 0xff000000 : 0x00000000;
	uint32 byte_mask = bm1 | bm2 | bm3 | bm4;

	//TODO 接下来的操作需要用到spsr，不过目前并没有，所以不处理了。
	if (true)
		return EXEC_UNPREDICTABLE;

	if (r == 0)
	{
		/*
		伪代码
		if InAPrivilegedMode() then
			if (operand AND StateMask) != 0 then
			UNPREDICTABLE // Attempt to set non-ARM execution state
			else
			mask = byte_mask AND (UserMask OR PrivMask)
		else
			mask = byte_mask AND UserMask
			CPSR = (CPSR AND NOT mask) OR (operand AND mask)
		*/
		//TOTO 这里不考虑特权模式
		// uint32 mask = byte_mask &
	}
	else
	{
		return EXEC_UNPREDICTABLE;
	}

	return EXEC_SUCCESS;
}
