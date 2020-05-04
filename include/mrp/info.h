/**
 * 这个文件用于获取mrp文件的信息
 */

#ifndef __MRP_INFO_H__
#define __MRP_INFO_H__

#include "common/type.h"

#define MRP_MAX_INNEER_NAME_LEN 128 //mrp内部文件名称最大长度
#define MRP_MAX_INNEER_FILE 512		//mrp内部文件最大数量

//mrp信息结构
typedef struct
{
	char name[12];	   //内部名
	char showName[24]; //显示名
	uint32 version;	   //版本号
	uint32 appid;	   //appid
	char author[40];   //作者
	char desc[64];	   //介绍
	//这两个和上面的差不多吧，不知道为什么要设置这么两个，待解...
	uint32 ver; //版本号
	uint32 id;	//appid
} mrp_info_t;

//mrp内部文件信息
typedef struct
{
	char name[MRP_MAX_INNEER_NAME_LEN]; //文件名
	uint32 offset;						//文件偏移
	uint32 size;						//文件长度
	bool isgzip;						//是不是gzip编码
} mrp_file_info;

//mrp读取器定义
typedef struct
{
	FILE *fd;								  //文件指针
	mrp_info_t info;						  //mrp信息
	mrp_file_info files[MRP_MAX_INNEER_FILE]; //mrp文件列表
	uint32 data_offset;						  //数据部分位置
	uint32 list_offset;						  //文件列表开始位置
	uint32 file_len;						  //文件列表长度
} mrp_reader_t;

/*
打开一个mrp文件
参数:
	filename		文件名
*/
mrp_reader_t *mrp_open(const char *filename);

/*
关闭mrp
参数：
	reader		读取器
*/
void mrp_close(mrp_reader_t *reader);

/*
加载文件列表，如果已经加载则直接略过
参数：
	reader		读取器
*/
void mrp_load_files(mrp_reader_t *reader);

/*
获取内部mrp文件信息
参数：
	reader		读取器
	filename	文件名称
返回：
	文件信息，如果文件不存在则返回null
*/
mrp_file_info *mrp_file(mrp_reader_t *reader, char *filename);

/*
读取mrp内部文件
参数：
	reader			读取器
	filename		内部文件名
	buffer			存放的位置
返回：
	0		读取成功
	-1		读取失败
*/
int32 mrp_read(mrp_reader_t *reader, char *filename, uint8 *buffer);

#endif