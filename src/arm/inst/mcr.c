#include "arm.h"

int32 arm_inst_mcr(cpu_state_t *st, uint32 inst)
{
	loginst("*mcr", inst);

	return EXEC_NOT_IMPL;
}
