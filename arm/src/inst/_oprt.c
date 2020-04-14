#include <math.h>

#include "arm.h"

int32 signed_sat(int64 val, int32 min, int32 max, bool *issat)
{
	if (val > max)
	{
		*issat = true;
		return max;
	}
	else if (val < min)
	{
		*issat = true;
		return min;
	}
	else
	{
		*issat = false;
		return (int32)val;
	}
}

uint32 unsigned_sat(int64 val, uint32 max, bool *issat)
{
	if (val > max)
	{
		*issat = true;
		return max;
	}
	else if (val < 0)
	{
		*issat = true;
		return 0;
	}
	return (uint32)val;
}