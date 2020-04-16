#include "arm.h"

int32 arm_inst_smlal_x_y(cpu_state_t *st, uint32 inst)
{
	loginst("smlal_x_y", inst);

	if (!cond_ok())
		return EXEC_SUCCESS;

	uint32 rdh = inst_b4(16);
	uint32 rdl = inst_b4(12);
	uint32 rs = inst_b4(8);
	uint32 rm = inst_b4(0);

	uint32 x = inst_b1(5);
	uint32 y = inst_b1(6);

	int32 operand1 = (x == 0) ? (int16)bitm(regv(rm), 0, 15) : (int16)bitm(regv(rm), 16, 31);
	int32 operand2 = (y == 0) ? (int16)bitm(regv(rs), 0, 15) : (int16)bitm(regv(rs), 16, 31);

	int32 mul_op = operand1 * operand2;
	uint32 resl = regv(rdl) = regv(rdl) + mul_op;
	regv(rdh) = regv(rdh) + ((mul_op < 0) ? 0xffffffff : 0x000000000) + carry(resl, regv(rdl));

	return EXEC_SUCCESS;
}
