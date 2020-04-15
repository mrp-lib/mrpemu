#include <math.h>

#include "arm.h"

int32 signed_sat(int64 val, int32 n, bool *issat)
{
	int32 pown = powl(2, n - 1);
	int32 pown1 = pown - 1;

	if (val < -pown)
	{
		*issat = true;
		return -pown;
	}
	else if (val > pown1)
	{
		*issat = true;
		return pown1;
	}
	else
	{
		*issat = false;
		return val;
	}
}

uint32 unsigned_sat(int64 val, uint32 n, bool *issat)
{
	int32 pown = powl(2, n);
	int32 pown1 = pown - 1;

	if (val < 0)
	{
		*issat = true;
		return 0;
	}
	else if (val > pown1)
	{
		*issat = true;
		return pown1;
	}
	else
	{
		*issat = false;
		return val;
	}
}