#ifndef __MR_GB2312_H__
#define __MR_GB2312_H__

typedef unsigned short unicode_char;

typedef uint16 *(*T_mr_c2u)(char *cp, int32 *err, int32 *size);

uint16 *mr_c2u(char *cp, int *err, int *size);

#endif
