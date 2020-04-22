#ifndef __MRP_MR_DRAW_H__
#define __MRP_MR_DRAW_H__

#include "mrp/vm.h"

void swi_mr_drawRect(vm_info_t *vm);
void swi_mr_drawText(vm_info_t *vm);
void swi_mr_drawBitmap(vm_info_t *vm);

#endif