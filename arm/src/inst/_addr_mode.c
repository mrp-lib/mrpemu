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

//cond 0 1 0 1 U B 0 L Rn Rd offset_12
//cond 0 1 1 1 U B 0 L Rn Rd 0 0 0 0 0 0 0 0 Rm
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

//cond 0 0 0 P U 1 W L Rn Rd immedH 1 S H 1 ImmedL
//cond 0 0 0 P U 0 W L Rn Rd SBZ    1 S H 1 Rm

//           P U I W
//cond 0 0 0 0 U 1 0 L Rn Rd immedH 1 S H 1 ImmedL

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