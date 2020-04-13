#include "arm.h"

int32 arm_inst_ldrb(cpu_state_t *st, uint32 inst)
{
	loginst("ldrb", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//TODO 先进行内存检测

	//TODO CP15_reg1_Ubit只有在内核模式下才能访问，这里不处理它

	//得到地址
	uint32 addr = addr_mode_2(st, inst);
	// uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	// uint32 p = inst_b1(24);
	// uint32 w = inst_b1(21);

	if (rd == r_pc)
		return EXEC_UNPREDICTABLE;

	// 如果P为0， 此时如果W为0则正常访问，如果W为1则再用户模式下访问 （因为都在用户模式下，所以不用管）
	// 如果P为1， 此时如果W为0则不更新基址寄存器，否则结果写会基址寄存器

	//官方文档是没有处理pc的，所以不用管了
	//和ldr一样处理pc
	// if (rn == r_pc)
	// {
	// 	st->registers[rd] = addr + 8;
	// }
	// else
	// {
	uint8 byte = mem_ld8(st->mem, addr); //读取字节
	uint32 data = (uint32)byte;			 //扩充到32位
	st->registers[rd] = data;
	// }

	return EXEC_SUCCESS;
}