#include "arm.h"

int32 arm_inst_mcrr(cpu_state_t *st, uint32 inst)
{
	loginst("*mcrr", inst);

	return EXEC_NOT_IMPL;
}
