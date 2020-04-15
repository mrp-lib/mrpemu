#include "arm.h"

int32 arm_inst_srs(cpu_state_t *st, uint32 inst)
{
	loginst("srs", inst);

	//sbz
	// if (inst_bm(5, 7) != 0 || inst_b4(12) != 0)
	// 	return EXEC_UNPREDICTABLE;

	// //TODO 内存访问权限检测

	// uint32 p = inst_b1(24);
	// uint32 u = inst_b1(23);
	// uint32 w = inst_b1(21);
	// uint32 mode = inst_bm(0, 4);

	// 1 1 1 1 1 0 0 P U 1 W 0 1 1 0 1 SBZ 0 1 0 1 SBZ mode

	// return EXEC_SUCCESS;

	//TODO 暂时不实现吧

	return EXEC_NOT_IMPL;
}
