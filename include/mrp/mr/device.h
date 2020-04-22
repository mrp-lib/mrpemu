#ifndef __MRP_MR_DEVICE_H__
#define __MRP_MR_DEVICE_H__

#include "common/type.h"
#include "mrp/vm.h"

typedef struct
{
	/** 屏幕宽 */
	uint32 width;
	/** 屏幕高 */
	uint32 height;
	/** 屏幕象素深度，单位bit */
	uint32 bit;
} mr_screeninfo_t;

void swi_mr_getScreenInfo(vm_info_t *vm);

#endif