#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "mrp.h"

bool bitmap_has_bit(uint8 *bitmap, uint32 n)
{
	bitmap += n / 8;
	return (0x80 >> (n % 8)) & *bitmap;
}

int32 font_init(vm_info_t *vm)
{
	if (vm->mem->font.buf != null)
		return MR_SUCCESS;

	//字体名称
	char fontfile[MAX_FILE_PATH_LEN] = {0};
	sprintf(fontfile, "%s%ssystem/gb16.uc2", mrst.sysinfo.sdcard_dir, mrst.sysinfo.dsm_dir);

	//文件长度
	struct stat s;
	int ret = stat(fontfile, &s);
	if (ret != 0)
		return MR_FAILED;

	//打开文件
	FILE *fd = fopen(fontfile, "rb");
	if (!fd)
		return MR_FAILED;

	//读取字体内容
	uint8 *buf = (uint8 *)malloc(s.st_size);
	if (s.st_size != fread(buf, 1, s.st_size, fd))
	{
		fclose(fd);
		return MR_FAILED;
	}

	vm->mem->font.buf = buf;
	vm->mem->font.size = 16;
	vm->mem->font.len = s.st_size;
	vm->mem->font.chlen = 32; //(size * size) / 8;

	return MR_SUCCESS;
}

void font_free(vm_info_t *vm)
{
	if (vm->mem->font.buf == null)
		return;
	free(vm->mem->font.buf);
}

uint8 *font_get_bitmap(vm_info_t *vm, uint16 ch)
{
	//取得点阵
	return memcpy(vm->mem->font.bitmapBuf, vm->mem->font.buf + vm->mem->font.chlen * ch, vm->mem->font.chlen);
}

int32 font_ch_width(vm_info_t *vm, uint16 ch)
{
	//普通ASCII字符
	if (ch < 0x80)
		return vm->mem->font.size >> 1; //宽度=字体大小/2

	//其他字符
	return vm->mem->font.size;
}

int32 font_text_width(vm_info_t *vm, uint16 *str)
{
	//逐个取长度并相加
	uint32 len = 0;
	while (*str)
		len += font_ch_width(vm, *(str++));
	return len;
}

int32 font_ch_draw(vm_info_t *vm, int32 x, int32 y, uint16 ch, uint16 color)
{
	uint8 *bmp = font_get_bitmap(vm, ch);
	uint32 fw = font_ch_width(vm, ch);

	for (uint32 dy = 0; dy < vm->mem->font.size; dy++)
	{
		for (uint32 dx = 0; dx < fw; dx++)
		{
			if (!bitmap_has_bit(bmp, dy * vm->mem->font.size + dx))
				continue;
			*(vm->mem->video + (dy + y) * mrst.sysinfo.screen_width + (dx + x)) = color;
		}
	}
}
