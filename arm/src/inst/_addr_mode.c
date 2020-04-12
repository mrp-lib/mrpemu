#include "arm.h"

//IPU

uint32 addr_mode_2(cpu_state_t *st, uint32 inst)
{
	uint32 I = (inst >> 25) & 0b0001;
	uint32 U = (inst >> 23) & 0b0001;
	uint32 rn = (inst >> 16) & 0x000f;

	//立即数偏移
	if (I == 0)
	{
		if (U == 1)
			return st->registers[rn] + (inst & 0x0fff);
		else
			return st->registers[rn] - (inst & 0x0fff);
	}
	//寄存器偏移
	else
	{
		uint32 rm = inst & 0x000f;
		uint32 B4_11 = (inst >> 4) & 0x00ff;
		//4-11位都是0
		if (B4_11 == 0)
		{
			if (U == 1)
				return st->registers[rn] + st->registers[rm];
			else
				return st->registers[rn] - st->registers[rm];
		}
		//带有计算的
		else
		{
			uint32 shift = (inst >> 5) & 0b0011; //第5-6位
			uint32 imm = (inst >> 7) & 0x001f;	 //7-11位
			//对于不同的shift进行不同的处理
			uint32 index;
			switch (shift)
			{
			case 0b00:
				index = lsl(st->registers[rm], imm);
				break;
			case 0b01:
				if (imm == 0)
					index = 0;
				else
					index = lsr(st->registers[rm], imm);
				break;
			case 0b10:
				if (imm == 0)
					index = (st->registers[rm] >> 31) ? 0xffffffff : 0;
				else
					index = asr(st->registers[rm], imm);
				break;
			case 0b11:
				if (imm == 0)
					index = lsl(st->c, 31) | lsr(st->registers[rm], 1);
				else
					index = ror(st->registers[rm], imm);
				break;
			}
			//得到结果
			if (U == 1)
				return st->registers[rn] + index;
			else
				return st->registers[rn] - index;
		}
	}
}

//cond 0 0 0 P U 1 W L Rn Rd immedH 1 S H 1 ImmedL
//cond 0 0 0 P U 0 W L Rn Rd SBZ    1 S H 1 Rm
uint32 addr_mode_3(cpu_state_t *st, uint32 inst)
{
	uint32 b22 = (inst >> 22) & 0x0001;
	uint32 p = (inst >> 24) & 0x0001;
	uint32 u = (inst >> 23) & 0x0001;
	uint32 w = (inst >> 21) & 0x0001;
	uint32 rn = (inst >> 16) & 0x000f;

	uint32 addr;

	//立即数偏移
	if (b22 == 1)
	{
		uint32 immh = (inst >> 8) & 0x000f;
		uint32 imml = inst & 0x000f;
		uint32 imm = (immh << 4) | immh;
		if (p == 1)
		{
			addr = (u == 1) ? st->registers[rn] + imm : st->registers[rn] - imm;
			if (w == 1)
				st->registers[rn] = addr;
		}
		else
		{
			if (w == 0)
			{
				addr = st->registers[rn];
				st->registers[rn] = (u == 1) ? st->registers[rn] + imm : st->registers[rn] - imm;
			}
			//else ERROR
		}
	}
	//寄存器偏移
	else
	{
		uint32 rm = inst & 0x000f;
		if (p == 1)
		{
			addr = (u == 1) ? st->registers[rn] + st->registers[rm] : st->registers[rn] - st->registers[rm];
			if (w == 1)
				st->registers[rn] = addr;
		}
		else
		{
			if (w == 0)
			{
				addr = st->registers[rn];
				st->registers[rn] = (u == 1) ? st->registers[rn] + st->registers[rm] : st->registers[rn] - st->registers[rm];
			}
			// else ERROR
		}
	}
	return addr;
}