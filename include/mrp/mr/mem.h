#ifndef __MRP_MR_MEM_H__
#define __MRP_MR_MEM_H__

#include "mrp/common/helper.h"

void swi_mr_malloc(vm_info_t *vm);
void swi_mr_free(vm_info_t *vm);
void swi_mr_realloc(vm_info_t *vm);

#endif