#include "mrp.h"

uint16 *mr_c2u(char *cp, int32 *err, int32 *size)
{
	uint8 *ucp = (uint8 *)cp;
	const uint8 *pReturn;
	uint16 *uniBuf;
	uint32 uniSize;
	uint32 chCount;
	uint32 i;

	logi("mr_c2u(%p,%p,%p)", cp, err, size);

	if (null != err)
		*err = -1;

	chCount = 0;
	for (i = 0; 0 != ucp[i]; i++)
	{
		if (ucp[i] >= 0xa1 && ucp[i] <= 0xfe && 0 != ucp[i + 1])
			i++;
		chCount++;
	}

	uniSize = 2 * chCount + 2;
	if (null != size)
		*size = uniSize;

	uniBuf = mr_malloc(uniSize);
	if (null != uniBuf)
	{
		chCount = 0;
		i = 0;
		while (0 != ucp[i])
		{
			if (ucp[i] < 0x80)
			{
				uniBuf[chCount++] = ucp[i] << 8;
				i++;
			}
			else
			{
				pReturn = GBCodeToUnicode(ucp + i);
				if (null != pReturn)
				{
					uniBuf[chCount++] = (pReturn[0] << 8) + pReturn[1];
				}
				else
				{
					if (null != err)
					{
						*err = i;
						mr_free(uniBuf, uniSize);
						uniBuf = null;
						goto end;
					}
					else
					{
						uniBuf[chCount++] = 0x3000;
					}
				}
				i += 2;
			}
		}

		uniBuf[chCount] = 0;
	}

end:
	return uniBuf;
}