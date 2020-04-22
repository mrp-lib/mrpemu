#ifndef __MRP_FRONT_H__
#define __MRP_FRONT_H__

#include "common/type.h"
#include "mrp/vm.h"

int xl_font_sky16_init();
int xl_font_sky16_close();

void dpoint(vm_info_t *vm, int x, int y, int color);

int xl_font_sky16_getfontpix(char *buf, int n);
char *xl_font_sky16_getChar(uint16 id);
char *xl_font_sky16_drawChar(vm_info_t *vm, uint16 id, int x, int y, uint16 color);
int xl_font_sky16_textWidthHeightLines(uint8 *pcText, uint16 showWidth, int32 *width, int32 *height, int32 *lines);
void xl_font_sky16_charWidthHeight(uint16 id, int32 *width, int32 *height);

#endif