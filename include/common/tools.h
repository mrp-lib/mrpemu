#ifndef __COMMON_TOOLS_H__
#define __COMMON_TOOLS_H__

#define max(a, b) ((a) > (b) ? (a) : (b)) //计算最大值
#define min(a, b) ((a) > (b) ? (b) : (a)) //计算最小值

#define ntohl(num) ((num << 24) | ((num & 0x0000FF00) << 8) | ((num & 0x00FF0000) >> 8) | (num >> 24)) //大小端转换

#endif