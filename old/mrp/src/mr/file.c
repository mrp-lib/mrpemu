#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/errno.h>

#include "mrp.h"

#define MR_MAX_FILENAME_SIZE 128
#define MR_MAX_FILE_SIZE 1024000

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
char *get_filename(char *outputbuf, const char *filename)
{
	char dsmFullPath[MAX_PATH_LEN + 1];

	snprintf(dsmFullPath, sizeof(dsmFullPath), "%s%s%s", sdcard_dir, root_dir, filename);
	FormatPathString(dsmFullPath, '/');
	GBToUTF8String((uint8 *)&dsmFullPath, (uint8 *)outputbuf, MAX_PATH_LEN);

	return outputbuf;
}

// 打开文件
MR_FILE_HANDLE mr_open(const char *filename, uint32 mode)
{
	logi("mr_open(%s, %d)", filename, mode);

	int new_mode = 0;
	char fullpathname[DSM_MAX_FILE_LEN] = {0};

	if (mode & MR_FILE_RDONLY)
		new_mode = O_RDONLY;
	if (mode & MR_FILE_WRONLY)
		new_mode = O_WRONLY;
	if (mode & MR_FILE_RDWR)
		new_mode = O_RDWR;

	get_filename(fullpathname, filename);

	//如果文件存在 带此标志会导致错误
	if ((mode & MR_FILE_CREATE) && (0 != access(fullpathname, F_OK)))
		new_mode |= O_CREAT;

	//返回值：成功则返回文件描述符，否则返回 -1
	int f = open(fullpathname, new_mode, 0777);
	if (f < 0)
	{
		return (MR_FILE_HANDLE)0;
	}

	f += 5; //因为 linux 返回0也成功，mrp返回0为失败！所以统一加5

	return (MR_FILE_HANDLE)(f);
}

// 关闭文件
int32 mr_close(MR_FILE_HANDLE f)
{
	logi("mr_close(%d)", f);

	if (f == 0)
		return MR_FAILED;
	int ret = close(f - 5);
	if (ret != 0)
	{
		return MR_FAILED;
	}
	return MR_SUCCESS;
}

// 读取文件
int32 mr_read(MR_FILE_HANDLE f, void *p, uint32 l)
{
	logi("mr_read(%d, %p, %d)", f, p, l);

	if (f < 5)
		return MR_FAILED;

	return read(f - 5, p, (size_t)l);
}

// 写入文件
int32 mr_write(MR_FILE_HANDLE f, void *p, uint32 l)
{
	logi("mr_write(%d, %p, %d)", f, p, l);
	if (f < 0)
		return MR_FAILED;

	return write(f - 5, p, (size_t)l);
}

// 偏移文件读写指针
int32 mr_seek(MR_FILE_HANDLE f, int32 pos, int method)
{
	logi("mr_seek(%d, %d, %d)", f, pos, method);

	if (f == 0)
		return MR_FAILED;

	off_t ret = lseek(f - 5, (off_t)pos, method);
	if (ret < 0)
	{
		return MR_FAILED;
	}

	return MR_SUCCESS;
}

// 得到文件信息
int32 mr_info(const char *filename)
{
	logi("mr_info(%s)", filename);
	char fullpathname[DSM_MAX_FILE_LEN] = {0};

	struct stat s1;
	int ret = stat(get_filename(fullpathname, filename), &s1);

	if (ret != 0)
		return MR_IS_INVALID;

	if (s1.st_mode & S_IFDIR)
		return MR_IS_DIR;
	else if (s1.st_mode & S_IFREG)
		return MR_IS_FILE;

	else
		return MR_IS_INVALID;
}

// 删除文件
int32 mr_remove(const char *filename)
{
	logi("mr_remove(%s)", filename);

	char fullpathname[DSM_MAX_FILE_LEN] = {0};
	int ret;

	ret = remove(get_filename(fullpathname, filename));
	if (ret != 0 && errno != 2)
	{
		return MR_FAILED;
	}

	return MR_SUCCESS;
}

// 文件重命名
int32 mr_rename(const char *oldname, const char *newname)
{
	logi("mr_rename(%s, %s)", oldname, newname);

	char fullpathname_1[DSM_MAX_FILE_LEN] = {0};
	char fullpathname_2[DSM_MAX_FILE_LEN] = {0};

	get_filename(fullpathname_1, oldname);
	get_filename(fullpathname_2, newname);
	int ret = rename(fullpathname_1, fullpathname_2);
	if (ret != 0)
	{
		return MR_FAILED;
	}

	return MR_SUCCESS;
}

// 创建目录
int32 mr_mkDir(const char *name)
{
	logi("mr_mkdir(%s)", name);

	char fullpathname[DSM_MAX_FILE_LEN] = {0};
	get_filename(fullpathname, name);

	//检测是否已存在
	if (access(fullpathname, F_OK) == 0)
		return MR_SUCCESS;

	int ret = mkdir(fullpathname, 0777);
	if (ret != 0)
		return MR_FAILED;

	return MR_SUCCESS;
}

// 删除目录
int32 mr_rmDir(const char *name)
{
	loge("mr_rmDir(%s)", name);

	char fullpathname[DSM_MAX_FILE_LEN] = {0};
	char fullpathname2[DSM_MAX_FILE_LEN] = {0};

	get_filename(fullpathname, name);

	//删除权限
	if (strcasecmp(fullpathname, sdcard_dir) == 0)
	{
		return MR_FAILED;
	}

	sprintf(fullpathname2, "%s%s", sdcard_dir, root_dir);
	if (strcasecmp(fullpathname, fullpathname2) == 0)
	{
		return MR_FAILED;
	}

	int ret = rmdir(fullpathname);
	if (ret != 0)
	{
		return MR_FAILED;
	}

	return MR_SUCCESS;
}

// 初始化一个文件目录的搜索，并返回第一搜索
MR_FILE_HANDLE mr_findStart(const char *name, char *buffer, uint32 len)
{
	logi("mr_findStart(%s, %p, %d)", name, buffer, len);

	if (!name || !buffer || len == 0)
		return MR_FAILED;

	DIR *pDir;
	struct dirent *pDt;
	char fullpathname[DSM_MAX_FILE_LEN] = {0};

	memset(buffer, 0, len);
	if ((pDir = opendir(get_filename(fullpathname, name))) != NULL)
	{
		if ((pDt = readdir(pDir)) != NULL)
		{
			UTF8ToGBString((uint8 *)pDt->d_name, (uint8 *)buffer, len);
		}

		return (MR_FILE_HANDLE)pDir;
	}

	return MR_FAILED;
}

// 搜索目录的下一个结果
int32 mr_findGetNext(MR_FILE_HANDLE search_handle, char *buffer, uint32 len)
{
	logi("mr_findStart(%d, %p, %d)", search_handle, buffer, len);

	if (!search_handle || search_handle == MR_FAILED || !buffer || len == 0)
		return MR_FAILED;

	DIR *pDir = (DIR *)search_handle;
	struct dirent *pDt;

	memset(buffer, 0, len);
	if ((pDt = readdir(pDir)) != NULL)
	{

		UTF8ToGBString((uint8 *)pDt->d_name, (uint8 *)buffer, len);
		return MR_SUCCESS;
	}

	return MR_FAILED;
}

// 停止当前的搜索
int32 mr_findStop(MR_SEARCH_HANDLE search_handle)
{
	logi("mr_findStop(%d)", search_handle);
	if (!search_handle || search_handle == MR_FAILED)
		return MR_FAILED;

	DIR *pDir = (DIR *)search_handle;
	closedir(pDir);

	return MR_SUCCESS;
}

// 返回的是最后一次操作文件失败的错误信息
int32 mr_ferrno(void)
{
	logi("mr_ferrno");
	return (int32)MR_FAILED;
}

// 得到指定文件得大小
int32 mr_getLen(const char *filename)
{
	logi("mr_getLen(%s)", filename);
	char fullpathname[DSM_MAX_FILE_LEN] = {0};
	struct stat s1;

	int ret = stat(get_filename(fullpathname, filename), &s1);

	if (ret != 0)
		return -1;

	return s1.st_size;
}

void *_mr_readFile(const char *filename, int *filelen, int lookfor)
{
	//int ret;
	int method;
	uint32 reallen, found = 0;
	int32 oldlen, nTmp;
	uint32 len;
	void *filebuf;
	MR_FILE_HANDLE f;
	char TempName[MR_MAX_FILENAME_SIZE];
	char *mr_m0_file;
	int is_rom_file = false;

	println("_mr_readFile: pack_filename=%s, filename=%s", pack_filename, filename);

	if ((pack_filename[0] == '*') || (pack_filename[0] == '$')) /*m0 file or ram file?*/
	{															/*read file from m0*/
		uint32 pos = 0;
		uint32 m0file_len;

		if (pack_filename[0] == '*')
		{															   /*m0 file?*/
			mr_m0_file = (char *)mr_m0_files[pack_filename[1] - 0x41]; //这里定义文件名为*A即是第一个m0文件
																	   //*B是第二个.........
		}
		else
		{
			mr_m0_file = mr_ram_file;
		}

		if (mr_m0_file == null)
		{
			if ((char *)mr_m0_files[pack_filename[1] - 0x41] == null)
				MRDBGPRINTF("mr_m0_files[%d] nil!", pack_filename[1] - 0x41);
			if (mr_ram_file == null)
				MRDBGPRINTF("mr_ram_file nil!");
			_mr_readFileShowInfo(filename, 1001);
			return 0;
		}
		pos = pos + 4;
		memcpy(&len, &mr_m0_file[pos], 4);
		pos = pos + 4;

		if ((pack_filename[0] == '$'))
		{
			m0file_len = mr_ram_file_len;
		}
		else
		{
			memcpy(&m0file_len, &mr_m0_file[pos], 4);
		}

		pos = pos + len;
		while (!found)
		{
			if (((pos + 4) >= m0file_len) || (len < 1) || (len >= MR_MAX_FILE_SIZE))
			{
				_mr_readFileShowInfo(filename, 1004);
				return 0;
			}
			memcpy(&len, &mr_m0_file[pos], 4);

			pos = pos + 4;
			if (((len + pos) >= m0file_len) || (len < 1) || (len >= MR_MAX_FILENAME_SIZE))
			{
				_mr_readFileShowInfo(filename, 1002);
				return 0;
			}
			memset(TempName, 0, sizeof(TempName));
			memcpy(TempName, &mr_m0_file[pos], len);
			pos = pos + len;
			if (strcmp(filename, TempName) == 0)
			{
				if (lookfor == 1)
				{
					return (void *)1;
				}
				found = 1;
				memcpy(&len, &mr_m0_file[pos], 4);

				pos = pos + 4;
				if (((len + pos) > m0file_len) || (len < 1) || (len >= MR_MAX_FILE_SIZE))
				{
					_mr_readFileShowInfo(filename, 1003);
					return 0;
				}
			}
			else
			{
				memcpy(&len, &mr_m0_file[pos], 4);
				pos = pos + 4 + len;
			}
		}

		*filelen = len;
		if (*filelen <= 0)
		{
			_mr_readFileShowInfo(filename, 1005);
			return 0;
		}

		if (lookfor == 2)
		{
			return (void *)&mr_m0_file[pos];
		}
		filebuf = &mr_m0_file[pos];
		is_rom_file = true;
	}
	else /*read file from efs , EFS 中的文件*/
	{
		f = mr_open(pack_filename, MR_FILE_RDONLY);
		if (f == 0)
		{
			_mr_readFileShowInfo(filename, 2002);
			return 0;
		}

		// 从这里开始是新版的mrp处理
		{
			uint32 headbuf[4];

			memset(headbuf, 0, sizeof(headbuf));
			nTmp = mr_read(f, &headbuf, sizeof(headbuf));
			if ((nTmp != 16) || (headbuf[0] != 1196446285))
			{
				mr_close(f);
				_mr_readFileShowInfo(filename, 3001);
				return 0;
			}
			if (headbuf[1] <= 232)
			{
				mr_close(f);
				_mr_readFileShowInfo(filename, 3051);
				return 0;
			}
			{
				//新版mrp
				uint32 indexlen = headbuf[1] + 8 - headbuf[3];
				uint8 *indexbuf = mr_malloc(indexlen);
				uint32 pos = 0;
				uint32 file_pos, file_len;
				if (!indexbuf)
				{
					mr_close(f);
					_mr_readFileShowInfo(filename, 3003);
					return 0;
				}
				nTmp = mr_seek(f, headbuf[3] - 16, MR_SEEK_CUR);
				if (nTmp < 0)
				{
					mr_close(f);
					mr_free(indexbuf, indexlen);
					_mr_readFileShowInfo(filename, 3002);
					return 0;
				}

				nTmp = mr_read(f, indexbuf, indexlen);

				if ((nTmp != (int32)indexlen))
				{
					mr_close(f);
					mr_free(indexbuf, indexlen);
					_mr_readFileShowInfo(filename, 3003);
					return 0;
				}

				while (!found)
				{
					memcpy(&len, &indexbuf[pos], 4);
					pos = pos + 4;
					if (((len + pos) > indexlen) || (len < 1) || (len >= MR_MAX_FILENAME_SIZE))
					{
						mr_close(f);
						mr_free(indexbuf, indexlen);
						_mr_readFileShowInfo(filename, 3004);
						return 0;
					}
					memset(TempName, 0, sizeof(TempName));
					memcpy(TempName, &indexbuf[pos], len);
					pos = pos + len;

					if (strcmp(filename, TempName) == 0)
					{
						if (lookfor == 1)
						{
							mr_close(f);
							mr_free(indexbuf, indexlen);
							return (void *)1;
						}
						found = 1;
						memcpy(&file_pos, &indexbuf[pos], 4);
						pos = pos + 4;
						memcpy(&file_len, &indexbuf[pos], 4);
						pos = pos + 4;

						if ((file_pos + file_len) > headbuf[2])
						{
							mr_close(f);
							mr_free(indexbuf, indexlen);
							_mr_readFileShowInfo(filename, 3005);
							return 0;
						}
					}
					else
					{
						pos = pos + 12;
						if (pos >= indexlen)
						{
							mr_close(f);
							mr_free(indexbuf, indexlen);
							_mr_readFileShowInfo(filename, 3006);
							return 0;
						}
					}
				}

				mr_free(indexbuf, indexlen);

				*filelen = file_len;

				filebuf = mr_malloc((uint32)*filelen);
				if (filebuf == NULL)
				{
					mr_close(f);
					_mr_readFileShowInfo(filename, 3007);
					return 0;
				}

				nTmp = mr_seek(f, file_pos, MR_SEEK_SET);
				if (nTmp < 0)
				{
					mr_free(filebuf, *filelen);
					mr_close(f);
					_mr_readFileShowInfo(filename, 3008);
					return 0;
				}

				oldlen = 0;

				while (oldlen < *filelen)
				{
					nTmp = mr_read(f, (char *)filebuf + oldlen, *filelen - oldlen);

					if (nTmp <= 0)
					{
						mr_free(filebuf, *filelen);
						mr_close(f);
						_mr_readFileShowInfo(filename, 3009);
						return 0;
					}
					oldlen = oldlen + nTmp;
				}

				mr_close(f);
			}
		}
	}

	mr_gzInBuf = filebuf;
	LG_gzoutcnt = 0;
	LG_gzinptr = 0;

	method = mr_get_method(*filelen);
	if (method < 0)
	{
		return filebuf;
	}

	reallen = (uint32)(((uch *)filebuf)[*filelen - 4]);
	reallen |= (uint32)(((uch *)filebuf)[*filelen - 3]) << 8;
	reallen |= (uint32)(((uch *)filebuf)[*filelen - 2]) << 16;
	reallen |= (uint32)(((uch *)filebuf)[*filelen - 1]) << 24;

	mr_gzOutBuf = mr_malloc(reallen);
	oldlen = *filelen;
	*filelen = reallen;

	if (mr_gzOutBuf == null)
	{
		if (!is_rom_file)
			mr_free(mr_gzInBuf, oldlen);
		return 0;
	}

	if (mr_unzip() != 0)
	{
		if (!is_rom_file)
			mr_free(mr_gzInBuf, oldlen);
		mr_free(mr_gzOutBuf, reallen);
		MRDBGPRINTF("_mr_readFile: \"%s\" Unzip err!", filename);
		return 0;
	}

	if (!is_rom_file)
		mr_free(mr_gzInBuf, oldlen);

	return mr_gzOutBuf;
}