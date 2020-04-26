#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "mrp.h"

void swi_mr_memcpy(vm_info_t *vm)
{
	vmpt dst = vm->cpu->registers[0]; //本身就是返回值
	vmpt src = vm->cpu->registers[1];
	uint32 n = vm->cpu->registers[2];
	logsysc("mr_memcpy(dst=0x%08x, src=0x%08x, n=%d)", dst, src, n);

	memcpy(vmpt_real(void, dst), vmpt_real(const void, src), n);
}

void swi_mr_memmove(vm_info_t *vm)
{
	vmpt dst = vm->cpu->registers[0]; //本身就是返回值
	vmpt src = vm->cpu->registers[1];
	uint32 n = vm->cpu->registers[2];
	logsysc("mr_memmove(dst=0x%08x, src=0x%08x, n=%d)", dst, src, n);

	memmove(vmpt_real(void, dst), vmpt_real(const void, src), n);
}

void swi_mr_memset(vm_info_t *vm)
{
	vmpt s = vm->cpu->registers[0]; //本身就是返回值
	uint8 ch = vm->cpu->registers[1];
	uint32 n = vm->cpu->registers[2];
	logsysc("mr_memset(s=0x%08x, ch=%d, n=%d)", s, ch, n);

	memset(vmpt_real(void, s), ch, n);
}

void swi_mr_memcmp(vm_info_t *vm)
{
	vmpt s1 = vm->cpu->registers[0];
	vmpt s2 = vm->cpu->registers[1];
	uint32 n = vm->cpu->registers[2];
	logsysc("mr_memcmp(s1=0x%08x, s2=0x%08x, n=%d)", s1, s2, n);

	mr_ret(memcmp(vmpt_real(const void, s1), vmpt_real(const void, s2), n)); //比较并返回结果
}

void swi_mr_memchr(vm_info_t *vm)
{
	vmpt s = vm->cpu->registers[0]; //本身就是返回值
	uint8 ch = vm->cpu->registers[1];
	uint32 n = vm->cpu->registers[2];
	logsysc("mr_memchr(s=0x%08x, ch=%d, n=%d)", s, ch, n);

	memchr(vmpt_real(const void, s), ch, n);
}

void swi_mr_strcpy(vm_info_t *vm)
{
	vmpt dst = vm->cpu->registers[0]; //本身就是返回值
	vmpt src = vm->cpu->registers[1];
	logsysc("mr_strncpy(dst=%s, src=%s)", vmpt_real(char, dst), vmpt_real(const char, src));

	strcpy(vmpt_real(char, dst), vmpt_real(const char, src));
}

void swi_mr_strncpy(vm_info_t *vm)
{
	vmpt dst = vm->cpu->registers[0]; //本身就是返回值
	vmpt src = vm->cpu->registers[1];
	uint32 n = vm->cpu->registers[2];
	logsysc("mr_strncpy(dst=%s, src=%s, n=%d)", vmpt_real(char, dst), vmpt_real(const char, src), n);

	strncpy(vmpt_real(char, dst), vmpt_real(const char, src), n);
}

void swi_mr_strcat(vm_info_t *vm)
{
	vmpt dst = vm->cpu->registers[0]; //本身就是返回值
	vmpt src = vm->cpu->registers[1];
	logsysc("mr_strcat(dst=%s, src=%s)", vmpt_real(char, dst), vmpt_real(const char, src));

	strcat(vmpt_real(char, dst), vmpt_real(const char, src));
}

void swi_mr_strncat(vm_info_t *vm)
{
	vmpt dst = vm->cpu->registers[0]; //本身就是返回值
	vmpt src = vm->cpu->registers[1];
	uint32 n = vm->cpu->registers[2];
	logsysc("mr_strncat(dst=%s, src=%s, n=%d)", vmpt_real(char, dst), vmpt_real(const char, src), n);

	strncat(vmpt_real(char, dst), vmpt_real(const char, src), n);
}

void swi_mr_strcmp(vm_info_t *vm)
{
	vmpt s1 = vm->cpu->registers[0];
	vmpt s2 = vm->cpu->registers[1];
	logsysc("mr_strcmp(s1=%s, s2=%s)", vmpt_real(const char, s1), vmpt_real(const char, s2));

	mr_ret(strcmp(vmpt_real(const char, s1), vmpt_real(const char, s2)));
}

void swi_mr_strncmp(vm_info_t *vm)
{
	vmpt s1 = vm->cpu->registers[0];
	vmpt s2 = vm->cpu->registers[1];
	uint32 n = vm->cpu->registers[2];
	logsysc("mr_strncmp(s1=%s, s2=%s, n=%d)", vmpt_real(const char, s1), vmpt_real(const char, s2), n);

	mr_ret(strncmp(vmpt_real(const char, s1), vmpt_real(const char, s2), n));
}

void swi_mr_strcoll(vm_info_t *vm)
{
	vmpt s1 = vm->cpu->registers[0];
	vmpt s2 = vm->cpu->registers[1];
	logsysc("mr_strcoll(s1=%s, s2=%s)", vmpt_real(const char, s1), vmpt_real(const char, s2));

	mr_ret(strcoll(vmpt_real(const char, s1), vmpt_real(const char, s2)));
}

void swi_mr_strlen(vm_info_t *vm)
{
	vmpt str = vm->cpu->registers[0];
	logsysc("mr_strlen(str=%s)", vmpt_real(const char, str));

	mr_ret(strlen(vmpt_real(const char, str)));
}

void swi_mr_strstr(vm_info_t *vm)
{
	vmpt haystack = vm->cpu->registers[0];
	vmpt needle = vm->cpu->registers[1];
	logsysc("mr_strstr(haystack=%s, needle=%s)", vmpt_real(const char, haystack), vmpt_real(const char, needle));

	char *ret = strstr(vmpt_real(const char, haystack), vmpt_real(const char, haystack));
	mr_ret(vm_mem_offset(ret));
}

void swi_mr_atoi(vm_info_t *vm)
{
	vmpt s = vm->cpu->registers[0];
	logsysc("mr_atoi(s=%s)", vmpt_real(const char, s));

	mr_ret(atoi(vmpt_real(const char, s)));
}

void swi_mr_strtoul(vm_info_t *vm) {}

void swi_mr_rand(vm_info_t *vm)
{
	logsysc("mr_rand()");
	mr_ret(rand());
}

void swi_mr_sprintf(vm_info_t *vm)
{
	//TODO 待实现
}

void swi_mr_printf(vm_info_t *vm)
{
	printf("this is printf\n");
}

void swi_mr_wstrlen(vm_info_t *vm)
{
	vmpt s = vm->cpu->registers[0];
	char *str = vmpt_real(char, s);
	logsysc("mr_wstrlen(str=%s)", str);

	int lenth = 0;
	unsigned char *ss = (unsigned char *)str;

	while (((*ss << 8) + *(ss + 1)) != 0)
	{
		lenth += 2;
		ss += 2;
	}

	mr_ret(lenth);
}