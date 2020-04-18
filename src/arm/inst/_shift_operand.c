#include "arm.h"

//获取shifter_operand
bool shifter_operand(cpu_state_t *st, uint32 inst, uint32 *operand)
{
	//这两个变量用来返回
	uint32 shifter_operand;
	bool shifter_carry_out;
	//取得指令的I位(25)
	uint32 I = inst_b1(25);
	uint32 rn = inst_b4(16);
	//如果I位为1表示立即数偏移
	if (I == 1)
	{
		uint32 immed8 = inst_b8(0);
		uint32 imm = inst_b4(8);
		shifter_operand = ror(immed8, imm * 2);
		if (imm == 0)
		{
			shifter_carry_out = st->cpsr.c;
		}
		else
		{
			shifter_carry_out = (shifter_operand >> 31) & 0b1;
		}
	}
	//否则表示寄存器偏移（需要保证不是扩展指令集）
	else
	{
		//取得Rm和shift
		uint32 Rm = st->registers[inst_b4(0)];
		uint32 shift = inst_bm(5, 6);
		//第4位为0，使用立即数
		if ((inst & 0b10000) == 0)
		{
			uint32 imm = inst_bm(7, 11);
			//A5-9
			if (shift == 0b00)
			{
				// A5-8、 A5-9
				if (imm == 0)
				{
					shifter_operand = Rm;
					shifter_carry_out = st->cpsr.c;
				}
				else
				{
					shifter_operand = lsl(Rm, imm);
					shifter_carry_out = (shifter_operand >> (32 - imm)) & 0b1;
				}
			}
			//A5-11
			else if (shift == 0b01)
			{
				//如果shift_imm为0
				if (imm == 0)
				{
					shifter_operand = 0;
					shifter_carry_out = (Rm >> 31) & 0b1;
				}
				else
				{
					shifter_operand = lsr(Rm, imm);
					shifter_carry_out = (Rm >> (imm - 1)) & 0b1;
				}
			}
			//A5-13
			else if (shift == 0b10)
			{
				if (imm == 0)
				{
					uint32 Rm31 = (Rm >> 31) & 0b1;
					if (Rm31 == 0)
					{
						shifter_operand = 0;
						shifter_carry_out = Rm31;
					}
					else
					{
						shifter_operand = 0xffffffff;
						shifter_carry_out = Rm31;
					}
				}
				else
				{
					shifter_operand = asr(Rm, imm);
					shifter_carry_out = Rm >> (imm - 1) & 0b1;
				}
			}
			//A5-15
			else
			{
				//循环右移扩展 (参考: A5-17)
				if (imm == 0)
				{
					shifter_operand = lsr(Rm, 1) | lsl(st->cpsr.c, 31);
					shifter_carry_out = Rm & 0b1;
				}
				else
				{
					shifter_operand = ror(Rm, imm);
					shifter_carry_out = Rm >> (imm - 1) & 0b1;
				}
			}
		}
		//第4位为1，使用寄存器
		else
		{
			uint32 Rs = st->registers[inst_b4(8)];
			uint32 Rs07 = Rs & 0b11111111;
			//对于Rs07为0时，处理方式都是一样的
			if (Rs07 == 0)
			{
				shifter_operand = Rm;
				shifter_carry_out = st->cpsr.c;
			}
			else
			{
				//A5-10
				if (shift == 0b00)
				{
					if (Rs07 < 32)
					{
						shifter_operand = lsl(Rm, Rs07);
						shifter_carry_out = Rm >> (32 - Rs07) & 0b1;
					}
					else if (Rs07 == 32)
					{
						shifter_operand = 0;
						shifter_carry_out = Rm & 0b1;
					}
					else
					{
						shifter_operand = 0;
						shifter_carry_out = 0;
					}
				}
				//A5-12
				else if (shift == 0b01)
				{
					if (Rs07 < 32)
					{
						shifter_operand = lsr(Rm, Rs07);
						shifter_carry_out = Rm >> (Rs07 - 1) & 0b1;
					}
					else if (Rs07 == 32)
					{
						shifter_operand = 0;
						shifter_carry_out = (Rm >> 31) & 0b1;
					}
					else
					{ //Rs07>32
						shifter_operand = 0;
						shifter_carry_out = 0;
					}
				}
				//A5-14
				else if (shift == 0b10)
				{
					if (Rs07 < 32)
					{
						shifter_operand = asr(Rm, Rs07);
						shifter_carry_out = Rm >> (Rs07 - 1) & 0b1;
					}
					else
					{
						uint32 Rm31 = (Rm >> 31) & 0b1;
						if (Rm31 == 0)
						{
							shifter_operand = 0;
							shifter_carry_out = Rm31;
						}
						else
						{
							shifter_operand = 0xffffffff;
							shifter_carry_out = Rm31;
						}
					}
				}
				//A5-16
				else
				{
					uint32 Rs04 = Rs & 0b1111;
					if (Rs04 == 0)
					{
						shifter_operand = Rm;
						shifter_carry_out = (Rm >> 31) & 0b1;
					}
					else
					{
						shifter_operand = ror(Rm, Rs04);
						shifter_carry_out = Rm >> (Rs04 - 1) & 0b1;
					}
				}
			}
		}
	}
	//特别处理pc
	if (rn == r_pc)
		shifter_operand += 4; //文档是+8的，不过这里不一样
	//OK，返回结果
	*operand = shifter_operand;
	return !!shifter_carry_out;
}