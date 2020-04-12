#include "arm.h"

int32 arm_inst_brpt(cpu_state_t *st, uint32 inst)
{
	loginst("*brpt", inst);

	return EXEC_NOT_IMPL;
}
