#include <stdlib.h>

#include "zlib/zlib.h"
#include "utils/gzip.h"

//TODO 需要考虑多线程的问题。
// uint8 *zbuffer;
static uint8 zbuffer[GZIP_BUFFER_SIZE];

uint8 *ungzip(uint8 *src, uint32 srcLen, uint8 *dst, uint32 *dstLen)
{
	z_stream strm;

	if (dst == null)
	{
		strm.avail_out = GZIP_BUFFER_SIZE;
		strm.next_out = (Bytef *)zbuffer;
	}
	else
	{
		strm.avail_out = *dstLen;
		strm.next_out = (Bytef *)dst;
	}

	strm.zalloc = null;
	strm.zfree = null;
	strm.opaque = null;
	strm.avail_in = srcLen;
	strm.next_in = (Bytef *)src;

	int err = inflateInit2(&strm, MAX_WBITS + 16);
	if (err != Z_OK)
	{
		inflateEnd(&strm);
		return null;
	}

	err = inflate(&strm, Z_FINISH);
	if (err != Z_STREAM_END)
	{
		inflateEnd(&strm);
		return null;
	}

	*dstLen = strm.total_out;

	return (dst == null) ? zbuffer : dst;
}