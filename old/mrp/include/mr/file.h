#ifndef __MR_FILE_H__
#define __MR_FILE_H__

#include "common/type.h"

//最大文件长度
#define DSM_MAX_FILE_LEN 256

//文件句柄
#define MR_FILE_HANDLE int32

//文件夹搜索句柄
#define MR_SEARCH_HANDLE int32

#define MR_FILE_RDONLY 1	//以只读的方式打开文件。
#define MR_FILE_WRONLY 2	//以只写的方式打开文件。
#define MR_FILE_RDWR 4		//以读写的方式打开文件。
#define MR_FILE_CREATE 8	//如果文件不存在，创建该文件。
#define MR_FILE_RECREATE 16 //无论文件存不存在，都重新创建该文件。

#define MR_IS_FILE 1	//文件
#define MR_IS_DIR 2		//目录
#define MR_IS_INVALID 8 //无效(非文件、非目录)

#define _mr_readFileShowInfo(f, n)                       \
	do                                                   \
	{                                                    \
		mr_printf("_mr_readFile:%s err, code=%d", f, n); \
	} while (0)

enum
{
	MR_SEEK_SET,
	MR_SEEK_CUR,
	MR_SEEK_END
};

typedef struct
{
	char magic[4];
	uint32 info_size;
	uint32 mrp_file_size;
	uint32 list_offset;
} mrp_head_st;

typedef int32 (*T_mr_ferrno)(void);
typedef int32 (*T_mr_open)(const char *filename, uint32 mode);
typedef int32 (*T_mr_close)(int32 f);
typedef int32 (*T_mr_info)(const char *filename);
typedef int32 (*T_mr_write)(int32 f, void *p, uint32 l);
typedef int32 (*T_mr_read)(int32 f, void *p, uint32 l);
typedef int32 (*T_mr_seek)(int32 f, int32 pos, int method);
typedef int32 (*T_mr_getLen)(const char *filename);
typedef int32 (*T_mr_remove)(const char *filename);
typedef int32 (*T_mr_rename)(const char *oldname, const char *newname);
typedef int32 (*T_mr_mkDir)(const char *name);
typedef int32 (*T_mr_rmDir)(const char *name);
typedef int32 (*T_mr_findStart)(const char *name, char *buffer, uint32 len);
typedef int32 (*T_mr_findGetNext)(int32 search_handle, char *buffer, uint32 len);
typedef int32 (*T_mr_findStop)(int32 search_handle);

typedef void *(*T__mr_readFile)(const char *filename, int32 *filelen, int32 lookfor);

/**
 * 由相对路径的文件名接成绝对路径名
 * 参数:
 * 	filename:相对路径的文件名
 * outputbuf:转换好的绝对路径文件名(outputbuf的大小要大于等于DSM_MAX_FILE_LEN * ENCODING_LENGTH)
 * 返回:
 * 	绝对路径的文件名
 */
char *get_filename(char *outputbuf, const char *filename);

/**
 * 打开一个文件
 * 参数:
 * 	filename:文件名
 * 	mode:打开方式
 * 返回:
 * 	>=0		文件句柄
 * 	<0		失败
 */
MR_FILE_HANDLE mr_open(const char *filename, uint32 mode);

/**
 * 关闭一个文件
 * 参数:
 * 	fh		要关闭得文件得句柄
 * 返回:
 * 	MR_SUCCESS		成功
 * 	MR_FAILED		失败
 */
int32 mr_close(MR_FILE_HANDLE fh);

/**
 * 读取文件中得数据
 * 参数:
 * 	fh		要读得文件得句柄
 * 	buffer	缓存得指针
 * 	len		缓存得大小
 * 返回:
 * 	MR_FAILED	失败
 * 	>=0			读取到的文件长度
 */
int32 mr_read(MR_FILE_HANDLE fh, void *buffer, uint32 len);

/**
 * 往一个文件中写入数据
 * 参数:
 * 	fh		要写入得文件得句柄
 * 	data	缓存得指针
 * 	len		要写入数据得大小
 * 返回
 * 	MR_FAILED		失败
 * 	>=0				写入的长度
 */
int32 mr_write(MR_FILE_HANDLE fh, void *data, uint32 len);

/**
 * 偏移文件读写指针
 * 参数:
 * 	fh		文件句柄
 * 	pos   	要偏移得数量
 * 	method	偏移起算的位置
 * 返回:
 * 	MR_SUCCESS		成功
 * 	MR_FAILED		失败
 */
int32 mr_seek(MR_FILE_HANDLE f, int32 pos, int method);

/**
 * 获取文件信息
 * 参数:
 * 	filename		文件名称
 * 返回:
 * 	MR_IS_FILE		是文件
 * 	MR_IS_DIR		是目录
 * 	MR_IS_INVALID	无效
 */
int32 mr_info(const char *filename);

/**
 * 删除文件
 * 参数:
 * 	filename	要删除的文件的文件名
 * 返回:
 * 	MR_SUCCESS		成功
 * 	MR_FAILED		失败
 */
int32 mr_remove(const char *filename);

/**
 * 重命名文件
 * 参数:
 * 	oldname		原文件名
 * 	newname		新文件名
 * 返回:
 * 	MR_SUCCESS		成功
 * 	MR_FAILED		失败
 */
int32 mr_rename(const char *oldname, const char *newname);

/**
 * 创建目录
 * 参数:
 * 	name	目录名
 * 返回:
 * 	MR_SUCCESS		成功
 * 	MR_FAILED		失败
 */
int32 mr_mkDir(const char *name);

/**
 * 删除目录
 * 参数:
 * 	name	要删除的目录名
 * 返回:
 * 	MR_SUCCESS		成功
 * 	MR_FAILED		是吧
 */
int32 mr_rmDir(const char *name);

/**
 * 初始化一个文件目录的搜索，并返回第一搜索。
 * 参数:
 * 	name	要搜索的目录名
 * 	buffer	保存第一个搜索结果的buf
 * 	len		buf的大小
 * 返回:
 * 	成功:第一个搜索结果的句柄
 * 	MR_FAILED	失败
 */
MR_FILE_HANDLE mr_findStart(const char *name, char *buffer, uint32 len);

/**
 * 搜索目录的下一个结果
 * 参数:
 * 	search_handle		目录的句柄
 * 	buffer				存放搜索结果的buf
 * 	len					buf的大小
 * 返回:
 * 	MR_SUCCESS		成功
 * 	MR_FAILED		失败
 */
int32 mr_findGetNext(MR_FILE_HANDLE search_handle, char *buffer, uint32 len);

/**
 * 停止目录搜索
 * 参数:
 * 	search_handle		搜索句柄
 * 返回:
 * 	MR_SUCCESS		成功
 * 	MR_FAILED		失败
 */
int32 mr_findStop(MR_SEARCH_HANDLE search_handle);

/**
 * 返回的是最后一次操作文件失败的错误信息。
 * 只是是个空实现
 * 参数:无
 * 返回:
 * 	MR_SUCCESS
 * 	MR_FAILED
 */
int32 mr_ferrno(void);

/**
 * 得到指定文件得大小
 * 参数:
 * 	filename		所指定得文件名
 * 返回:
 * 	>=0			文件大小
 * 	MR_FAILED	失败
 */
int32 mr_getLen(const char *filename);

//读取mrp文件
void *_mr_readFile(const char *filename, int *filelen, int lookfor);

#endif