#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arm.h"

#define call_inst(ins_name) arm_inst_##ins_name(st, inst)

//创建CPU
cpu_state_t *cpu_create(memory_t *mem)
{
	logi("cpu_create(size=%p)", mem);

	//创建CPU状态
	cpu_state_t *cpuState = malloc(sizeof(cpu_state_t));
	if (cpuState != null)
	{
		memset(cpuState, 0, sizeof(cpu_state_t));
		cpuState->mem = mem;
	}

	//ok
	return cpuState;
}

//释放CPU
void cpu_destory(cpu_state_t *st)
{
	logi("cpu_destory(st=%p)", st);
	if (st != null)
	{
		if (st->mem != null)
			mem_destory(st->mem);
		free(st);
	}
}

//取得指令
uint32 cpu_fetch_inst(cpu_state_t *st)
{
	//由于三级流水线的问题，pc应该-8才是当前的指令
	// uint32 inst = st->registers[r_pc - 8]; //取到指令

	uint32 inst = mem_ld32(st->mem, st->registers[r_pc]);
	st->registers[r_pc] += 4; //下一条指令
	return inst;			  //返回取到的指令
}

void cpu_print_regs(cpu_state_t *st)
{
	for (uint32 i = 0; i <= 15; i++)
	{
		if (i != 0)
			printf(" ");
		printf("R%d=0x%08x", i, st->registers[i]);
	}
	//打印cspr
	printf(" CSPR=");
	printf(st->n ? "N" : "n");
	printf(st->z ? "Z" : "z");
	printf(st->c ? "C" : "c");
	printf(st->v ? "V" : "v");
	printf("_");
	printf(st->t ? "T" : "t");
	printf("\n");
}

//执行指令
int32 cpu_exec_inst(cpu_state_t *st, uint32 inst)
{
	//无条件语句
	if ((inst & 0xf0000000) == 0xf0000000)
	{
		uint32 opcode = (inst >> 26) & 0b0011;
		if (opcode == 0b0000)
		{
			uint32 b16 = (inst >> 16) & 0b0001;
			if (b16 == 0) //cps
				return call_inst(cps);
			else //setend
				return call_inst(setend);
		}
		else if (opcode == 0b0001) //pld
			return call_inst(pld);
		else if (opcode == 0b0010)
		{
			uint32 b25 = (inst >> 25) & 0b0001;
			if (b25 == 0)
			{
				uint32 b22 = (inst >> 22) & 0b0001;
				if (b22 == 0) //rfe
					return call_inst(rfe);
				else //srs
					return call_inst(srs);
			}
			else //blx(1)
				return call_inst(blx_1);
		}
	}
	else
	{
		uint32 idcode = (inst >> 25) & 0b0111;	  //识别码
		if (idcode == 0b0000 || idcode == 0b0001) //000或001
		{
			uint32 sub_idcode = (inst >> 4) & 0b1111; //子识别码
			uint32 opcode = (inst >> 21) & 0b1111;	  //操作码
			uint32 s = (inst >> 20) & 0b0001;
			//只有000才会是1xx1
			if (idcode == 0b0000 && (sub_idcode & 0b1001) == 0b1001)
			{
				//1001
				if (sub_idcode == 0b1001)
				{
					if (opcode == 0b0000) //mul
						return call_inst(mul);
					else if (opcode == 0b0001) //mla
						return call_inst(mla);
					else if (opcode == 0b0010) //umaal
						return call_inst(umaal);
					else if (opcode == 0b0100) //umull
						return call_inst(umull);
					else if (opcode == 0b0101) //umlal
						return call_inst(umlal);
					else if (opcode == 0b0110) //smull
						return call_inst(smull);
					else if (opcode == 0b0111) //smlal
						return call_inst(smlal);
					else if (opcode == 0b1000) //swp
						return call_inst(swp);
					else if (opcode == 0b1010) //swpb
						return call_inst(swpb);
					else if (opcode == 0b1100)
					{
						if (s == 1) //ldrex
							return call_inst(ldrex);
						else //strex
							return call_inst(strex);
					}
					else //undefined
						return EXEC_UNPREDICTABLE;
				}
				//1011
				else if (sub_idcode == 0b1011)
				{
					if (s == 1) //ldrh
						return call_inst(ldrh);
					else //strh
						return call_inst(strh);
				}
				//1101
				else if (sub_idcode == 0b1101)
				{
					if (s == 1) //ldrsb
						return call_inst(ldrsb);
					else //ldrd
						return call_inst(ldrd);
				}
				//1111
				else if (sub_idcode == 0b1111)
				{
					if (s == 1) //ldrsh
						return call_inst(ldrsh);
					else //strd
						return call_inst(strd);
				}
			}
			//op=10xx s=0
			else if (s == 0 && (opcode & 0b1100) == 0b1000)
			{
				uint32 opxx = opcode & 0b0011; //取opcode的xx位
				if (idcode == 0b0001)
				{
					if ((opcode & 0b0001) == 0b0001) //msr
						return call_inst(msr);
					else //undefined
						return EXEC_UNPREDICTABLE;
				}
				//subidcode=xxx0
				else if ((sub_idcode & 0b0001) == 0b0000)
				{
					if (sub_idcode == 0b0010) //bxj
						return call_inst(bxj);
					//下面对opxx也只有4种情况
					else if (opxx == 0b0000) //smla<x><y>
						return call_inst(smla_x_y);
					else if (opxx == 0b0001)
					{
						if ((sub_idcode & 0b0010) == 0b0000) //smlaw
							return call_inst(smlaw);
						else //smulw<y>
							return call_inst(smulw_y);
					}
					else if (opxx == 0b0010) //smlal<x><y>
						return call_inst(smlal_x_y);
					else //smul<x><y>
						return call_inst(smul_x_y);
				}
				//subidcode=0xx1 xx对应了4种情况
				else if ((sub_idcode & 0b1001) == 0b0001)
				{
					if (sub_idcode == 0b0001)
					{
						if (opxx == 0b0001) //bx
							return call_inst(bx);
						else if (opxx == 0b0011) //clz
							return call_inst(clz);
					}
					else if (sub_idcode == 0b0011) //blx(2)
						return call_inst(blx_2);
					else if (sub_idcode == 0b0101)
					{
						if (opxx == 0b0000) //qadd
							return call_inst(qadd);
						else if (opxx == 0b0001) //qsub
							return call_inst(qsub);
						else if (opxx == 0b0010) //qdadd
							return call_inst(qdadd);
						else //qdsub
							return call_inst(qdsub);
					}
					else //brpt
						return call_inst(brpt);
				}
			}
			else
			{
				switch (opcode)
				{
				case 0b0000: //and
					return call_inst(and);
				case 0b0001: //eor
					return call_inst(eor);
				case 0b0010: //sub
					return call_inst(sub);
				case 0b0011: //rsb
					return call_inst(rsb);
				case 0b0100: //add
					return call_inst(add);
				case 0b0101: //adc
					return call_inst(adc);
				case 0b0110: //sbc
					return call_inst(sbc);
				case 0b0111: //rsc
					return call_inst(rsc);
				case 0b1000: //tst
					return call_inst(tst);
				case 0b1001: //teq
					return call_inst(teq);
				case 0b1010: //cmp
					return call_inst(cmp);
				case 0b1011: //cmn
					return call_inst(cmn);
				case 0b1100: //orr
					return call_inst(orr);
				case 0b1101:
				{
					uint32 i = (inst >> 25) & 0b0001;
					if (s == 0 && i == 0) //cpy
						return call_inst(cpy);
					else //mov
						return call_inst(mov);
				}
				case 0b1110: //bic
					return call_inst(bic);
				case 0b1111: //mvn
					return call_inst(mvn);
				}
			}
		}
		else if (idcode == 0b0010 || idcode == 0b0011) //010或011
		{
			uint32 b24 = (inst >> 21) & 0b0001;		  //第24位
			uint32 sub_idcode = (inst >> 4) & 0b1111; //子识别码
			uint32 opcode = (inst >> 21) & 0b1111;	  //操作码
			uint32 l = (inst >> 20) & 0b0001;
			uint32 r16 = (inst >> 16) & 0x000f; //第16-19，可能是rn或rd
			uint32 r12 = (inst >> 12) & 0x000f; //第12-15，可能是rn或rd

			//如果识别码是010 或 子识别码是xxx0表示数据加载存储指令
			if (idcode == 0b0010 || (sub_idcode & 0b0001) == 0b0000)
			{
				uint32 p = (inst >> 24) & 0b0001;
				uint32 b = (inst >> 22) & 0b0001;
				uint32 w = (inst >> 21) & 0b0001;
				if (l == 0)
				{
					if (b == 0)
					{
						if (p == 0 && w == 1) //strt
							return call_inst(strt);
						else //str
							return call_inst(str);
					}
					else
					{
						if (p == 0 && w == 1) //strbt
							return call_inst(strbt);
						else //strb
							return call_inst(strb);
					}
				}
				else
				{
					if (b == 0)
					{
						if (p == 0 && w == 1) //ldrt
							return call_inst(ldrt);
						else //ldr
							return call_inst(ldr);
					}
					else
					{
						if (p == 0 && w == 1) //ldrbt
							return call_inst(ldrbt);
						else //ldrb
							return call_inst(ldrb);
					}
				}
			}
			//下面根据子识别码来识别，子识别码最后一位是1因此只有8种情况
			else if (sub_idcode == 0b0001)
			{
				switch (opcode)
				{
				case 0b0000: //sadd16
					return call_inst(sadd16);
				case 0b0001:
					if (l == 0) //qadd16
						return call_inst(qadd16);
					else //shadd16
						return call_inst(shadd16);
				case 0b0010: //uadd16
					return call_inst(uadd16);
				case 0b0011:
					if (l == 0) //uqadd16
						return call_inst(uqadd16);
					else //uhadd16
						return call_inst(uhadd16);
				case 0b0100: // * pkhbt
					return call_inst(pkhbt);
				case 0b0101: // * ssat
					return call_inst(ssat);
				case 0b0111: // * usat
					return call_inst(usat);
				case 0b1000:
					if (r12 == 0b1111) // * smuad
						return call_inst(smuad);
					else // * smlad
						return call_inst(smlad);
				case 0b1010:
					if (l == 0) // * smlald
						return call_inst(smlald);
					else if (r12 == 0b1111) // * smmul
						return call_inst(smmul);
					else //smmla
						return call_inst(smmla);
				case 0b1100:
					//看rn是不是1111
					if (r12 == 0b1111) //usad8
						return call_inst(usad8);
					else //usada8
						return call_inst(usada8);
				default:
					return EXEC_UNPREDICTABLE;
				}
			}
			else if (sub_idcode == 0b0011)
			{
				switch (opcode)
				{
				case 0b0000: //saddsubx
					return call_inst(saddsubx);
				case 0b0001:
					if (l == 0) //qaddsubx
						return call_inst(qaddsubx);
					else //shaddsubx
						return call_inst(shaddsubx);
				case 0b0010: //uaddsubx
					return call_inst(uaddsubx);
				case 0b0011:
					if (l == 0) //uqaddsubx
						return call_inst(uqaddsubx);
					else //uhaddsubx
						return call_inst(uhaddsubx);
				case 0b0101:
					if (l == 0) //ssat16
						return call_inst(ssat16);
					else //rev
						return call_inst(rev);
				case 0b0111: //usat16
					return call_inst(usat16);
				case 0b1000:
					if (r12 == 0b1111) //smuad
						return call_inst(smuad);
					else //smlad
						return call_inst(smlad);
				case 0b1010:
					if (l == 0) //smlald
						return call_inst(smlald);
					else if (r12 == 0b1111) //smmul
						return call_inst(smmul);
					else //smmla
						return call_inst(smmla);
				default:
					return EXEC_UNPREDICTABLE;
				}
			}
			else if (sub_idcode == 0b0101)
			{
				switch (opcode)
				{
				case 0b0000: //ssubaddx
					return call_inst(ssubaddx);
				case 0b0001:
					if (l == 0) //qsubaddx
						return call_inst(qsubaddx);
					else //shsubaddx
						return call_inst(shsubaddx);
				case 0b0010: //usubaddx
					return call_inst(usubaddx);
				case 0b0011:
					if (l == 0) //uqsubaddx
						return call_inst(uqsubaddx);
					else //uhsubaddx
						return call_inst(uhsubaddx);
				case 0b0100: // * pkhtb
					return call_inst(pkhtb);
				case 0b0101: //ssat
					return call_inst(ssat);
				case 0b0111: //usat
					return call_inst(usat);
				case 0b1000:
					if (r12 == 0b1111) //smusd
						return call_inst(smusd);
					else //smlsd
						return call_inst(smlsd);
				case 0b1010: //smlsld
					return call_inst(smlsld);
				default:
					return EXEC_UNPREDICTABLE;
				}
			}
			else if (sub_idcode == 0b0111)
			{
				switch (opcode)
				{
				case 0b0000: //ssub16
					return call_inst(ssub16);
				case 0b0001:
					if (l == 0) //qsub16
						return call_inst(qsub16);
					else //shsub16
						return call_inst(shsub16);
				case 0b0010: //usub16
					return call_inst(usub16);
				case 0b0011:
					if (l == 0) //uqsub16
						return call_inst(uqsub16);
					else //uhsub16
						return call_inst(uhsub16);
				case 0b0100:
					if (r16 == 0b1111) //sxtb16
						return call_inst(sxtb16);
					else //sxtab16
						return call_inst(sxtab16);
				case 0b0101:
					if (l == 0)
					{
						if (r16 == 0b1111) //sxtb
							return call_inst(sxtb);
						else //sxtab
							return call_inst(sxtab);
					}
					else
					{
						if (r16 == 0b1111) //sxth
							return call_inst(sxth);
						else //sxtah
							return call_inst(sxtah);
					}
				case 0b0110:
					if (r16 == 0b1111) //uxtb16
						return call_inst(uxtb16);
					else //uxtab16
						return call_inst(uxtab16);
				case 0b0111:
					if (l == 0)
					{
						if (r16 == 0b1111) //uxtb
							return call_inst(uxtb);
						else //uxtab
							return call_inst(uxtab);
					}
					else
					{
						if (r16 == 0b1111) //uxth
							return call_inst(uxth);
						else //uxtah
							return call_inst(uxtah);
					}

				case 0b1000:
					if (r12 == 0b1111) //smusd
						return call_inst(smusd);
					else //smlsd
						return call_inst(smlsd);
				case 0b1010: //smlsld
					return call_inst(smlsld);
				default:
					return EXEC_UNPREDICTABLE;
				}
			}
			else if (sub_idcode == 0b1001)
			{
				switch (opcode)
				{
				case 0b0000: //sadd8
					return call_inst(sadd8);
				case 0b0001:
					if (l == 0) //qadd8
						return call_inst(qadd8);
					else //shadd8
						return call_inst(shadd8);
				case 0b0010: //uadd8
					return call_inst(uadd8);
				case 0b0011:
					if (l == 0) //uqadd8
						return call_inst(uqadd8);
					else //uhadd8
						return call_inst(uhadd8);
				case 0b0100: //pkhbt
					return call_inst(pkhbt);
				case 0b0101: //ssat
					return call_inst(ssat);
				case 0b0111: //usat
					return call_inst(usat);
				default:
					return EXEC_UNPREDICTABLE;
				}
			}
			else if (sub_idcode == 0b1011)
			{
				switch (opcode)
				{
				case 0b0100: //sel
					return call_inst(sel);
				case 0b0101: //rev16
					return call_inst(rev16);
				case 0b0111: //revsh
					return call_inst(revsh);
				default:
					return EXEC_UNPREDICTABLE;
				}
			}
			else if (sub_idcode == 0b1101)
			{
				switch (opcode)
				{
				case 0b0100: //pkhtb
					return call_inst(pkhtb);
				case 0b0101: //ssat
					return call_inst(ssat);
				case 0b0111: //usat
					return call_inst(usat);
				case 0b1010: //smmls
					return call_inst(smmls);
				default:
					return EXEC_UNPREDICTABLE;
				}
			}
			else if (sub_idcode == 0b1111)
			{
				if (opcode == 0b1111 && l == 1) //undefined
					return EXEC_UNPREDICTABLE;
				switch (opcode)
				{
				case 0b0000: //ssub8
					return call_inst(ssub8);
				case 0b0001:
					if (l == 0) //qsub8
						return call_inst(qsub8);
					else //shsub8
						return call_inst(shsub8);
				case 0b0010: //usub8
					return call_inst(usub8);
				case 0b0011:
					if (l == 0) //uqsub8
						return call_inst(uqsub8);
					else //uhsub8
						return call_inst(uhsub8);
				case 0b1010: //smmls
					return call_inst(smmls);
				default:
					return EXEC_UNPREDICTABLE;
				}
			}
		}
		else if (idcode == 0b0100)
		{
			uint32 s = (inst >> 22) & 0b0001;
			uint32 l = (inst >> 20) & 0b0001;
			if (l == 0)
			{
				if (s == 0) //stm(1)
					return call_inst(stm_1);
				else //stm(2)
					return call_inst(stm_2);
			}
			else
			{
				if (s == 0) //ldm(1)
					return call_inst(ldm_1);
				else
				{
					uint32 b15 = (inst >> 15) & 0b0001;
					if (b15 == 0) //ldm(2)
						return call_inst(ldm_2);
					else //ldm(3)
						return call_inst(ldm_3);
				}
			}
		}
		else if (idcode == 0b0101) //b,bl
			return call_inst(b_bl);
		else if (idcode == 0b0110)
		{
			uint32 l = (inst >> 20) & 0b0001;
			uint32 opcode = (inst >> 21) & 0b1111;
			if (l == 0)
			{
				if (opcode == 0b0010) //mcrr
					return call_inst(mcrr);
				else //stc
					return call_inst(stc);
			}
			else
			{
				if (opcode == 0b0010) //mrrc
					return call_inst(mrrc);
				else //ldr
					return call_inst(ldr);
			}
		}
		else if (idcode == 0b0111)
		{
			uint32 b24 = (inst >> 24) & 0b0001;
			if (b24 == 0)
			{
				uint32 b4 = (inst >> 4) & 0b0001;
				if (b4 == 0) //cdp
					return call_inst(cdp);
				else
				{
					uint32 l = (inst >> 20) & 0b0001;
					if (l == 0) //mcr
						return call_inst(mcr);
					else //mrc
						return call_inst(mrc);
				}
			}
			else //swi
				return call_inst(swi);
		}
	}
}

/*
指令整理：

000,001:
	子识别码=1001
			mul				cond 0 0 0 0 0 0 0 S Rd SBZ Rs 1 0 0 1 Rm
			mla				cond 0 0 0 0 0 0 1 S Rd Rn Rs 1 0 0 1 Rm
			umaal			cond 0 0 0 0 0 1 0 0 RdHi RdLo Rs 1 0 0 1 Rm
			umull			cond 0 0 0 0 1 0 0 S RdHi RdLo Rs 1 0 0 1 Rm
			umlal			cond 0 0 0 0 1 0 1 S RdHi RdLo Rs 1 0 0 1 Rm
			smull			cond 0 0 0 0 1 1 0 S RdHi RdLo Rs 1 0 0 1 Rm
			smlal			cond 0 0 0 0 1 1 1 S RdHi RdLo Rs 1 0 0 1 Rm
			swp				cond 0 0 0 1 0 0 0 0 Rn Rd SBZ 1 0 0 1 Rm
			swpb			cond 0 0 0 1 0 1 0 0 Rn Rd SBZ 1 0 0 1 Rm
			strex			cond 0 0 0 1 1 0 0 0 Rn Rd SBO 1 0 0 1 Rm
			ldrex			cond 0 0 0 1 1 0 0 1 Rn Rd SBO 1 0 0 1 SBO
	子识别码=1011
			ldrh			cond 0 0 0 P U I W 1 Rn Rd addr_mode 1 0 1 1 addr_mode
			strh			cond 0 0 0 P U I W 0 Rn Rd addr_mode 1 0 1 1 addr_mode
	子识别码=1101
			ldrsb			cond 0 0 0 P U I W 1 Rn Rd addr_mode 1 1 0 1 addr_mode
			ldrd			cond 0 0 0 P U I W 0 Rn Rd addr_mode 1 1 0 1 addr_mode
	子识别码=1111
			ldrsh			cond 0 0 0 P U I W 1 Rn Rd addr_mode 1 1 1 1 addr_mode
			strd			cond 0 0 0 P U I W 0 Rn Rd addr_mode 1 1 1 1 addr_mode
	;
	
			msr				cond 0 0 1 1 0 R 1 0 field_mask SBO rotate_imm 8_bit_immediate
			mrs				cond 0 0 0 1 0 R 0 0 SBO Rd SBZ
			msr				cond 0 0 0 1 0 R 1 0 field_mask SBO SBZ 0 0 0 0 Rm

			bx				cond 0 0 0 1 0 0 1 0 SBO SBO SBO 0 0 0 1 Rm
			clz				cond 0 0 0 1 0 1 1 0 SBO Rd SBO 0 0 0 1 Rm
			blx(2)			cond 0 0 0 1 0 0 1 0 SBO SBO SBO 0 0 1 1 Rm
			qadd			cond 0 0 0 1 0 0 0 0 Rn Rd SBZ 0 1 0 1 Rm
			qsub			cond 0 0 0 1 0 0 1 0 Rn Rd SBZ 0 1 0 1 Rm
			qdadd			cond 0 0 0 1 0 1 0 0 Rn Rd SBZ 0 1 0 1 Rm
			qdsub			cond 0 0 0 1 0 1 1 0 Rn Rd SBZ 0 1 0 1 Rm
			brpt			cond 0 0 0 1 0 0 1 0 immed 0 1 1 1 immed (cond = 1110)

			bxj				cond 0 0 0 1 0 0 1 0 SBO SBO SBO 0 0 1 0 Rm
			smla<x><y>		cond 0 0 0 1 0 0 0 0 Rd Rn Rs 1 y x 0 Rm
			smlaw			cond 0 0 0 1 0 0 1 0 Rd Rn Rs 1 y 0 0 Rm
			smulw<y>		cond 0 0 0 1 0 0 1 0 Rd SBZ Rs 1 y 1 0 Rm
			smlal<x><y>		cond 0 0 0 1 0 1 0 0 RdHi RdLo Rs 1 y x 0 Rm
			smul<x><y>		cond 0 0 0 1 0 1 1 0 Rd SBZ Rs 1 y x 0 Rm

			and				cond 0 0 I 0 0 0 0 S Rn Rd shifter_operand
			eor				cond 0 0 I 0 0 0 1 S Rn Rd shifter_operand
			sub				cond 0 0 I 0 0 1 0 S Rn Rd shifter_operand
			rsb				cond 0 0 I 0 0 1 1 S Rn Rd shifter_operand
			add				cond 0 0 I 0 1 0 0 S Rn Rd shifter operand
			adc				cond 0 0 I 0 1 0 1 S Rn Rd shifter_operand
			sbc				cond 0 0 I 0 1 1 0 S Rn Rd shifter_operand
			rsc				cond 0 0 I 0 1 1 1 S Rn Rd shifter_operand
			tst				cond 0 0 I 1 0 0 0 1 Rn SBZ shifter_operand
			teq				cond 0 0 I 1 0 0 1 1 Rn SBZ shifter_operand
			cmp				cond 0 0 I 1 0 1 0 1 Rn SBZ shifter_operand
			cmn				cond 0 0 I 1 0 1 1 1 Rn SBZ shifter_operand
			orr				cond 0 0 I 1 1 0 0 S Rn Rd shifter_operand
			mov				cond 0 0 I 1 1 0 1 S SBZ Rd shifter_operand
			cpy				cond 0 0 0 1 1 0 1 0 SBZ Rd 0 0 0 0 0 0 0 0 Rm
			bic				cond 0 0 I 1 1 1 0 S Rn Rd shifter_operand
			mvn				cond 0 0 I 1 1 1 1 S SBZ Rd shifter_operand
	;

010,011
	识别码=011
		B[24-20] = 11111, B[7-4]=1111
			未定义
		B[24]=0, 子识别码=0001
			sadd16			cond 0 1 1 0 0 0 0 1 Rn Rd SBO 0 0 0 1 Rm
			qadd16			cond 0 1 1 0 0 0 1 0 Rn Rd SBO 0 0 0 1 Rm
			shadd16			cond 0 1 1 0 0 0 1 1 Rn Rd SBO 0 0 0 1 Rm
			uadd16			cond 0 1 1 0 0 1 0 1 Rn Rd SBO 0 0 0 1 Rm
			uqadd16			cond 0 1 1 0 0 1 1 0 Rn Rd SBO 0 0 0 1 Rm
			uhadd16			cond 0 1 1 0 0 1 1 1 Rn Rd SBO 0 0 0 1 Rm
			usada8			cond 0 1 1 1 1 0 0 0 Rd Rn Rs 0 0 0 1 Rm
			usad8			cond 0 1 1 1 1 0 0 0 Rd 1 1 1 1 Rs 0 0 0 1 Rm
		B[24]=0, 子识别码=0011
			saddsubx		cond 0 1 1 0 0 0 0 1 Rn Rd SBO 0 0 1 1 Rm
			qaddsubx		cond 0 1 1 0 0 0 1 0 Rn Rd SBO 0 0 1 1 Rm
			shaddsubx		cond 0 1 1 0 0 0 1 1 Rn Rd SBO 0 0 1 1 Rm
			uaddsubx		cond 0 1 1 0 0 1 0 1 Rn Rd SBO 0 0 1 1 Rm
			uqaddsubx		cond 0 1 1 0 0 1 1 0 Rn Rd SBO 0 0 1 1 Rm
			uhaddsubx		cond 0 1 1 0 0 1 1 1 Rn Rd SBO 0 0 1 1 Rm
			ssat16			cond 0 1 1 0 1 0 1 0 sat_imm Rd SBO 0 0 1 1 Rm
			rev				cond 0 1 1 0 1 0 1 1 SBO Rd SBO 0 0 1 1 Rm
			usat16			cond 0 1 1 0 1 1 1 0 sat_imm Rd SBO 0 0 1 1 Rm
		B[24]=0, 子识别码=0101
			ssubaddx		cond 0 1 1 0 0 0 0 1 Rn Rd SBO 0 1 0 1 Rm
			qsubaddx		cond 0 1 1 0 0 0 1 0 Rn Rd SBO 0 1 0 1 Rm
			shsubaddx		cond 0 1 1 0 0 0 1 1 Rn Rd SBO 0 1 0 1 Rm
			usubaddx		cond 0 1 1 0 0 1 0 1 Rn Rd SBO 0 1 0 1 Rm
			uqsubaddx		cond 0 1 1 0 0 1 1 0 Rn Rd SBO 0 1 0 1 Rm
			uhsubaddx		cond 0 1 1 0 0 1 1 1 Rn Rd SBO 0 1 0 1 Rm
		B[24]=0, 子识别码=0111
			ssub16			cond 0 1 1 0 0 0 0 1 Rn Rd SBO 0 1 1 1 Rm
			qsub16			cond 0 1 1 0 0 0 1 0 Rn Rd SBO 0 1 1 1 Rm
			shsub16			cond 0 1 1 0 0 0 1 1 Rn Rd SBO 0 1 1 1 Rm
			usub16			cond 0 1 1 0 0 1 0 1 Rn Rd SBO 0 1 1 1 Rm
			uqsub16			cond 0 1 1 0 0 1 1 0 Rn Rd SBO 0 1 1 1 Rm
			uhsub16			cond 0 1 1 0 0 1 1 1 Rn Rd SBO 0 1 1 1 Rm
			sxtab16			cond 0 1 1 0 1 0 0 0 Rn Rd rotate SBZ 0 1 1 1 Rm
			sxtb16			cond 0 1 1 0 1 0 0 0 1 1 1 1 Rd rotate SBZ 0 1 1 1 Rm
			sxtab			cond 0 1 1 0 1 0 1 0 Rn Rd rotate SBZ 0 1 1 1 Rm
			sxtb			cond 0 1 1 0 1 0 1 0 1 1 1 1 Rd rotate SBZ 0 1 1 1 Rm
			sxtah			cond 0 1 1 0 1 0 1 1 Rn Rd rotate SBZ 0 1 1 1 Rm
			sxth			cond 0 1 1 0 1 0 1 1 1 1 1 1 Rd rotate SBZ 0 1 1 1 Rm
			uxtab16			cond 0 1 1 0 1 1 0 0 Rn Rd rotate SBZ 0 1 1 1 Rm
			uxtb16			cond 0 1 1 0 1 1 0 0 1 1 1 1 Rd rotate SBZ 0 1 1 1 Rm
			uxtab			cond 0 1 1 0 1 1 1 0 Rn Rd rotate SBZ 0 1 1 1 Rm
			uxtb			cond 0 1 1 0 1 1 1 0 1 1 1 1 Rd rotate SBZ 0 1 1 1 Rm
			uxtah			cond 0 1 1 0 1 1 1 1 Rn Rd rotate SBZ 0 1 1 1 Rm
			uxth			cond 0 1 1 0 1 1 1 1 1 1 1 1 Rd rotate SBZ 0 1 1 1 Rm
		B[24]=0, 子识别码=1001
			sadd8			cond 0 1 1 0 0 0 0 1 Rn Rd SBO 1 0 0 1 Rm
			qadd8			cond 0 1 1 0 0 0 1 0 Rn Rd SBO 1 0 0 1 Rm
			shadd8			cond 0 1 1 0 0 0 1 1 Rn Rd SBO 1 0 0 1 Rm
			uadd8			cond 0 1 1 0 0 1 0 1 Rn Rd SBO 1 0 0 1 Rm
			uqadd8			cond 0 1 1 0 0 1 1 0 Rn Rd SBO 1 0 0 1 Rm
			uhadd8			cond 0 1 1 0 0 1 1 1 Rn Rd SBO 1 0 0 1 Rm
		B[24]=0, 子识别码=1011
			sel				cond 0 1 1 0 1 0 0 0 Rn Rd SBO 1 0 1 1 Rm
			rev16			cond 0 1 1 0 1 0 1 1 SBO Rd SBO 1 0 1 1 Rm
			revsh			cond 0 1 1 0 1 1 1 1 SBO Rd SBO 1 0 1 1 Rm
		B[24]=0, 子识别码=1111
			ssub8			cond 0 1 1 0 0 0 0 1 Rn Rd SBO 1 1 1 1 Rm
			qsub8			cond 0 1 1 0 0 0 1 0 Rn Rd SBO 1 1 1 1 Rm
			shsub8			cond 0 1 1 0 0 0 1 1 Rn Rd SBO 1 1 1 1 Rm
			usub8			cond 0 1 1 0 0 1 0 1 Rn Rd SBO 1 1 1 1 Rm
			uqsub8			cond 0 1 1 0 0 1 1 0 Rn Rd SBO 1 1 1 1 Rm
			uhsub8			cond 0 1 1 0 0 1 1 1 Rn Rd SBO 1 1 1 1 Rm
		B[24]=0, 子识别码归纳到上面去(属于上面的情况)
			pkhbt			cond 0 1 1 0 1 0 0 0 Rn Rd shift_imm 0 0 1 Rm			0001 1001
			pkhtb			cond 0 1 1 0 1 0 0 0 Rn Rd shift_imm 1 0 1 Rm			0101 1101
			ssat			cond 0 1 1 0 1 0 1 sat_imm Rd shift_imm sh 0 1 Rm		0001 0101 1001 1101
			usat			cond 0 1 1 0 1 1 1 sat_imm Rd shift_imm sh 0 1 Rm		0001 0101 1001 1101
		B[24]=1，子识别码=xxx1
			smlad			cond 0 1 1 1 0 0 0 0 Rd Rn Rs 0 0 X 1 Rm				0001 0011
			smuad			cond 0 1 1 1 0 0 0 0 Rd 1 1 1 1 Rs 0 0 X 1 Rm			0001 0011
			smlsd			cond 0 1 1 1 0 0 0 0 Rd Rn Rs 0 1 X 1 Rm				0101 0111
			smusd		`	cond 0 1 1 1 0 0 0 0 Rd 1 1 1 1 Rs 0 1 X 1 Rm			0101 0111
			smlald			cond 0 1 1 1 0 1 0 0 RdHi RdLo Rs 0 0 X 1 Rm			0001 0011
			smlsld			cond 0 1 1 1 0 1 0 0 RdHi RdLo Rs 0 1 X 1 Rm			0101 0111
			smmla			cond 0 1 1 1 0 1 0 1 Rd Rn Rs 0 0 R 1 Rm				0001 0011
			smmls			cond 0 1 1 1 0 1 0 1 Rd Rn Rs 1 1 R 1 Rm				1101 1111
			smmul			cond 0 1 1 1 0 1 0 1 Rd 1 1 1 1 Rs 0 0 R 1 Rm			0001 0011

	识别码=010 | 子识别码=xxx0
			ldr				cond 0 1 I P U 0 W 1 Rn Rd addr_mode
			ldrb			cond 0 1 I P U 1 W 1 Rn Rd addr_mode
			ldrbt			cond 0 1 I 0 U 1 1 1 Rn Rd addr_mode
			ldrt			cond 0 1 I 0 U 0 1 1 Rn Rd addr_mode
			str				cond 0 1 I P U 0 W 0 Rn Rd addr_mode
			strb			cond 0 1 I P U 1 W 0 Rn Rd addr_mode
			strbt			cond 0 1 I 0 U 1 1 0 Rn Rd addr_mode
			strt			cond 0 1 I 0 U 0 1 0 Rn Rd addr_mode
	;

100
			ldm				cond 1 0 0 P U 0 W 1 Rn register_list
			ldm(2)			cond 1 0 0 P U 1 0 1 Rn 0 register_list
			ldm(3)			cond 1 0 0 P U 1 W 1 Rn 1 register_list
			stm(1)			cond 1 0 0 P U 0 W 0 Rn register_list
			stm(2)			cond 1 0 0 P U 1 0 0 Rn register_list
	;

101
			b,bl			cond 1 0 1 L signed_immed_24
	;

110
			ldc				cond 1 1 0 P U N W 1 Rn CRd cp_num 8_bit_word_offset
			mrrc			cond 1 1 0 0 0 1 0 1 Rn Rd cp_num opcode CRm
			stc				cond 1 1 0 P U N W 0 Rn CRd cp_num 8_bit_word_offset
			mcrr			cond 1 1 0 0 0 1 0 0 Rn Rd cp_num opcode CRm
	;

111
	B[24]=0
			cdp				cond 1 1 1 0 opcode_1 CRn CRd cp_num opcode_2 0 CRm
			mcr				cond 1 1 1 0 opcode_1 0 CRn Rd cp_num opcode_2 1 CRm
			mrc				cond 1 1 1 0 opcode_1 1 CRn Rd cp_num opcode_2 1 CRm
	B[24]=1
			swi				cond 1 1 1 1 immed_24

条件限定指令 cond=1111
			cps				cond 0 0 0 1 0 0 0 0 imod mmod 0 SBZ A I F 0 mode			[16]=0
			setend			cond 0 0 0 1 0 0 0 0 0 0 0 1 SBZ E SBZ 0 0 0 0 SBZ			[16] = 1
			pld				cond 0 1 I 1 U 1 0 1 Rn 1 1 1 1 addr_mode
			rfe				cond 1 0 0 P U 0 W 1 Rn SBZ 1 0 1 0 SBZ
			srs				cond 1 0 0 P U 1 W 0 1 1 0 1 SBZ 0 1 0 1 SBZ mode
			blx(1)			cond 1 0 1 H signed_immed_24


*/