#include <stdlib.h>
#include <string.h>
#include "mrp.h"
#include "common/tools.h"

#define scr_point(x, y) (vm->mem->video + (y)*mrst.sysinfo.screen_width + (x))							 //显存某个位置的像素点，使用rgb565，所以每个像素点16位
#define mkrgb(r, g, b) (uint16)(((uint32)(r >> 3) << 11) + ((uint32)(g >> 2) << 5) + ((uint32)(b >> 3))) //从给定的rgb构建rgb565颜色

//懒得写，简化一下吧
#define sw mrst.sysinfo.screen_width
#define sh mrst.sysinfo.screen_height

void swi_mr_drawRect(vm_info_t *vm)
{
	//原型： (int16 x, int16 y, int16 w, int16 h, uint8 r, uint8 g, uint8 b) -> void
	int32 x = vmreg(0);
	int32 y = vmreg(1);
	int32 w = vmreg(2);
	int32 h = vmreg(3);
	uint8 r = vmstack(0);
	uint8 g = vmstack(1);
	uint8 b = vmstack(2);
	logsysc("mr_drawRect(x=%d, y=%d, w=%d, h=%d, r=%d, g=%d, b=%d)", x, y, w, h, r, g, b);

	//处理一下位置
	if (w == 0 || h == 0)
		return;
	if (w < 0)
		x -= w = abs(w);
	if (h < 0)
		y -= h = abs(h);

	//处理一下绘制范围
	int32 minx = max(x, 0);
	int32 miny = max(y, 0);
	int32 maxx = min(sw, x + w);
	int32 maxy = min(sh, y + h);

	uint16 color = mkrgb(r, g, b);

	//先绘制第一行
	for (uint32 dx = minx; dx < maxx; dx++)
	{
		*scr_point(dx, miny) = color;
	}

	//其他行的跟着拷贝就行了
	uint16 *line1 = scr_point(minx, miny);
	for (uint32 dy = miny + 1; dy < maxy; dy++)
	{
		memcpy(scr_point(minx, dy), line1, (maxx - minx) << 1);
	}
}

void swi_mr_platDrawChar(vm_info_t *vm)
{
	//原型： (uint16 ch, int32 x, int32 y, int32 color) -> void
	uint16 ch = vmreg(0);
	uint16 x = vmreg(1);
	uint16 y = vmreg(2);
	uint16 color = vmreg(3);
	logsysc("mr_platDrawChar(ch=%d, x=%d, y=%d, color=%d)", ch, x, y, color);

	font_ch_draw(vm, x, y, ch, color);
}

void swi_mr_drawText(vm_info_t *vm)
{
	//原型： (char* pcText, int16 x, int16 y, uint8 r, uint8 g, uint8 b, int is_unicode, uint16 font) -> int32
	char *pcText = vmpt_real(char, vmreg(0));
	int32 x = vmreg(1);
	int32 y = vmreg(2);
	uint8 r = vmreg(3);
	uint8 g = vmstack(0);
	uint8 b = vmstack(1);
	uint32 is_unicode = vmstack(2);
	uint16 font = vmstack(3);
	logsysc("mr_drawText(pText=%s, x=%d, y=%d, r=%d, g=%d, b=%d, is_unicode=%d, font=%d)", pcText, x, y, r, g, b, is_unicode, font);

	int32 len;
	uint16 color = mkrgb(r, g, b);

	if (!is_unicode)
		pcText = (char *)mr_c2u(pcText, NULL, &len);

	font_init(vm);

	int32 sx = x;
	uint8 *p = (uint8 *)pcText;
	while (*p || *(p + 1))
	{
		uint16 ch;
		ch = (uint16)((*p << 8) + *(p + 1));
		font_ch_draw(vm, sx, y, ch, color);
		sx += font_ch_width(vm, ch);
		p += 2;

		if (sx > sw)
			break;
	}

	if (!is_unicode)
		free(pcText);

	mr_ret(MR_SUCCESS);
}

void swi_mr_drawTextEx(vm_info_t *vm)
{
	//原型： (char* pcText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst, int flag, uint16 font) -> int32
	char *text = vmpt_real(char, vmreg(0));
	int32 x = vmreg(1);
	int32 y = vmreg(2);
	int32 rx = vmreg(3);
	int32 ry = vmstack(0);
	int32 rw = vmstack(1);
	int32 rh = vmstack(2);

	logsysc("mr_drawTextEx(text=%s, x=%d, y=%d, rect={%d,%d,%d,%d})", text, x, y, rx, ry, rw, rh);

	mr_ret(MR_SUCCESS);
}

void swi_mr_refreshBuffer(vm_info_t *vm)
{
	//原型： (uint16 *bmp, int16 x, int16 y, uint16 w, uint16 h) -> void
	vmpt _mbp = vmreg(0);
	uint16 *bmp = vmpt_real(uint16, _mbp);
	int32 x = vmreg(1);
	int32 y = vmreg(2);
	int32 w = vmreg(3);
	int32 h = vmstack(0);
	logsysc("mr_drawBitmap(bmp=0x%08x, x=%d, y=%d, w=%d, h=%d)", _mbp, x, y, w, h);

	//调用函数进行刷新
	if (vm->onRefresh == null)
	{
		logw("no screen refresh callback provided, ignore!");
		return;
	}

	vm->onRefresh(bmp, x, y, w, h);
}

void swi_mr_drawPoint(vm_info_t *vm)
{
	//原型： (int16 x, int16 y, uint16 nativecolor) -> void
	uint32 x = vmreg(0);
	uint32 y = vmreg(1);
	uint16 color = vmreg(2);
	// logsysc("mr_drawPoint(x=%d, y=%d, color=0x%04x)", x, y, color);

	if (x >= 0 && y >= 0 && x < sw && y < sh)
		*scr_point(x, y) = color;
}

void swi_mr_getCharBitmap(vm_info_t *vm)
{
	//原型： (uint16 ch, uint16 fontSize, int *width, int *height) -> const char*
	uint16 ch = vmreg(0);
	uint16 fontSize = vmreg(1);
	vmpt _width = vmreg(2);
	vmpt _height = vmreg(3);
	logsysc("mr_getCharBitmap(ch=%d, fontSize=%d, width=0x%08x, height=0x%08x)", ch, fontSize, _width, _height);

	font_init(vm);

	*vmpt_real(int32, _width) = font_ch_width(vm, ch);
	*vmpt_real(int32, _height) = vm->mem->font.size;

	uint8 *addr = font_get_bitmap(vm, ch);
	mr_ret(vm_mem_offset(addr));
}