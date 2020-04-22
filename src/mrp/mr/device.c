#include "mrp.h"

void swi_mr_getScreenInfo(vm_info_t *vm)
{
	//原型： (mr_screeninfo *s) -> int32
	vmpt _s = vmreg(0);
	mr_screeninfo_t *s = vmpt_real(mr_screeninfo_t, _s);
	logsysc("mr_getScreenInfo(s=0x%08x)", _s);

	s->width = mrst.sysinfo.screen_width;
	s->height = mrst.sysinfo.screen_height;
	s->bit = mrst.sysinfo.screen_bits;

	mr_ret(MR_SUCCESS);
}