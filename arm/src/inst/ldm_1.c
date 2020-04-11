#include "arm.h"

int32 arm_inst_ldm_1(cpu_state_t *st, uint32 inst)
{
	//TODO 内存权限检测，这里暂时不处理
	uint32 pu = (inst >> 23) & 0x00011; //P位和U位合在一起讨论
	uint32 w = (inst >> 21) & 0x0001;
	uint32 rn = (inst >> 16) & 0x000f;
	uint32 regs = inst & 0x00ff;

	if (rn == 15)
		return EXEC_UNPREDICTABLE;

	//得到基地址
	uint32 addr = (rn >> 2) << 2; //地址对齐
	switch (pu)
	{
	case 0b0000: //da
		for (uint32 i = 0; i < 14; i++)
		{
			if ((regs >> (14 - i)) & 0x0001)
			{

				st->registers[14 - i] = st->memory[addr];
				addr -= 4;
			}
		}
		break;
	case 0b0001: //ia
		for (uint32 i = 0; i < 14; i++)
		{
			if ((regs >> i) & 0x0001)
			{

				st->registers[i] = st->memory[addr];
				addr += 4;
			}
		}
		break;
	case 0b0010: //db
		for (uint32 i = 0; i < 14; i++)
		{
			if ((regs >> (14 - i)) & 0x0001)
			{

				addr -= 4;
				st->registers[14 - i] = st->memory[addr];
			}
		}
		break;
	case 0b0011: //ib
		for (uint32 i = 0; i < 14; i++)
		{
			if ((regs >> i) & 0x0001)
			{

				st->registers[i] = st->memory[addr];
				addr += 4;
			}
		}
		break;
	}
	//如果寄存器列表有R15(pc)
	if (regs >> 15)
	{
		//取内存值
		if (pu == 0b0010)
			addr -= 4;
		else if (pu == 0b0011)
			addr += 4;
		uint32 value = st->memory[addr];
		if (pu == 0b0000)
			addr -= 4;
		else if (pu == 0b0001)
			addr += 4;
		//对值进行处理
		//不适用这种方式
		// st->registers[r_pc] = value & 0xFFFFFFFE;
		// st->t = value & 0x0001;
		//否则呢
		st->registers[r_pc] = value & 0xFFFFFFFC;
	}
	//回写
	if (w == 1)
		st->registers[rn] = addr;

	//完成
	return EXEC_SUCCESS;
}