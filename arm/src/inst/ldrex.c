#include "arm.h"

int32 arm_inst_ldrex(cpu_state_t *st, uint32 inst)
{
	loginst("*ldrex", inst);

	return EXEC_NOT_IMPL;
}
