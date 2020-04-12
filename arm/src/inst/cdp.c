#include "arm.h"

int32 arm_inst_cdp(cpu_state_t *st, uint32 inst)
{
	loginst("*cdp", inst);

	return EXEC_NOT_IMPL;
}
