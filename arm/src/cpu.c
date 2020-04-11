#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arm.h"

//arm指令匹配表（有条件）
arm_inst_table_item_t arm_inst_cond_tab[] = {
	{0x0de00000, 0x00a00000, arm_inst_adc}, //adc 指令 [ cond 0 0 I 0 1 0 1 S Rn Rd shifter_operand ]
	{0x0de00000, 0x00800000, arm_inst_add}, //add 指令 [ cond 0 0 I 0 1 0 0 S Rn Rd shifter_operand ]
	{0x0de00000, 0x00000000, arm_inst_and}, //and 指令 [ cond 0 0 I 0 0 0 0 Rn Rd shifter_operand ]
	{0x0e000000, 0x0a000000, arm_inst_b},	//b,bl 指令 [ cond 1 0 1 L signed_immed_24 ]
	{0x0de00000, 0x01c00000, arm_inst_bic}, //bic 指令 [ cond 0 0 I 1 1 1 0 S Rn Rd shifter_operand ]
	// {0x01200000, 0x01200000, arm_inst_brpt},  //brpt 指令 [ cond 0 0 I 1 1 1 0 S Rn Rd shifter_operand ]  (断点指令，未实现)
	{0x0ff000f0, 0x01200030, arm_inst_blx_2}, //blx(2) 指令 [cond 0 0 0 1 0 0 1 0 SBO SBO SBO 0 0 1 1 Rm ]
	{0x0ff000f0, 0x01200010, arm_inst_bx},	  //bx 指令 [cond 0 0 0 1 0 0 1 0 SBO SBO SBO 0 0 0 1 Rm ]
	{0x0ff000f0, 0x01200020, arm_inst_bxj},	  //bxj 指令 [cond 0 0 0 1 0 0 1 0 SBO SBO SBO 0 0 1 0 Rm ]
	// {0x0f000010, 0x0e000000, arm_inst_cdp},	  //cdp 指令 [cond 1 1 1 0 opcode_1 CRn CRd cp_num opcode_2 0 CRm ] (协处理器数据处理，未实现)
	{0x0ff000f0, 0x01600010, arm_inst_clz}, //clz 指令 [cond 0 0 0 1 0 1 1 0 SBO Rd SBO 0 0 0 1 Rm ]
	{0x0df00000, 0x01700000, arm_inst_cmn}, //cmn 指令 [cond 0 0 I 1 0 1 1 1 Rn SBZ shifter_operand ]
	{0x0df00000, 0x01500000, arm_inst_cmp}, //cmp 指令 [cond 0 0 I 1 0 1 0 1 Rn SBZ shifter_operand ]
	{0x0ff00ff0, 0x01a00000, arm_inst_cpy}, //cpy 指令 [cond 0 0 0 1 1 0 1 0 SBZ Rd 0 0 0 0 0 0 0 0 Rm ]
	{0x0de00000, 0x00200000, arm_inst_eor}, //eor 指令 [cond 0 0 I 0 0 0 1 S Rn Rd shifter_operand ]
	// {0x0e100000, 0x0c100000, arm_inst_ldc},	  //ldc 指令 [cond 0 0 I 0 0 0 1 S Rn Rd shifter_operand ] (加载数据到协处理器，未实现)
	{0x0e500000, 0x08100000, arm_inst_ldm_1}, //ldm(1) 指令 [cond 1 0 0 P U 0 W 1 Rn register_list ]
	// {0x0e700000, 0x08500000, arm_inst_ldm_2}, //ldm(2) 指令 [cond 1 0 0 P U 1 0 1 Rn register_list ] (只有在特权模式下才有，故不实现)
	// {0x0e700000, 0x08500000, arm_inst_ldm_3}, //ldm(3) 指令 [cond 1 0 0 P U 1 W 1 Rn register_list ] (同上，不实现)
	{0x0c500000, 0x04100000, arm_inst_ldr},	  //ldr 指令 [cond 0 1 I P U 0 W 1 Rn Rd addr_mode ]
	{0x0c500000, 0x04500000, arm_inst_ldrb},  //ldrb 指令 [cond 0 1 I P U 1 W 1 Rn Rd addr_mode ]
	{0x0d700000, 0x0c700000, arm_inst_ldrbt}, //ldrbt 指令 [cond 0 1 I 0 U 1 1 1 Rn Rd addr_mode ]
};
//arm指令匹配表（无条件）
arm_inst_table_item_t arm_inst_uncond_tab[] = {
	{0x0e000000, 0x0a0000000, arm_inst_blx_1}, //blx(1) 指令 [ 1 1 1 1 1 0 1 H signed_immed_24 ] (此条指令会进入到thumb模式)
	{0x0ff10020, 0x010000000, arm_inst_cps},   //cps 指令 [ 1 1 1 1 0 0 0 1 0 0 0 0 imod mmod 0 SBZ A I F 0 mode ] (CPU状态改变指令，目前只是一个空实现)
};

bool cond_val(cpu_state_t *st, uint32 cond)
{
	//条件检测
	switch (cond)
	{
	case 0x00000000: //eq
		return st->z;
	case 0x00000001: //ne
		return !st->z;
	case 0x00000002: //cs/hs
		return st->c;
	case 0x00000003: //cc/lo
		return !st->c;
	case 0x00000004: //mi
		return st->n;
	case 0x00000005: //pl
		return !st->n;
	case 0x00000006: //vs
		return st->v;
	case 0x00000007: //vc
		return !st->v;
	case 0x00000008: //hi
		return st->c && !st->z;
	case 0x00000009: //ls
		return !st->c && st->z;
	case 0x0000000a: //ge
		return st->n == st->v;
	case 0x0000000b: //lt
		return st->n != st->v;
	case 0x0000000c: //gt
		return st->z == 0 && st->n == st->v;
	case 0x0000000d: //le
		return st->z == 1 || st->n != st->v;
	case 0x0000000e:
		return true;
	default:
		return false;
	}
}

//创建CPU
cpu_state_t *cpu_create(uint32 mem_size)
{
	logi("cpu_create(size=0x%08x)", mem_size);

	//分配内存
	void *pmem = malloc(mem_size);
	if (pmem == null)
		return null;

	//创建CPU状态
	cpu_state_t *cpuState = malloc(sizeof(cpu_state_t));
	if (cpuState == null)
	{
		free(pmem);
		return null;
	}

	//初始化状态
	memset(cpuState, 0, sizeof(cpu_state_t));
	cpuState->memory = pmem;
	cpuState->mem_size = mem_size;

	//ok
	return cpuState;
}

//释放CPU
void cpu_destory(cpu_state_t *st)
{
	logi("cpu_destory(st=%p)", st);
	free(st->memory);
	free(st);
}

//取得指令
uint32 cpu_fetch_inst(cpu_state_t *st)
{
	//由于三级流水线的问题，pc应该-8才是当前的指令
	uint32 inst = st->registers[r_pc - 8]; //取到指令
	st->registers[r_pc] += 4;			   //下一跳指令
	return inst;						   //返回取到的指令
}

//执行指令
int32 cpu_exec_inst(cpu_state_t *st, uint32 inst)
{
	logi("cpu_exec_inst(st=%p, inst=0x%08x)", st, inst);
	//获取条件
	uint32 cond = inst >> 28;
	//无条件指令
	if (cond == 0x0000000f)
	{
		//取得指令数量
		uint32 inst_count = sizeof(arm_inst_uncond_tab) / sizeof(arm_inst_table_item_t);
		//遍历指令表，找到合适的指令
		for (uint32 i = 0; i < inst_count; i++)
		{
			//找到指令，则处理它
			if ((arm_inst_uncond_tab[i].match & inst) == arm_inst_uncond_tab[i].result)
				return arm_inst_uncond_tab[i].resolver(st, inst);
		}
	}
	//条件指令
	else
	{
		//条件校验不通过，忽略此指令
		if (!cond_val(st, cond))
			return EXEC_IGNORE;
		//取得指令数量
		uint32 inst_count = sizeof(arm_inst_cond_tab) / sizeof(arm_inst_table_item_t);
		//遍历指令表，找到合适的指令
		for (uint32 i = 0; i < inst_count; i++)
		{
			//找到指令，则处理它
			if ((arm_inst_cond_tab[i].match & inst) == arm_inst_cond_tab[i].result)
				return arm_inst_cond_tab[i].resolver(st, inst);
		}
	}

	return EXEC_NOT_IMPL;
}
