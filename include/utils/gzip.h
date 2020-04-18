#ifndef __UTILS_GZIP_H__
#define __UTILS_GZIP_H__

#include "common/type.h"

//缓冲区的大小，1M应该够用了吧
#define GZIP_BUFFER_SIZE (1024 * 1024)

/*
数据的gzip解压
参数：
	src		原始数据
	srcLen	原始数据长度
	dst		目标数据指针，如果是空的话，则使用缓冲区，当使用缓冲区时，调用这个函数都会覆盖原有数据，因此，请做好备份
	dstLen	目标数据长度指针
返回：
	!null	目标数据，如果使用缓冲区，则返回缓冲区的指针，否则返回dst参数指针
	null	失败
*/
uint8 *ungzip(uint8 *src, uint32 srcLen, uint8 *dst, uint32 *dstLen);

#endif