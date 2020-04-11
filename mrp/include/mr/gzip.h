#ifndef __MR_GZIP_H__
#define __MR_GZIP_H__

#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/type.h"

#define memzero(s, n) memset((void *)(s), 0, (n))
#define MRDBGPRINTF mr_printf

#define local static

typedef unsigned char uch;
typedef unsigned short ush;
typedef unsigned long ulg;

/* Compression methods (see algorithm.doc) */
//#define STORED      0
//#define COMPRESSED  1
#define PACKED 2
//#define LZHED       3
/* methods 4 to 7 reserved */
#define DEFLATED 8
//#define MAX_METHODS 9
//extern int method;         /* compression method */

extern uch *mr_gzInBuf;
extern uch *mr_gzOutBuf;
extern unsigned LG_gzinptr;	 /* index of next byte to be processed in inbuf */
extern unsigned LG_gzoutcnt; /* bytes in output buffer */

//#//define isize bytes_in
/* for compatibility with old zip sources (to be cleaned) */

//typedef int file_t;     /* Do not use stdio */
//#define NO_FILE  (-1)   /* in memory compression */

#define PACK_MAGIC "\037\036"		   /* Magic header for packed files */
#define GZIP_MAGIC "\037\213"		   /* Magic header for gzip files, 1F 8B */
#define OLD_GZIP_MAGIC "\037\236"	   /* Magic header for gzip 0.5 = freeze 1.x */
#define LZH_MAGIC "\037\240"		   /* Magic header for SCO LZH Compress files*/
#define PKZIP_MAGIC "\120\113\003\004" /* Magic header for pkzip files */

/* gzip flag byte */
#define ASCII_FLAG 0x01	  /* bit 0 set: file probably ascii text */
#define CONTINUATION 0x02 /* bit 1 set: continuation of multi-part gzip file */
#define EXTRA_FIELD 0x04  /* bit 2 set: extra field present */
#define ORIG_NAME 0x08	  /* bit 3 set: original file name present */
#define COMMENT 0x10	  /* bit 4 set: file comment present */
#define ENCRYPTED 0x20	  /* bit 5 set: file is encrypted */
#define RESERVED 0xC0	  /* bit 6,7:   reserved */

/* internal file attribute */
#define UNKNOWN 0xffff
#define BINARY 0
#define ASCII 1

#define WSIZE 0x200000 /* window size--must be a power of two, and */

#define MIN_MATCH 3
#define MAX_MATCH 258
/* The minimum and maximum match lengths */

#define MIN_LOOKAHEAD (MAX_MATCH + MIN_MATCH + 1)
/* Minimum amount of lookahead, except at the end of the input file.
 * See deflate.c for comments about the MIN_MATCH+1.
 */

#define MAX_DIST (WSIZE - MIN_LOOKAHEAD)

#define get_byte() mr_gzInBuf[LG_gzinptr++]
#define try_byte() mr_gzInBuf[LG_gzinptr++]

/* put_byte is used for the compressed output, put_ubyte for the
 * uncompressed output. However unlzw() uses window for its
 * suffix table instead of its output buffer, so it does not use put_ubyte
 * (to be cleaned up).
 */
#define put_ubyte(c) mr_gzOutBuf[LG_gzoutcnt++] = (uch)(c)

/* Output a 16 bit value, lsb first */

/* Output a 32 bit value to the bit stream, lsb first */

#define seekable() 0	/* force sequential output */
#define translate_eol 0 /* no option -a yet */

#define tolow(c) (isupper(c) ? (c) - 'A' + 'a' : (c)) /* force to lower case */

/* Macros for getting two-byte and four-byte header values */
#define SH(p) ((ush)(uch)((p)[0]) | ((ush)(uch)((p)[1]) << 8))
#define LG(p) ((ulg)(SH(p)) | ((ulg)(SH((p) + 2)) << 16))

int mr_unzip(void);
int mr_get_method(int32 buf_len);

int mr_inflate(void);

#endif