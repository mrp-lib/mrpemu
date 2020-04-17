#include "arm.h"

int32 arm_inst_pld(cpu_state_t *st, uint32 inst)
{
	loginst("*pld", inst);
	return EXEC_NOT_IMPL;
}
