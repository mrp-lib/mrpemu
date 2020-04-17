#include <math.h>
#include "arm.h"

int32 arm_inst_usad8(cpu_state_t *st, uint32 inst)
{
	loginst("usad8", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rd = inst_b4(16);
	uint32 rn = inst_b4(12);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);

	uint8 rm0 = bitm(regv(rm), 0, 7);
	uint8 rm8 = bitm(regv(rm), 8, 15);
	uint8 rm16 = bitm(regv(rm), 16, 23);
	uint8 rm24 = bitm(regv(rm), 24, 31);

	uint8 rs0 = bitm(regv(rs), 0, 7);
	uint8 rs8 = bitm(regv(rs), 8, 15);
	uint8 rs16 = bitm(regv(rs), 16, 23);
	uint8 rs24 = bitm(regv(rs), 24, 31);

	uint32 diff1 = (rm0 < rs0) ? (rs0 - rm0) : (rm0 - rs0);
	uint32 diff2 = (rm8 < rs8) ? (rs8 - rm8) : (rm8 - rs8);
	uint32 diff3 = (rm16 < rs16) ? (rs16 - rm16) : (rm16 - rs16);
	uint32 diff4 = (rm24 < rs24) ? (rs24 - rm24) : (rm24 - rs24);

	regv(rd) = regv(rn) + diff1 + diff2 + diff3 + diff4;

	return EXEC_SUCCESS;
}
