#include "arm.h"

bool check_cond(cpu_state_t *st, uint32 inst)
{
	//条件检测
	switch (inst >> 28)
	{
	case 0x00000000: //eq
		return st->cpsr.z;
	case 0x00000001: //ne
		return !st->cpsr.z;
	case 0x00000002: //cs/hs
		return st->cpsr.c;
	case 0x00000003: //cc/lo
		return !st->cpsr.c;
	case 0x00000004: //mi
		return st->cpsr.n;
	case 0x00000005: //pl
		return !st->cpsr.n;
	case 0x00000006: //vs
		return st->cpsr.v;
	case 0x00000007: //vc
		return !st->cpsr.v;
	case 0x00000008: //hi
		return st->cpsr.c && !st->cpsr.z;
	case 0x00000009: //ls
		return !st->cpsr.c && st->cpsr.z;
	case 0x0000000a: //ge
		return st->cpsr.n == st->cpsr.v;
	case 0x0000000b: //lt
		return st->cpsr.n != st->cpsr.v;
	case 0x0000000c: //gt
		return st->cpsr.z == 0 && st->cpsr.n == st->cpsr.v;
	case 0x0000000d: //le
		return st->cpsr.z == 1 || st->cpsr.n != st->cpsr.v;
	case 0x0000000e:
		return true;
	default:
		return false;
	}
}