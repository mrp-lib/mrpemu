#include <stdlib.h>
#include <string.h>
#include "mrp.h"
#include "utils/coding.h"

uint16 *mr_c2u(char *cp, int32 *err, int32 *size)
{
	uint8 *ucp = (uint8 *)cp;
	const uint8 *pReturn;
	uint32 uniSize;
	uint32 chCount;
	uint32 i;

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

	uint16 *uniBuf = (uint16 *)malloc(uniSize);
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
						free(uniBuf);
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

/*
 * 整理路径，将分隔符统一为sep，并清除连续的多个
 *
 * 参数：路径(必须可读写)
 */
char *FormatPathString(char *path, char sep)
{
	char *p, *q;
	int flag = 0;

	if (NULL == path)
		return NULL;

	for (p = q = path; '\0' != *p; p++)
	{
		if ('\\' == *p || '/' == *p)
		{
			if (0 == flag)
				*q++ = sep;
			flag = 1;
		}
		else
		{
			*q++ = *p;
			flag = 0;
		}
	}

	*q = '\0';

	return path;
}

// 由相对路径的文件名接成绝对路径名
char *get_filename(vm_info_t *vm, char *outputbuf, char *filename)
{
	char dsmFullPath[MAX_FILE_PATH_LEN + 1] = {0};

	snprintf(dsmFullPath, sizeof(dsmFullPath), "%s%s%s", mrst.sysinfo.sdcard_dir, mrst.sysinfo.dsm_dir, filename);
	FormatPathString(dsmFullPath, '/');
	GBToUTF8String((uint8 *)&dsmFullPath, (uint8 *)outputbuf, MAX_FILE_PATH_LEN);

	return outputbuf;
}

FILE *open_file(vm_info_t *vm, char *filename, const char *mode)
{
	char fullpathname[MAX_FILE_PATH_LEN] = {0};
	get_filename(vm, fullpathname, filename);
	logi("open file %s", fullpathname);
	return fopen(fullpathname, mode);
}