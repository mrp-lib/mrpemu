#include "arm.h"

int32 arm_inst_strex(cpu_state_t *st, uint32 inst)
{
	loginst("*strex", inst);

	return EXEC_NOT_IMPL;
}
