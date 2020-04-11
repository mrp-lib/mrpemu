#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "mrp.h"

char sdcard_dir[MAX_PATH_LEN];
char root_dir[MAX_PATH_LEN];


int32 dsm_init(char *_sdcard_dir, char *_root_dir)
{
	logi("dsm_init(%s, %s)", _sdcard_dir, _root_dir);

	strcpy(sdcard_dir, _sdcard_dir);
	strcpy(root_dir, _root_dir);

	//创建SD卡目录
	if (!access(sdcard_dir, F_OK) == 0)
	{
		int ret = mkdir(sdcard_dir, 0777);
		if (ret != 0)
			return MR_FAILED;
	}
	//创建mythroad目录
	char mythroad_dir[MAX_PATH_LEN];
	sprintf(mythroad_dir, "%s%s", sdcard_dir, root_dir);
	if (!access(mythroad_dir, F_OK) == 0)
	{
		int ret = mkdir(mythroad_dir, 0777);
		if (ret != 0)
			return MR_FAILED;
	}

	return MR_SUCCESS;
}