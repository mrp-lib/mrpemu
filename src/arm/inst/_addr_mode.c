#include "arm.h"

uint32 _addr_bits(uint32 inst)
{
	uint32 bits = 0;
	for (int32 i = 0; i < 16; i++)
	{
		if (has_reg_m4(i))
			bits++;
	}
	return bits;
}

//         I
//cond 0 0 0 opcode S Rn Rd Rs 0 0 0 1 Rm
//cond 0 0 0 opcode S Rn Rd Rs 0 0 1 1 Rm
//cond 0 0 0 opcode S Rn Rd Rs 0 1 0 1 Rm
//cond 0 0 0 opcode S Rn Rd Rs 0 1 1 1 Rm
//cond 0 0 0 opcode S Rn Rd shift_imm 0 0 0 Rm
//cond 0 0 0 opcode S Rn Rd 0 0 0 0 0 0 0 0 Rm	//shift_imm=0而已，跟上一种一样的处理
//cond 0 0 0 opcode S Rn Rd shift_imm 0 1 0 Rm
//cond 0 0 0 opcode S Rn Rd shift_imm 1 0 0 Rm
//cond 0 0 0 opcode S Rn Rd shift_imm 1 1 0 Rm
//cond 0 0 0 opcode S Rn Rd 0 0 0 0 0 1 1 0 Rm	//shift_imm=0而已，跟上一种一样的处理
//cond 0 0 1 opcode S Rn Rd rotate_imm immed_8
uint32 addr_mode_1(cpu_state_t *st, uint32 inst, bool *carry)
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
		shifter_operand = ror(immed8, imm << 1); //imm*2
		if (imm == 0)
		{
			shifter_carry_out = st->cpsr.c;
		}
		else
		{
			shifter_carry_out = shifter_operand >> 31;
		}
	}
	//否则表示寄存器偏移（需要保证不是扩展指令集）
	else
	{
		//取4、5、6作为识别码
		uint32 icode = inst_bm(4, 6);
		//取得Rm和shift
		uint32 rm = inst_b4(0);
		uint32 rs = inst_b4(8);
		uint32 rs07 = regv(rs) & 0x00ff;
		uint32 rs04 = regv(rs) & 0x000f;
		uint32 shift_imm = inst_bm(7, 11);
		switch (icode)
		{
		case 0b000:
			if (shift_imm == 0)
			{
				shifter_operand = regv(rm);
				shifter_carry_out = st->cpsr.c;
			}
			else
			{
				shifter_operand = lsl(regv(rm), shift_imm);
				shifter_carry_out = (regv(rm) >> (32 - shift_imm)) & 0b01;
			}
			break;
		case 0b001:
			if (rs07 == 0)
			{
				shifter_operand = regv(rm);
				shifter_carry_out = st->cpsr.c;
			}
			else if (rs07 < 32)
			{
				shifter_operand = lsl(regv(rm), rs07);
				shifter_operand = (regv(rm) >> (32 - rs07)) & 0b01;
			}
			else if (rs07 == 32)
			{
				shifter_operand = 0;
				shifter_carry_out = regv(rm) & 0b01;
			}
			else
			{
				shifter_operand = 0;
				shifter_carry_out = 0;
			}
			break;
		case 0b010:
			if (shift_imm == 0)
			{
				shifter_operand = 0;
				shifter_carry_out = regv(rm) >> 31;
			}
			else
			{
				shifter_operand = lsr(regv(rm), shift_imm);
				shifter_carry_out = (regv(rm) >> (shift_imm - 1)) & 0b01;
			}
			break;
		case 0b011:
			if (rs07 == 0)
			{
				shifter_operand = regv(rm);
				shifter_carry_out = st->cpsr.c;
			}
			else if (rs07 < 32)
			{
				shifter_operand = lsr(regv(rm), rs07);
				shifter_carry_out = (regv(rm) >> (rs07 - 1)) & 0b01;
			}
			else if (rs07 == 32)
			{
				shifter_operand = 0;
				shifter_carry_out = regv(rm) >> 31;
			}
			else
			{
				shifter_operand = 0;
				shifter_carry_out = 0;
			}
			break;
		case 0b100:
			if (shift_imm == 0)
			{
				uint32 rm31 = regv(rm) >> 31;
				if (rm31 == 0)
				{
					shifter_operand = 0;
					shifter_carry_out = rm31;
				}
				else
				{
					shifter_operand = 0xffffffff;
					shifter_carry_out = rm31;
				}
			}
			else
			{
				shifter_operand = asr(regv(rm), shift_imm);
				shifter_carry_out = (regv(rm) << (shift_imm - 1)) & 0b01;
			}
			break;
		case 0b101:
			if (rs07 == 0)
			{
				shifter_operand = regv(rm);
				shifter_carry_out = st->cpsr.c;
			}
			else if (rs07 < 32)
			{
				shifter_operand = asr(regv(rm), rs07);
				shifter_carry_out = (regv(rm) >> (rs07 - 1)) & 0b01;
			}
			else
			{
				uint32 rm31 = regv(rm) >> 31;
				if (rm31 == 0)
				{
					shifter_operand = 0;
					shifter_carry_out = rm31;
				}
				else
				{
					shifter_operand = 0xffffffff;
					shifter_carry_out = rm31;
				}
			}
			break;
		case 0b110:
			if (shift_imm == 0)
			{
				shifter_operand = lsl((uint32)st->cpsr.c, 31) | lsr(regv(rm), 1);
				shifter_carry_out = regv(rm) & 0b01;
			}
			else
			{
				shifter_operand = ror(regv(rm), shift_imm);
				shifter_carry_out = (regv(rm) >> (shift_imm - 1)) & 0b01;
			}
			break;
		case 0b111:
			if (rs07 == 0)
			{
				shifter_operand = regv(rm);
				shifter_carry_out = st->cpsr.c;
			}
			else if (rs04 == 0)
			{
				shifter_operand = regv(rm);
				shifter_carry_out = regv(rm) >> 31;
			}
			else
			{
				shifter_operand = ror(regv(rm), rs04);
				shifter_carry_out = (regv(rm) >> (rs04 - 1)) & 0b01;
			}
			break;
		}
	}
	//特别处理pc
	if (rn == r_pc)
		shifter_operand += 4; //文档是+8的，不过这里不一样

	//OK，返回结果
	if (carry != null)
		*carry = shifter_carry_out;

	return shifter_operand;
}

//         I P U B W L
//cond 0 1 0 1 U B 0 L Rn Rd offset_12
//cond 0 1 0 1 U B 1 L Rn Rd offset_12
//cond 0 1 0 0 U B 0 L Rn Rd offset_12
//cond 0 1 1 1 U B 0 L Rn Rd 0 0 0 0 0 0 0 0 Rm
//cond 0 1 1 1 U B 0 L Rn Rd shift_imm shift 0 Rm
//cond 0 1 1 1 U B 1 L Rn Rd 0 0 0 0 0 0 0 0 Rm
//cond 0 1 1 1 U B 1 L Rn Rd shift_imm shift 0 Rm
//cond 0 1 1 0 U B 0 L Rn Rd 0 0 0 0 0 0 0 0 Rm
//cond 0 1 1 0 U B 0 L Rn Rd shift_imm shift 0 Rm
uint32 addr_mode_2(cpu_state_t *st, uint32 inst)
{
	uint32 I = inst_b1(25);
	uint32 P = inst_b1(24);
	uint32 U = inst_b1(23);
	uint32 W = inst_b1(21);
	uint32 rn = inst_b4(16);

	uint32 address;

	// if (rn == r_pc && P == 1 && W == 0)
	// {
	// 	address = st->registers[r_pc];// + 8;
	// }
	// else
	// {
	//立即数偏移
	if (I == 0)
	{
		uint32 offset_12 = inst_bm(0, 11);
		if (P == 0)
		{
			address = st->registers[rn];
			st->registers[rn] = (U == 1) ? st->registers[rn] + offset_12 : st->registers[rn] - offset_12;
		}
		else
		{
			address = (U == 1) ? st->registers[rn] + offset_12 : st->registers[rn] - offset_12;
			if (W)
				st->registers[rn] = address;
		}
	}
	//寄存器偏移
	else
	{
		uint32 rm = inst_b4(0);
		uint32 not0 = inst_bm(4, 11); //表示4-11位非全0
		uint32 shift = inst_bm(5, 6);
		uint32 shift_imm = inst_bm(7, 11);

		uint32 index;
		if (P == 0) //p==0 w==0
		{
			address = st->registers[rn];
			if (not0)
			{
				switch (shift)
				{
				case 0b00:
					index = lsl(st->registers[rm], shift_imm);
				case 0b01:
					if (shift_imm == 0) /* LSR #32 */
						index = 0;
					else
						index = lsr(st->registers[rm], shift_imm);
					break;
				case 0b10:
					if (shift_imm == 0) /* ASR #32 */
					{
						if (st->registers[rm] >> 31)
							index = 0xFFFFFFFF;
						else
							index = 0;
					}
					else
						index = asr(rm, shift_imm);
					break;
				case 0b11:
					if (shift_imm == 0) /* RRX */
						index = lsl(st->cpsr.c, 31) | lsr(st->registers[rm], 1);
					else /* ROR */
						index = ror(st->registers[rm], shift_imm);
					break;
				}
				st->registers[rn] = (U == 1) ? st->registers[rn] + index : st->registers[rn] - index;
			}
			else
				st->registers[rn] = (U == 1) ? st->registers[rn] + st->registers[rm] : st->registers[rn] - st->registers[rm];
		}
		else if (W == 0) //p==1 w==0
		{
			if (not0)
			{
				switch (shift)
				{
				case 0b00:
					index = lsl(st->registers[rm], shift_imm);
					break;
				case 0b01:
					if (shift_imm == 0) /* LSR #32 */
						index = 0;
					else
						index = lsr(st->registers[rm], shift_imm);
					break;
				case 0b10:
					if (shift_imm == 0) /* ASR #32 */
					{
						if (st->registers[rm] >> 31)
							index = 0xFFFFFFFF;
						else
							index = 0;
					}
					else
						index = asr(st->registers[rm], shift_imm);
					break;
				case 0b11:
					if (shift_imm == 0) /* RRX */
						index = lsl(st->cpsr.c, 31) | lsr(st->registers[rm], 1);
					else /* ROR */
						index = ror(st->registers[rm], shift_imm);
					break;
				}
				address = (U == 1) ? st->registers[rn] + index : st->registers[rn] - index;
			}
			else
				address = (U == 1) ? st->registers[rn] + st->registers[rm] : st->registers[rn] - st->registers[rm];
		}
		else //p==1 w==1
		{
			if (not0)
			{
				switch (shift)
				{
				case 0b00:
					index = lsl(st->registers[rm], shift_imm);
				case 0b01:
					if (shift_imm == 0) /* LSR #32 */
						index = 0;
					else
						index = lsr(st->registers[rm], shift_imm);
					break;
				case 0b10:
					if (shift_imm == 0) /* ASR #32 */
					{
						if (st->registers[rm] >> 31)
							index = 0xFFFFFFFF;
						else
							index = 0;
					}
					else
						index = asr(st->registers[rm], shift_imm);
					break;
				case 0b11:
					if (shift_imm == 0) /* RRX */
						index = lsl(st->cpsr.c, 31) | lsr(st->registers[rm], 1);
					else /* ROR */
						index = ror(st->registers[rm], shift_imm);
					break;
				}
				address = (U == 1) ? st->registers[rn] + index : st->registers[rn] - index;
			}
			else
				address = (U == 1) ? st->registers[rn] + st->registers[rm] : st->registers[rn] - st->registers[rm];

			//写回
			st->registers[rn] = address;
		}
	}
	if (rn == r_pc)
		address += 4;
	// }
	//最终返回地址
	return address;
}

//           P U I W
//cond 0 0 0 1 U 1 0 L Rn Rd immedH 1 S H 1 immedL
//cond 0 0 0 1 U 1 1 L Rn Rd immedH 1 S H 1 ImmedL
//cond 0 0 0 0 U 1 0 L Rn Rd immedH 1 S H 1 ImmedL
//cond 0 0 0 1 U 0 0 L Rn Rd SBZ 1 S H 1 Rm
//cond 0 0 0 1 U 0 1 L Rn Rd SBZ 1 S H 1 Rm
//cond 0 0 0 0 U 0 0 L Rn Rd SBZ 1 S H 1 Rm
uint32 addr_mode_3(cpu_state_t *st, uint32 inst)
{
	uint32 p = inst_b1(24);
	uint32 u = inst_b1(23);
	uint32 i = inst_b1(22);
	uint32 w = inst_b1(21);
	uint32 rn = inst_b4(16);

	uint32 pw = (p << 1) | w; //pw的组合

	uint32 address;

	//立即数偏移
	if (i == 1)
	{
		uint32 immh = inst_b4(8);
		uint32 imml = inst_b4(0);
		uint32 offset_8 = (immh << 4) | imml;
		if (pw == 0b00)
		{
			address = st->registers[rn];
			st->registers[rn] = (u == 1) ? st->registers[rn] + offset_8 : st->registers[rn] - offset_8;
		}
		else if (pw == 0b10)
		{
			address = (u == 1) ? st->registers[rn] + offset_8 : st->registers[rn] - offset_8;
		}
		else if (pw == 0b11)
		{
			address = (u == 1) ? st->registers[rn] + offset_8 : st->registers[rn] - offset_8;
			st->registers[rn] = address;
		}
		//其他是错的
	}
	//寄存器偏移
	else
	{
		uint32 rm = inst_b4(0);
		if (pw == 0b00)
		{
			address = st->registers[rn];
			st->registers[rn] = (u == 1) ? st->registers[rn] + st->registers[rm] : st->registers[rn] - st->registers[rm];
		}
		else if (pw == 0b10)
		{
			address = (u == 1) ? st->registers[rn] + st->registers[rm] : st->registers[rn] - st->registers[rm];
		}
		else if (pw == 0b11)
		{
			address = (u == 1) ? st->registers[rn] + st->registers[rm] : st->registers[rn] - st->registers[rm];
			st->registers[rn] = address;
		}
		//其他是错的
	}
	if (rn == r_pc)
		address += 4;
	//把地址返回出去
	return address;
}

//cond 1 0 0 0 0 S W L Rn register list
//cond 1 0 0 0 1 S W L Rn register list
//cond 1 0 0 1 0 S W L Rn register list
//cond 1 0 0 1 1 S W L Rn register list
void addr_mode_4(cpu_state_t *st, uint32 inst, uint32 *start_address, uint32 *end_address)
{
	uint32 pu = inst_bm(23, 24);
	uint32 rn = inst_b4(16);
	uint32 w = inst_b1(21);
	uint32 registers_list = inst_bm(0, 15);

	uint32 reg_bits = _addr_bits(registers_list); //计算总共有设置了多少个寄存器

	if (pu == 0b0000)
	{
		*start_address = st->registers[rn] - (reg_bits * 4) + 4;
		*end_address = st->registers[rn];
		if (w == 1)
			st->registers[rn] -= (reg_bits * 4);
	}
	else if (pu == 0b0001)
	{
		*start_address = st->registers[rn];
		*end_address = st->registers[rn] + (reg_bits * 4) - 4;
		if (w == 1)
			st->registers[rn] += (reg_bits * 4);
	}
	else if (pu == 0b0010)
	{
		*start_address = st->registers[rn] - (reg_bits * 4);
		*end_address = st->registers[rn] - 4;
		if (w == 1)
			st->registers[rn] -= (reg_bits * 4);
	}
	else
	{
		*start_address = st->registers[rn] + 4;
		*end_address = st->registers[rn] + (reg_bits * 4);
		if (w == 1)
			st->registers[rn] += (reg_bits * 4);
	}
}