#ifndef __DSM_DSM_H__
#define __DSM_DSM_H__

/** SD卡路径 */
extern char sdcard_dir[MAX_PATH_LEN];

/** mythroad路径（相对于SD卡路径） */
extern char root_dir[MAX_PATH_LEN];

/**
 * 初始化dsm，首先应该调用此函数进行初始化
 * 参数：
 * 		_sdcard_dir		SD卡路径
 * 		_root_dir		mythroad路径（相对于SD卡路径）
 */
int32 dsm_init(char *_sdcard_dir, char *_root_dir);

#endif