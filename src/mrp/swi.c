#include "arm.h"
#include "mrp.h"

//中断处理程序
void handle_interapt(vm_info_t *vm, uint32 nu)
{
	//TODO 暂时这样吧，先用着，等基本完善了再处理成字典。
	//处理中断号
	switch (nu)
	{
	case SWI_MR_MALLOC:
		return swi_mr_malloc(vm);
	case SWI_MR_FREE:
		return swi_mr_free(vm);
	case SWI_MR_REALLOC:
		return swi_mr_realloc(vm);

	case SWI_MR_MEMCPY:
		return swi_mr_memcpy(vm);
	case SWI_MR_MEMMOVE:
		return swi_mr_memmove(vm);
	case SWI_MR_STRCPY:
		return swi_mr_strcpy(vm);
	case SWI_MR_STRNCPY:
		return swi_mr_strncpy(vm);
	case SWI_MR_STRCAT:
		return swi_mr_strcat(vm);
	case SWI_MR_STRNCAT:
		return swi_mr_strncat(vm);
	case SWI_MR_MEMCMP:
		return swi_mr_memcmp(vm);
	case SWI_MR_STRCMP:
		return swi_mr_strcmp(vm);
	case SWI_MR_STRNCMP:
		return swi_mr_strncmp(vm);
	case SWI_MR_STRCOLL:
		return swi_mr_strcoll(vm);
	case SWI_MR_MEMCHR:
		return swi_mr_memchr(vm);
	case SWI_MR_MEMSET:
		return swi_mr_memset(vm);
	case SWI_MR_STRLEN:
		return swi_mr_strlen(vm);
	case SWI_MR_STRSTR:
		return swi_mr_strstr(vm);
	case SWI_MR_SPRINTF:
		return swi_mr_sprintf(vm);
	case SWI_MR_ATOI:
		return swi_mr_atoi(vm);
	case SWI_MR_STRTOUL:
		return swi_mr_strtoul(vm);
	case SWI_MR_RAND:
		return swi_mr_rand(vm);

	case SWI_MR_FERRNO:
		return swi_mr_ferrno(vm);
	case SWI_MR_OPEN:
		return swi_mr_open(vm);
	case SWI_MR_CLOSE:
		return swi_mr_close(vm);
	case SWI_MR_INFO:
		return swi_mr_info(vm);
	case SWI_MR_WRITE:
		return swi_mr_write(vm);
	case SWI_MR_READ:
		return swi_mr_read(vm);
	case SWI_MR_SEEK:
		return swi_mr_seek(vm);
	case SWI_MR_GETLEN:
		return swi_mr_getLen(vm);
	case SWI_MR_REMOVE:
		return swi_mr_remove(vm);
	case SWI_MR_RENAME:
		return swi_mr_rename(vm);
	case SWI_MR_MKDIR:
		return swi_mr_mkDir(vm);
	case SWI_MR_RMDIR:
		return swi_mr_rmDir(vm);
	case SWI_MR_FINDSTART:
		return swi_mr_findStart(vm);
	case SWI_MR_FINDGETNEXT:
		return swi_mr_findGetNext(vm);
	case SWI_MR_FINDSTOP:
		return swi_mr_findStop(vm);
	case SWI_MR_READFILE:
		return swi_mr_readFile(vm);

	case SWI_MR_PRINTF:
		return swi_mr_printf(vm);
	case SWI_MR_WSTRLEN:
		return swi_mr_wstrlen(vm);

	case SWI_MR_DRAWPOINT:
		return swi_mr_drawPoint(vm);
	case SWI_MR_REFRESHBUFFER:
		return swi_mr_refreshBuffer(vm);
	case SWI_MR_DRAWRECT:
		return swi_mr_drawRect(vm);
	case SWI_MR_DRAWTEXT:
		return swi_mr_drawText(vm);
	case SWI_MR_DRAWTEXTEX:
		return swi_mr_drawTextEx(vm);
	case SWI_MR_GETCHARBITMAP:
		return swi_mr_getCharBitmap(vm);
	case SWI_MR_PLATDRAWCHAR:
		return swi_mr_platDrawChar(vm);
	case SWI_MR_DRAWBITMAP:
		return swi_mr_drawBitmap(vm);

	case SWI_MR_GETSCREENINFO:
		return swi_mr_getScreenInfo(vm);

	case SWI_MR_C_FUNCTION_NEW:
		return swi_mr_c_function_new(vm);

	case SWI_MR_DIV:
		return swi_mr_div(vm);
	case SWI_MR_MOD:
		return swi_mr_mod(vm);

	//平台的一些调用
	case SWI_MR_TESTCOM:
		return swi_mr_testcom(vm);
	default:
		println("产生中断:0x%04x(%d)", nu, nu);
	}
}

int32 on_sorftware_interrupt(cpu_state_t *st, uint32 nu)
{
	//反向得到虚拟机指针
	vm_mem_map_t *memmap = (vm_mem_map_t *)(st->mem->buffer + VM_MEM_OFFSET); //取得内存映射
	vm_info_t *vm = (vm_info_t *)memmap->vmaddr;							  //从内存映射中取得虚拟机地址
	//处理中断
	handle_interapt(vm, nu);
	return 0;
}