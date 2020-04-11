#ifndef __COMMON_LOG_H__
#define __COMMON_LOG_H__

#include <stdio.h>

/** 换行打印 */
#define println(fmt, ...)       \
	printf(fmt, ##__VA_ARGS__); \
	printf("\n")

/** debug日志 */
#define logd(fmt, ...)  \
	printf("[DEBUG] "); \
	println(fmt, ##__VA_ARGS__)

/** 错误日志 */
#define loge(fmt, ...)  \
	printf("[ERROR] "); \
	println(fmt, ##__VA_ARGS__)

/** 警告日志 */
#define logw(fmt, ...)  \
	printf("[WARN]  "); \
	println(fmt, ##__VA_ARGS__)

/** 普通日志 */
#define logi(fmt, ...)  \
	printf("[LOG]   "); \
	println(fmt, ##__VA_ARGS__)

#endif