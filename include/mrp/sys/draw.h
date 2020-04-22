#ifndef __MRP_SYS_DRAW_H__
#define __MRP_SYS_DRAW_H__

char *mr_getCharBitmap(uint16 ch, uint16 fontSize, int *width, int *height);

void mr_platDrawChar(vm_info_t *vm, uint16 ch, int32 x, int32 y, int32 color);

#endif