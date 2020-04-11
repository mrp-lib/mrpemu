#include "arm.h"

int32 arm_inst_cps(cpu_state_t *st, uint32 inst)
{
	uint32 imod = (inst >> 18) & 0b0011;
	uint32 mmod = (inst >> 17) & 0b0001;
	if ((imod >> 1) == 1) //如果imode的第一位是1的话，会改变cpsr的A、I、F位
	{
		//TODO 这里暂时不做实现
	}
	if (mmod == 1) //如果mmod为1的话，则需要改变CPU的模式
	{
		//TODO 由于目前只有用户模式，所以不做改变
	}
	return EXEC_SUCCESS;
}