#ifndef __MRP_SYS_FMT_H__
#define __MRP_SYS_FMT_H__

#include "common/type.h"

#define FMT_STACK_SIZE 128

typedef struct
{
	uint32 n;					 //不定参数位置(单位:字节)
	uint32 regs[4];				 //r0~r3寄存器，参数1~4，共16字节
	uint8 stack[FMT_STACK_SIZE]; //参数所在栈的一部分，参数5~x，这里限制了长度，因此参数数量不能太多，如果太多了则有可能产生错误。
} arg_list;

int32 fmt_printf(char *format, uint8 *memBase, arg_list *arg);
int32 fmt_sprintf(char *buffer, char *format, uint8 *memBase, arg_list *arg);
int32 fmt_snprintf(char *buffer, uint32 count, char *format, uint8 *memBase, arg_list *arg);

#endif