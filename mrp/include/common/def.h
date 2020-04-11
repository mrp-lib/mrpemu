#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

/** 最大路径长度 */
#define MAX_PATH_LEN 256

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ABS(VAL) (((VAL) > 0) ? (VAL) : (-(VAL)))

//工具相关
#define ntohl(i) (i)
#define htonl(i) (((uint32)i >> 24) | (((uint32)i & 0xff0000) >> 8) | (((uint32)i & 0xff00) << 8) | ((uint32)i << 24))
#define ntohs(i) (i)

#endif