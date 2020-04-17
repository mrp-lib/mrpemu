#include "arm.h"

int32 arm_inst_mrrc(cpu_state_t *st, uint32 inst)
{
	loginst("*mrrc", inst);
	
	return EXEC_NOT_IMPL;
}
