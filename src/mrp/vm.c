#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arm.h"
#include "mrp.h"
#include "helper/swi.h"

static int16 c_func_tab[] = {
	SWI_MR_MALLOC,
	SWI_MR_FREE,
	SWI_MR_REALLOC,

	SWI_MR_MEMCPY,
	SWI_MR_MEMMOVE,
	SWI_MR_STRCPY,
	SWI_MR_STRNCPY,
	SWI_MR_STRCAT,
	SWI_MR_STRNCAT,
	SWI_MR_MEMCMP,
	SWI_MR_STRCMP,
	SWI_MR_STRNCMP,
	SWI_MR_STRCOLL,
	SWI_MR_MEMCHR,
	SWI_MR_MEMSET,
	SWI_MR_STRLEN,
	SWI_MR_STRSTR,
	SWI_MR_SPRINTF,
	SWI_MR_ATOI,
	SWI_MR_STRTOUL,
	SWI_MR_RAND,

	DATAP_RESERVE,
	DATAP_RESERVE,

	DATAP_C_INTERNAL_TABLE,
	DATA_C_PORT_TABLE,
	SWI_MR_C_FUNCTION_NEW,

	SWI_MR_PRINTF,
	SWI_MR_MEM_GET,
	SWI_MR_MEM_FREE,
	SWI_MR_DRAWBITMAPA,
	SWI_MR_GETCHARBITMAP,
	SWI_MR_TIMERSTART,
	SWI_MR_TIMERSTOP,
	SWI_MR_GETTIME,
	SWI_MR_GETDATETIME,
	SWI_MR_GETUSERINFO,
	SWI_MR_SLEEP,

	SWI_MR_PLAT,
	SWI_MR_PLATEX,

	SWI_MR_FERRNO,
	SWI_MR_OPEN,
	SWI_MR_CLOSE,
	SWI_MR_INFO,
	SWI_MR_WRITE,
	SWI_MR_READ,
	SWI_MR_SEEK,
	SWI_MR_GETLEN,
	SWI_MR_REMOVE,
	SWI_MR_RENAME,
	SWI_MR_MKDIR,
	SWI_MR_RMDIR,
	SWI_MR_FINDSTART,
	SWI_MR_FINDGETNEXT,
	SWI_MR_FINDSTOP,

	SWI_MR_EXIT,
	SWI_MR_STARTSHAKE,
	SWI_MR_STOPSHAKE,
	SWI_MR_PLAYSOUND,
	SWI_MR_STOPSOUND,

	SWI_MR_SENDSMS,
	SWI_MR_CALL,
	SWI_MR_GETNETWORKID,
	SWI_MR_CONNECTWAP,

	SWI_MR_MENUCREATE,
	SWI_MR_MENUSETITEM,
	SWI_MR_MENUSHOW,
	DATAP_RESERVE,
	SWI_MR_MENURELEASE,
	SWI_MR_MENUREFRESH,
	SWI_MR_DIALOGCREATE,
	SWI_MR_DIALOGRELEASE,
	SWI_MR_DIALOGREFRESH,
	SWI_MR_TEXTCREATE,
	SWI_MR_TEXTRELEASE,
	SWI_MR_TEXTREFRESH,
	SWI_MR_EDITCREATE,
	SWI_MR_EDITRELEASE,
	SWI_MR_EDITGETTEXT,
	SWI_MR_WINCREATE,
	SWI_MR_WINRELEASE,

	SWI_MR_GETSCREENINFO,
	SWI_MR_INITNETWORK,
	SWI_MR_CLOSENETWORK,
	SWI_MR_GETHOSTBYNAME,
	SWI_MR_SOCKET,
	SWI_MR_CONNECT,
	SWI_MR_CLOSESOCKET,
	SWI_MR_RECV,
	SWI_MR_RECVFROM,
	SWI_MR_SEND,
	SWI_MR_SENDTO,

	DATA_SCREENBUF,
	DATA_SCREENWIDTH,
	DATA_SCREENHEIGHT,
	DATA_SCREENBIT,
	DATA_BITMAP,
	DATA_TILE,
	DATA_MAP,
	DATA_SOUND,
	DATA_SPRITE,

	DATA_PACK_FILENAME,
	DATA_START_FILENAME,
	DATA_OLD_PACK_FILENAME,
	DATA_OLD_START_FILENAME,

	DATA_RAM_FILE,
	DATA_RAM_FILE_LEN,

	DATA_SOUNDON,
	DATA_SHAKEON,

	DATA_LG_MEM_BASE,
	DATA_LG_MEM_LEN,
	DATA_LG_MEM_END,
	DATA_LG_MEM_LEFT,

	DATA_SMS_CFG_BUF,

	SWI_MR_MD5_INIT,
	SWI_MR_MD5_APPEND,
	SWI_MR_MD5_FINISH,

	SWI_MR_LOAD_SMS_CFG,
	SWI_MR_SAVE_SMS_CFG,

	SWI_MR_DISPUPEX,
	SWI_MR_DRAWPOINT,
	SWI_MR_DRAWBITMAP,
	SWI_MR_DRAWBITMAPEX,
	SWI_MR_DRAWRECT,
	SWI_MR_DRAWTEXT,
	SWI_MR_BITMAPCHECK,
	SWI_MR_READFILE,
	SWI_MR_WSTRLEN,
	SWI_MR_REGISTERAPP,
	SWI_MR_DRAWTEXTEX,
	SWI_MR_EFFSETCON,
	SWI_MR_TESTCOM,
	SWI_MR_TESTCOM1,
	SWI_MR_C2U,
	SWI_MR_DIV,
	SWI_MR_MOD,

	DATA_LG_MEM_MIN,
	DATA_LG_MEM_TOP,
	SWI_MR_UPDCRC,
	DATA_START_FILE_PARAMETER,
	DATA_SMS_RETURN_FLAG,
	DATA_SMS_RETURN_VAL,
	SWI_MR_UNZIP,
	DATA_EXIT_CB,
	DATA_EXIT_CB_DATA,
	DATA_ENTRY,
	SWI_MR_PLATDRAWCHAR,
	DATA_LG_MEM_FREE,
	SWI_MR_TRANSBITMAPDRAW,
	SWI_MR_DRAWREGION,
};

vm_info_t *vm_create(uint32 memSize)
{
	logi("vm_create(memSize=%d)", memSize);
	//内存分配
	uint32 cpu_mem_size = sizeof(vm_mem_map_t) + VM_MEM_OFFSET + memSize; //计算CPU内存大小
	memory_t *mem = mem_create(cpu_mem_size);							  //分配内存
	if (mem == null)
		return null;

	//用一个指针指向内存映射的区域
	vm_mem_map_t *memMap = (vm_mem_map_t *)(mem->buffer + VM_MEM_OFFSET);

	//分配CPU
	cpu_state_t *cpu = cpu_create(mem);
	if (cpu == null)
	{
		mem_destory(mem);
		return null;
	}

	//分配虚拟机
	vm_info_t *vm = (vm_info_t *)malloc(sizeof(vm_info_t));
	if (vm == null)
	{
		mem_destory(mem);
		cpu_destory(cpu);
		return null;
	}

	//初始化
	memMap->sys_info.vmaddr = (uint64)vm;								 //保存虚拟机地址
	vm->mem = memMap;													 //保存内存图
	vm->cpu = cpu;														 //保存CPU信息
	vm->heap_start = mem->buffer + VM_MEM_OFFSET + sizeof(vm_mem_map_t); //计算一下堆内存的开始地址
	vm->heap_end = mem->buffer + cpu_mem_size;							 //计算一下堆内存的结束地址

	//内存初始化
	uint32 mem_blks = vm->heap_blks = (vm->heap_end - vm->heap_start) / VM_MEM_BLOCK_SIZE; //内存块总数
	vm->heap_size = mem_blks * VM_MEM_BLOCK_SIZE;										   //可用内存大小
	vm->heap_blk_tb = (uint8 *)malloc(8 * mem_blks);									   //内存块表初始化，内存块数*每个块用8个字节描述（为了方便就这样了）
	memset(vm->heap_blk_tb, 0, 8 * mem_blks);											   //初始化，标记全部为空闲
	vm->heap_free = vm->heap_size;														   //剩余为全部内存

	//返回出去
	return vm;
}

void vm_free(vm_info_t *vm)
{
	if (vm == null)
		return;

	logi("vm_free(vm=%p)", vm);

	//释放CPU
	if (vm->cpu != null)
	{
		//CPU内存释放
		if (vm->cpu->mem != null)
			mem_destory(vm->cpu->mem);
		//CPU释放
		cpu_destory(vm->cpu);
	}

	//释放虚拟机
	free(vm);
}

uint8 *vm_mem_alloc(vm_info_t *vm, int32 len)
{
	logi("vm_mem_alloc(vm=%p, len=%d)", vm, len);

	uint32 need = (len + VM_MEM_BLOCK_SIZE - 1) / VM_MEM_BLOCK_SIZE; //计算需要的连续快
	uint32 found = 0;												 //已找到了的连续快数量
	uint32 start = 0;												 //开始块位置

	//开始寻找
	for (uint32 i = 0; i < vm->heap_blks; i++)
	{
		uint8 *blki = vm->heap_blk_tb + i; //块信息
		//空闲块
		if (*blki == 0)
		{
			if (found == 0)
				start = i;
			found++;
			//看看是否够了
			if (found == need)
			{
				//标记这些块被分配
				uint32 endBlk = start + found;
				for (uint32 j = start; j < endBlk; j++)
					*(vm->heap_blk_tb + j) = VM_MEM_BLK_USD;

				*(vm->heap_blk_tb + start) |= VM_MEM_BLK_BEG; //标记第一块位位先导

				//将偏移返回
				return vm->heap_start + start * VM_MEM_BLOCK_SIZE;
			}
		}
		//非空闲块
		else
			found = 0;
	}

	//找了半天，没有合适的
	return null;
}

bool vm_mem_free(vm_info_t *vm, uint8 *mem)
{
	logi("vm_mem_free(vm=%p, mem=%p)", vm, mem);
	uint32 offset = mem - vm->heap_start;	  //相对于堆内存的偏移
	uint32 blkN = offset / VM_MEM_BLOCK_SIZE; //所在块位置
	//超出了
	if (blkN >= vm->heap_blks)
		return false;

	uint8 *blk = vm->heap_blk_tb + blkN; //内存块

	//是一个空闲块，不处理
	if (*blk == 0)
		return false;

	//如果不是先导块则继续往后看
	if ((*blk & VM_MEM_BLK_BEG) != VM_MEM_BLK_BEG)
	{
		bool finded = false;
		for (uint32 i = blkN - 1; i >= 0; i--)
		{
			blk = vm->heap_blk_tb + i;
			if ((*blk & VM_MEM_BLK_BEG) == VM_MEM_BLK_BEG)
			{
				blkN = i;
				finded = true;
				break;
			}
		}
		//没有找到？？
		if (!finded)
			return false;
	}

	//开始释放内存
	while (true)
	{
		//释放并指向下一块
		*blk = 0;
		blk++;
		//如果遇到空闲块或者是开始块表示释放完成了
		if ((*blk == 0) || ((*blk & VM_MEM_BLK_BEG) == VM_MEM_BLK_BEG))
		{
			println("END");
			break;
		}
	}
}

/*
安装一个函数
参数：
	vm			虚拟机
	index		安装到给定的位置
	swino		中断号，用于发起系统调用
*/
void vm_install_func1(vm_info_t *vm, uint32 index, uint16 swino)
{
	/*
	arm中的函数代码大体如下
		PUSH    {R11}			保存r11的值
		ADD     R11, SP, #0		//把sp的值存到r11中
		NOP						//程序
		MOV     SP, R11			//恢复sp
		POP     {R11}			//恢复r11
		BX      LR				//返回
	*/
	//这里简单起见就不用考虑得那么复杂了，直接中断即可
	uint32 *start = vm->mem->mr_funcs + (index << 1); //每个函数两条指令所以*2
	*start = 0xef000000 | swino;					  // swi xxxx	中断
	*(start + 1) = 0xe12fff1e;						  // bx lr		返回

	//保存函数指针
	vm->mem->mr_func_tab[index] = (uint32)((void *)start - (void *)(vm->cpu->mem->buffer));
}

void vm_install_func(vm_info_t *vm)
{
	uint32 tlen = sizeof(c_func_tab) / sizeof(int16);
	for (uint32 i = 0; i < tlen; i++)
	{
		if (c_func_tab[i] > 0)
			vm_install_func1(vm, i, c_func_tab[i]);
		else
			println("TODO: %d", c_func_tab[i]);
	}
}