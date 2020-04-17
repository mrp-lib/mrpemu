#ifndef __MR_CPORT_H__
#define __MR_CPORT_H__

#include "common/type.h"

typedef void *(*T_memcpy)(void *s1, const void *s2, int n);
typedef void *(*T_memmove)(void *s1, const void *s2, int n);
typedef char *(*T_strcpy)(char *s1, const char *s2);
typedef char *(*T_strncpy)(char *s1, const char *s2, int n);
typedef char *(*T_strcat)(char *s1, const char *s2);
typedef char *(*T_strncat)(char *s1, const char *s2, int n);
typedef int (*T_memcmp)(const void *s1, const void *s2, int n);
typedef int (*T_strcmp)(const char *s1, const char *s2);
typedef int (*T_strncmp)(const char *s1, const char *s2, int n);
typedef int (*T_strcoll)(const char *s1, const char *s2);
typedef void *(*T_memchr)(const void *s, int c, int n);
typedef void *(*T_memset)(void *s, int c, int n);
typedef int (*T_strlen)(const char *s);
typedef char *(*T_strstr)(const char *s1, const char *s2);
typedef int (*T_sprintf)(char *s, const char *format, ...);
typedef int (*T_atoi)(const char *nptr);
typedef unsigned long int (*T_strtoul)(const char *nptr, char **endptr, int base);
typedef int (*T_rand)(void);
typedef void (*T_mr_printf)(const char *format, ...);

typedef int (*T_mr_wstrlen)(char *txt);

void *mr_memcpy(void *dst, const void *src, uint32 n);
void *mr_memmove(void *dst, const void *src, uint32 n);
void *mr_memset(void *dst, int ch, uint32 n);
int mr_memcmp(const void *lhs, const void *rhs, uint32 n);
void *mr_memchr(const void *s, int ch, uint32 n);
void *mr_strcpy(char *dst, const char *src);
char *mr_strncpy(char *dst, const char *src, uint32 n);
char *mr_strcat(char *dst, const char *src);
char *mr_strncat(char *dst, const char *src, uint32 n);
int mr_strcmp(const char *lhs, const char *rhs);
int mr_strncmp(const char *lhs, const char *rhs, uint32 n);
int mr_strcoll(const char *lhs, const char *rhs);
int mr_strlen(const char *s);
char *mr_strstr(const char *haystack, const char *needle);
int mr_atoi(const char *s);
unsigned long mr_strtoul(const char *s, char **end_ptr, int base);
int mr_rand(void);

int mr_sprintf(char *buf, const char *fmt, ...);
void mr_printf(const char *format, ...);

int mr_wstrlen(char *str);

#endif