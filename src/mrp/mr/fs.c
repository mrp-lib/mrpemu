#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#include "mrp.h"
#include "utils/coding.h"

#ifdef _WIN32
#define _mkdir mkdir
#else
#define _mkdir(dirname) mkdir((dirname), 0777)
#endif

void swi_mr_ferrno(vm_info_t *vm)
{
	//原型： (void) -> int32
	logi("mr_ferrno()");
	mr_ret(MR_SUCCESS); //不处理
}

void swi_mr_open(vm_info_t *vm)
{
	//原型： (const char *filename, uint32 mode) -> int32
	vmpt _filename = vmreg(0);
	uint32 mode = vmreg(1);
	char *filename = vmpt_real(char, _filename);
	logsysc("mr_open(filename=%s, mode=%d)", filename, mode);

	int new_mode = 0;
	if (mode & MR_FILE_RDONLY)
		new_mode = O_RDONLY;
	if (mode & MR_FILE_WRONLY)
		new_mode = O_WRONLY;
	if (mode & MR_FILE_RDWR)
		new_mode = O_RDWR;

	char fullpathname[MAX_FILE_PATH_LEN] = {0};
	get_filename(vm, fullpathname, filename);

	//如果文件存在 带此标志会导致错误
	if ((mode & MR_FILE_CREATE) && (0 != access(fullpathname, F_OK)))
		new_mode |= O_CREAT;

	//返回值：成功则返回文件描述符，否则返回 -1
	int f = open(fullpathname, new_mode, 0777);
	if (f < 0)
	{
		loge("mr_open failed with filename=%s", fullpathname);
		mr_ret(0);
		return;
	}

	f += 5; //因为 linux 返回0也成功，mrp返回0为失败！所以统一加5

	mr_ret(f);
}

void swi_mr_close(vm_info_t *vm)
{
	//原型： (int32 f) -> int32
	int32 f = vmreg(0);
	logsysc("mr_close(f=%d)", f);

	if (f == 0)
	{
		mr_ret(MR_FAILED);
		return;
	}
	int ret = close(f - 5);
	if (ret != 0)
	{
		mr_ret(MR_FAILED);
		return;
	}

	mr_ret(MR_SUCCESS);
}

void swi_mr_info(vm_info_t *vm)
{
	//原型： (const char *filename) -> int32
	char *filename = vmpt_real(char, vmreg(0));
	logsysc("mr_info(filename=%s)", filename);

	char fullpathname[MAX_FILE_PATH_LEN] = {0};

	struct stat s1;
	int ret = stat(get_filename(vm, fullpathname, filename), &s1);

	if (ret != 0)
	{
		mr_ret(MR_IS_INVALID);
		return;
	}

	if (S_ISDIR(s1.st_mode))
		mr_ret(MR_IS_DIR);
	else if (S_ISREG(s1.st_mode))
		mr_ret(MR_IS_FILE);
	else
		mr_ret(MR_IS_INVALID);
}

void swi_mr_write(vm_info_t *vm)
{
	//原型： (int32 f, void *p, uint32 l) -> int32
	int32 f = vmreg(0);
	vmpt _p = vmreg(1);
	uint32 l = vmreg(2);
	logsysc("mr_write(f=%d, p=0x%08x, l=%d)", f, _p, l);
	void *p = vmpt_real(void, _p);

	if (f < 0)
	{
		mr_ret(MR_FAILED);
		return;
	}

	mr_ret(write(f - 5, p, l));
}

void swi_mr_read(vm_info_t *vm)
{
	//原型： (int32 f, void *p, uint32 l) -> int32
	int32 f = vmreg(0);
	vmpt _p = vmreg(1);
	uint32 l = vmreg(2);
	logsysc("mr_read(f=%d, p=0x%08x, l=%d)", f, _p, l);
	void *p = vmpt_real(void, _p);

	if (f < 5)
	{
		mr_ret(MR_FAILED);
		return;
	}

	mr_ret(read(f - 5, p, l));
}

void swi_mr_seek(vm_info_t *vm)
{
	//原型： (int32 f, int32 pos, int method) -> int32
	int32 f = vmreg(0);
	int32 pos = vmreg(1);
	int32 method = vmreg(2);
	logsysc("mr_seek(f=%d, pos=%d, method=%d)", f, pos, method);

	if (f == 0)
	{
		mr_ret(MR_FAILED);
		return;
	}

	int32 ret = lseek(f - 5, pos, method);

	mr_ret((ret < 0) ? MR_FAILED : MR_SUCCESS);
}

void swi_mr_getLen(vm_info_t *vm)
{
	//原型： (const char *filename) -> int32
	vmpt _filename = vmreg(0);
	char *filename = vmpt_real(char, _filename);
	logsysc("mr_getLen(filename=%s)", filename);

	char fullpathname[MAX_FILE_PATH_LEN] = {0};
	struct stat s1;

	int ret = stat(get_filename(vm, fullpathname, filename), &s1);

	if (ret != 0)
	{
		mr_ret(MR_FAILED);
		return;
	}

	mr_ret(s1.st_size);
}

void swi_mr_remove(vm_info_t *vm)
{
	//原型： (const char *filename) -> int32
	vmpt _filename = vmreg(0);
	char *filename = vmpt_real(char, _filename);
	logsysc("mr_remove(filename=%s)", filename);

	char fullpathname[MAX_FILE_PATH_LEN] = {0};
	int ret;

	ret = remove(get_filename(vm, fullpathname, filename));
	if (ret != 0 && errno != 2)
	{
		mr_ret(MR_FAILED);
		return;
	}

	mr_ret(MR_SUCCESS);
}

void swi_mr_rename(vm_info_t *vm)
{
	//原型： (const char *oldname, const char *newname) -> int32
	char *oldname = vmpt_real(char, vmreg(0));
	char *newname = vmpt_real(char, vmreg(1));
	logsysc("mr_rename(oldname=%s, newname=%s)", oldname, newname);

	char fullpathname_1[MAX_FILE_PATH_LEN] = {0};
	char fullpathname_2[MAX_FILE_PATH_LEN] = {0};

	get_filename(vm, fullpathname_1, oldname);
	get_filename(vm, fullpathname_2, newname);
	int ret = rename(fullpathname_1, fullpathname_2);

	mr_ret((ret == 0) ? MR_SUCCESS : MR_FAILED);
}

void swi_mr_mkDir(vm_info_t *vm)
{
	//原型： (const char *dirname) -> int32
	char *dirname = vmpt_real(char, vmreg(0));
	logsysc("mr_mkDir(dirname=%s)", dirname);

	char fullpathname[MAX_FILE_PATH_LEN] = {0};
	get_filename(vm, fullpathname, dirname);

	//检测是否已存在
	if (access(fullpathname, F_OK) == 0)
	{
		mr_ret(MR_SUCCESS);
		return;
	}

	int ret = _mkdir(fullpathname);

	mr_ret((ret == 0) ? MR_SUCCESS : MR_FAILED);
}

void swi_mr_rmDir(vm_info_t *vm)
{
	//原型： (const char *dirname) -> int32
	char *dirname = vmpt_real(char, vmreg(0));
	logsysc("mr_rmDir(dirname=%s)", dirname);

	char fullpathname[MAX_FILE_PATH_LEN] = {0};
	char fullpathname2[MAX_FILE_PATH_LEN] = {0};

	get_filename(vm, fullpathname, dirname);

	if (strcasecmp(fullpathname, mrst.sysinfo.sdcard_dir) == 0)
	{
		mr_ret(MR_FAILED);
		return;
	}

	sprintf(fullpathname2, "%s%s", mrst.sysinfo.sdcard_dir, mrst.sysinfo.dsm_dir);
	if (strcasecmp(fullpathname, fullpathname2) == 0)
	{
		mr_ret(MR_FAILED);
		return;
	}

	int ret = rmdir(fullpathname);

	mr_ret((ret == 0) ? MR_SUCCESS : MR_FAILED);
}

void swi_mr_findStart(vm_info_t *vm)
{
	//原型： (const char *dirname, char *buffer, uint32 len) -> int32
	char *dirname = vmpt_real(char, vmreg(0));
	vmpt _buffer = vmreg(1);
	char *buffer = vmpt_real(char, _buffer);
	uint32 len = vmreg(2);
	logsysc("mr_findStart(dirname=%s, buffer=0x%08x, len=%d)", dirname, _buffer, len);

	if (!dirname || !buffer || len == 0)
	{
		mr_ret(MR_FAILED);
		return;
	}

	DIR *pDir;
	struct dirent *pDt;
	char fullpathname[MAX_FILE_PATH_LEN] = {0};

	memset(buffer, 0, len);
	if ((pDir = opendir(get_filename(vm, fullpathname, dirname))) != NULL)
	{
		if ((pDt = readdir(pDir)) != NULL)
		{
			UTF8ToGBString((uint8 *)pDt->d_name, (uint8 *)buffer, len);
		}

		mr_ret((int32)pDir);
		return;
	}

	mr_ret(MR_FAILED);
}

void swi_mr_findGetNext(vm_info_t *vm)
{
	//原型： (int32 search_handle, char *buffer, uint32 len) -> int32
	int32 handle = vmreg(0);
	vmpt _buffer = vmreg(1);
	char *buffer = vmpt_real(char, _buffer);
	uint32 len = vmreg(2);
	logsysc("mr_findGetNext(handle=%d, buffer=0x%08x, len=%d)", handle, _buffer, len);

	if (!handle || handle == MR_FAILED || !buffer || len == 0)
	{
		mr_ret(MR_FAILED);
		return;
	}

	DIR *pDir = (DIR *)handle;
	struct dirent *pDt;

	memset(buffer, 0, len);
	if ((pDt = readdir(pDir)) != NULL)
	{
		UTF8ToGBString((uint8 *)pDt->d_name, (uint8 *)buffer, len);
		mr_ret(MR_SUCCESS);
		return;
	}
	mr_ret(MR_FAILED);
}

void swi_mr_findStop(vm_info_t *vm)
{
	//原型： (int32 search_handle) -> int32
	int32 handle = vmreg(0);
	logsysc("mr_findStop(handle=%d)", handle);

	if (!handle || handle == MR_FAILED)
	{
		mr_ret(MR_FAILED);
		return;
	}

	DIR *pDir = (DIR *)handle;

	mr_ret(closedir(pDir));
}

void swi_mr_readFile(vm_info_t *vm)
{
	//原型： (const char *filename, int32 *filelen, int32 lookfor) -> void*
	char *filename = vmpt_real(char, vmreg(0));
	vmpt _filelen = vmreg(1);
	int32 lookfor = vmreg(2);
	uint32 *filelen = vmpt_real(uint32, _filelen);
	logsysc("mr_readFile(filename=%s, filelen=0x%08x, lookfor=%d)", filename, _filelen, lookfor);

	uint8 *buf = read_mrp_file(vm, filename, filelen, lookfor);
	//空处理
	if (buf == null)
	{
		mr_ret(0);
	}
	//如果只是为了看看是否存在，则直接返回了
	else if (lookfor)
	{
		mr_ret(1);
	}
	//否则需要拷贝一下内存
	else
	{
		vmpt addr = mem_malloc(vm, *filelen);
		memcpy(vmpt_real(uint8, addr), buf, *filelen);
		free(buf);
		mr_ret(addr);
	}
}