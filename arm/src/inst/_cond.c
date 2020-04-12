#include "arm.h"

bool check_cond(cpu_state_t *st, uint32 inst)
{
	//条件检测
	switch (inst >> 28)
	{
	case 0x00000000: //eq
		return st->z;
	case 0x00000001: //ne
		return !st->z;
	case 0x00000002: //cs/hs
		return st->c;
	case 0x00000003: //cc/lo
		return !st->c;
	case 0x00000004: //mi
		return st->n;
	case 0x00000005: //pl
		return !st->n;
	case 0x00000006: //vs
		return st->v;
	case 0x00000007: //vc
		return !st->v;
	case 0x00000008: //hi
		return st->c && !st->z;
	case 0x00000009: //ls
		return !st->c && st->z;
	case 0x0000000a: //ge
		return st->n == st->v;
	case 0x0000000b: //lt
		return st->n != st->v;
	case 0x0000000c: //gt
		return st->z == 0 && st->n == st->v;
	case 0x0000000d: //le
		return st->z == 1 || st->n != st->v;
	case 0x0000000e:
		return true;
	default:
		return false;
	}
}