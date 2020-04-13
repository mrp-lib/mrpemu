#include "arm.h"

int32 arm_inst_mrc(cpu_state_t *st, uint32 inst)
{
	loginst("*mrc", inst);

	return EXEC_NOT_IMPL;
}
