#ifndef __MRP_MR_DRAW_H__
#define __MRP_MR_DRAW_H__

#include "mrp/vm.h"

typedef struct mr_screenRect_st
{
	uint16 x;
	uint16 y;
	uint16 w;
	uint16 h;
} mr_screenRect_t;

typedef struct mr_colour_st
{
	uint8 r;
	uint8 g;
	uint8 b;
} mr_colour_t;

void swi_mr_drawRect(vm_info_t *vm);
void swi_mr_drawText(vm_info_t *vm);
void swi_mr_drawTextEx(vm_info_t *vm);
void swi_mr_refreshBuffer(vm_info_t *vm);
void swi_mr_drawPoint(vm_info_t *vm);
void swi_mr_getCharBitmap(vm_info_t *vm);
void swi_mr_platDrawChar(vm_info_t *vm);

#endif