#include "arm.h"

int32 arm_inst_ldr(cpu_state_t *st, uint32 inst)
{
	//TODO 先进行内存检测

	//TODO CP15_reg1_Ubit只有在内核模式下才能访问，这里不处理它

	//得到地址
	uint32 addr = addr_mode_2(st, inst);
	uint32 rn = (inst >> 16) & 0x000f;
	uint32 rd = (inst >> 12) & 0x000f;
	uint32 p = (inst >> 24) & 0x0001;
	uint32 w = (inst >> 21) & 0x0001;

	// 如果P为0， 此时如果W为0则正常访问，如果W为1则再用户模式下访问 （因为都在用户模式下，所以不用管）
	// 如果P为1， 此时如果W为0则不更新基址寄存器，否则结果写会基址寄存器
	uint32 data = st->memory[addr];
	if (rd == r_pc)
		st->registers[r_pc] = data & 0xfffffffc;
	else
		st->registers[r_pc] = data;

	//写回
	if (p == 1 && w == 1)
		st->registers[rn] = addr;

	return EXEC_SUCCESS;
}