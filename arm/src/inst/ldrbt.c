#include "arm.h"

int32 arm_inst_ldrbt(cpu_state_t *st, uint32 inst)
{
	//TODO 先进行内存检测

	//TODO CP15_reg1_Ubit只有在内核模式下才能访问，这里不处理它

	//得到地址
	uint32 addr = addr_mode_2(st, inst);
	uint32 rn = (inst >> 16) & 0x000f;
	uint32 rd = (inst >> 12) & 0x000f;

	if (rd == r_pc)
		return EXEC_UNPREDICTABLE;

	uint8 byte = st->memory[addr]; //读取字节
	uint32 data = (uint32)byte;	   //扩充到32位

	st->registers[r_pc] = data;

	//写回(这里是直接写回)
	// TODO 未验证
	st->registers[rn] = addr;

	return EXEC_SUCCESS;
}