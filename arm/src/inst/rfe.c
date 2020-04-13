#include "arm.h"

int32 arm_inst_rfe(cpu_state_t *st, uint32 inst)
{
	loginst("ref", inst);

	//TODO 这条指令用来在特权模式中从异常返回

	return EXEC_NOT_IMPL;
}
