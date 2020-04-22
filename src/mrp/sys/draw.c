#include "mrp.h"

char *mr_getCharBitmap(uint16 ch, uint16 fontSize, int *width, int *height)
{
	xl_font_sky16_charWidthHeight(ch, width, height);
	return xl_font_sky16_getChar(ch);
}

void mr_platDrawChar(vm_info_t *vm, uint16 ch, int32 x, int32 y, int32 color)
{
	xl_font_sky16_drawChar(vm, ch, x, y, color);
}