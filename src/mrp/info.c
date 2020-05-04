#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common/tools.h"
#include "mrp.h"

//mrp头信息
typedef struct
{
	char magic[4];		//这个没什么啦
	uint32 data_offset; //数据部分位置
	uint32 mrp_size;	//总字节数
	uint32 list_offset; //文件列表开始位置
	char name[12];		//内部名
	char showName[24];	//显示名
	char imei[16];		//串号
	uint32 version;		//版本号
	uint32 appid;		//appid
	char _1[12];		//未知
	char author[40];	//作者
	char desc[64];		//介绍
	uint32 ver;			//版本号
	uint32 id;			//appid

	/*
	这些就不管了。
	// 192-196		版本ID十六进制值
	// 196-200		appid十六进制值
	200-240		未知
	*/
} mrp_head_info;

mrp_reader_t *mrp_open(const char *filename)
{
	logi("mrp_open(filename=%s)", filename);
	//打开文件
	FILE *fd = fopen(filename, "rb");
	if (fd == null)
	{
		loge("mrp_open failed when open file");
		return null;
	}
	//读取头
	mrp_head_info head = {0};
	if (sizeof(mrp_head_info) != fread(&head, 1, sizeof(mrp_head_info), fd))
	{
		loge("mrp_open failed when read head");
		fclose(fd);
		return null;
	}
	//创建读取器
	mrp_reader_t *reader = (mrp_reader_t *)malloc(sizeof(mrp_reader_t));
	if (reader == null)
	{

		loge("mrp_open failed when alloc reader");
		fclose(fd);
		return null;
	}

	//初始化mrp信息
	strncpy(reader->info.name, head.name, sizeof(head.name) - 1);
	strncpy(reader->info.showName, head.showName, sizeof(head.showName) - 1);
	strncpy(reader->info.author, head.author, sizeof(head.author) - 1);
	strncpy(reader->info.desc, head.desc, sizeof(head.desc) - 1);
	reader->info.version = head.version;
	reader->info.appid = head.appid;
	reader->info.ver = ntohl(head.ver);
	reader->info.id = ntohl(head.id);
	//其他值
	reader->data_offset = head.data_offset;
	reader->list_offset = head.list_offset;
	//文件
	reader->fd = fd;
	//返回去
	return reader;
}

void mrp_close(mrp_reader_t *reader)
{
	if (reader != null)
	{
		logi("mrp_close(reader=%p)", reader);
		if (reader->fd)
			fclose(reader->fd);
		free(reader);
	}
}

void mrp_load_files(mrp_reader_t *reader)
{
	//避免重复加载
	if (reader->file_len > 0)
		return;
	logi("mrp_load_files(reader=%p)", reader);

	//逐个加载文件
	for (uint32 i = 0, offset = reader->list_offset; i < MRP_MAX_INNEER_FILE; i++)
	{
		uint32 nameLen, fileOffset, fileSize;
		char nameBuffer[MRP_MAX_INNEER_NAME_LEN] = {0};

		//读取文件名长度
		fseek(reader->fd, offset, SEEK_SET);
		fread(&nameLen, 1, 4, reader->fd);
		offset += 4;
		//读取文件名
		fseek(reader->fd, offset, SEEK_SET);
		fread(nameBuffer, 1, min(nameLen, MRP_MAX_INNEER_NAME_LEN) - 1, reader->fd); //这里读取长度-1是为了保留一个\0
		offset += nameLen;
		//读取偏移
		fseek(reader->fd, offset, SEEK_SET);
		fread(&fileOffset, 1, 4, reader->fd);
		offset += 4;
		//读取文件大小
		fseek(reader->fd, offset, SEEK_SET);
		fread(&fileSize, 1, 4, reader->fd);
		offset += 8; //最后还有4个空字节，所以+8

		//读取gzip识别字段
		uint16 gzipCode = 0; //是不是[0]=1f,[2]=8b
		fseek(reader->fd, fileOffset, SEEK_SET);
		fread(&gzipCode, 1, 2, reader->fd);

		//保存
		strcpy(reader->files[i].name, nameBuffer);
		reader->files[i].offset = fileOffset;
		reader->files[i].size = fileSize;
		reader->files[i].isgzip = (gzipCode == 0x8b1f);
		reader->file_len++; //自增

		//检测是否读取完（也就是读取到数据区域了,注意的是数据区域需要+8）
		if (offset >= reader->data_offset + 8)
			break;
	}
}

mrp_file_info *mrp_file(mrp_reader_t *reader, char *filename)
{
	logi("mrp_file(reader=%p, filename=%s)", reader, filename);
	mrp_load_files(reader); //不管怎样先加载一次吧

	for (uint32 i = 0; i < reader->file_len; i++)
	{
		if (strcmp(filename, reader->files[i].name) == 0)
		{
			return &reader->files[i];
		}
	}
	return null;
}

int32 mrp_read(mrp_reader_t *reader, char *filename, uint8 *buffer)
{
	logi("mrp_read(reader=%p, filename=%s, buffer=%p)", reader, filename, buffer);
	mrp_file_info *info = mrp_file(reader, filename); //取得文件
	if (info == null)
		return -1;
	//读取
	fseek(reader->fd, info->offset, SEEK_SET);
	size_t readed = fread(buffer, 1, info->size, reader->fd);
	if (info->size != readed)
		return -1;
	//返回
	return 0;
}
