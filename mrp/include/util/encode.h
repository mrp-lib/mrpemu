#ifndef __UTIL_ENCODE_H__
#define __UTIL_ENCODE_H__

#include "common/type.h"

typedef struct t_ucs2gb_st
{
	unsigned short ucs;
	unsigned short gb;
} t_ucs2gb;

extern unsigned char gb2uTable[];
extern t_ucs2gb tab_symucs2gb[];
extern unsigned short tab_ucs2gb[];

char *UCS2ByteRev(char *str);
int UCS2_strlen(const char *txt);
const unsigned char *GBCodeToUnicode(unsigned char *gbCode);

int GBToUCS2(unsigned char *gbCode, unsigned char *unicode);
int UCS2ToGBString(const uint16 *uniStr, uint8 *gbBuf, int gbMaxLen);
int UTF8ToUCS2String(const uint8 *utf8Str, uint16 *ucs2Buf, int ucs2MaxLen);
int UTF8ToGBString(const uint8 *utf8Str, uint8 *gbBuf, int gbMaxLen);
int GBToUTF8String(const uint8 *gbStr, uint8 *utf8Buf, int utf8MaxLen);
int UCS2ToUTF8(const uint8 *unicode, uint8 *utf8, int size);

#endif