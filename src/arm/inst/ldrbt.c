#include "arm.h"

int32 arm_inst_ldrbt(cpu_state_t *st, uint32 inst)
{
	loginst("ldrbt", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	//TODO 先进行内存检测

	//TODO CP15_reg1_Ubit只有在内核模式下才能访问，这里不处理它

	//得到地址
	uint32 addr = addr_mode_2(st, inst);
	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);

	println("ldrbt: %d ->r %d", addr, rd);

	if (rd == r_pc)
		return EXEC_UNPREDICTABLE;

	//同样，官方文档没有处理pc
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

	//写回(这里是直接写回)
	// TODO 未验证
	st->registers[rn] = addr;

	return EXEC_SUCCESS;
}