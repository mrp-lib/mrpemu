#include "arm.h"

int32 arm_inst_sadd8(cpu_state_t *st, uint32 inst)
{
	loginst("sadd8", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;
	//sbo
	if (inst_b4(8) != 0xf)
		return EXEC_UNPREDICTABLE;

	//必要值
	uint32 rn = inst_b4(16);
	uint32 rd = inst_b4(12);
	uint32 rm = inst_b4(0);

	uint8 ge = 0b0000;
	int32 sum;
	uint32 result = 0x00000000;

	int8 rn0 = bitm(regv(rn), 0, 7);
	int8 rn8 = bitm(regv(rn), 8, 15);
	int8 rn16 = bitm(regv(rn), 16, 23);
	int8 rn24 = bitm(regv(rn), 24, 31);

	int8 rm0 = bitm(regv(rm), 0, 7);
	int8 rm8 = bitm(regv(rm), 8, 15);
	int8 rm16 = bitm(regv(rm), 16, 23);
	int8 rm24 = bitm(regv(rm), 24, 31);

	sum = rn0 + rm0;
	result |= (sum & 0x000000ff);
	if (sum >= 0)
		ge |= 0b0001;

	sum = rn8 + rm8;
	result |= (sum & 0x000000ff) << 8;
	if (sum >= 0)
		ge |= 0b0010;

	sum = rn16 + rm16;
	result |= (sum & 0x000000ff) << 16;
	if (sum >= 0)
		ge |= 0b0100;

	sum = rn24 + rm24;
	result |= (sum & 0x000000ff) << 24;
	if (sum >= 0)
		ge |= 0b1000;

	regv(rd) = result;
	st->cpsr.ge = ge;

	return EXEC_SUCCESS;
}
