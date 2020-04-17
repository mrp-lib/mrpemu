#ifndef __MR_MD5_H__
#define __MR_MD5_H__

#include "common/type.h"

typedef unsigned char md5_byte_t; /* 8-bit byte */
typedef unsigned int md5_word_t;  /* 32-bit word */

typedef struct md5_state_s
{
	md5_word_t count[2]; /* message length in bits, lsw first */
	md5_word_t abcd[4];	 /* digest buffer */
	md5_byte_t buf[64];	 /* accumulate block */
} md5_state_t;

typedef void (*T_mr_md5_init)(md5_state_t *pms);
typedef void (*T_mr_md5_append)(md5_state_t *pms, const md5_byte_t *data, int nbytes);
typedef void (*T_mr_md5_finish)(md5_state_t *pms, md5_byte_t digest[16]);

void mr_md5_init(md5_state_t *pms);
void mr_md5_append(md5_state_t *pms, const md5_byte_t *data, int nbytes);
void mr_md5_finish(md5_state_t *pms, md5_byte_t digest[16]);

uint32 mr_updcrc(uint8 *s, uint32 n);

#endif