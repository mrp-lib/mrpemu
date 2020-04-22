#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arm.h"
#include "mrp.h"

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
	DATAP_C_PORT_TABLE,
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

	DATAP_SCREENBUF,
	DATAP_SCREENWIDTH,
	DATAP_SCREENHEIGHT,
	DATAP_SCREENBIT,
	DATAP_BITMAP,
	DATAP_TILE,
	DATAP_MAP,
	DATAP_SOUND,
	DATAP_SPRITE,

	DATAP_PACK_FILENAME,
	DATAP_START_FILENAME,
	DATAP_OLD_PACK_FILENAME,
	DATAP_OLD_START_FILENAME,

	DATAP_RAM_FILE,
	DATAP_RAM_FILE_LEN,

	DATAP_SOUNDON,
	DATAP_SHAKEON,

	DATAP_LG_MEM_BASE,
	DATAP_LG_MEM_LEN,
	DATAP_LG_MEM_END,
	DATAP_LG_MEM_LEFT,

	DATAP_SMS_CFG_BUF,

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

	DATAP_LG_MEM_MIN,
	DATAP_LG_MEM_TOP,
	SWI_MR_UPDCRC,
	DATAP_START_FILE_PARAMETER,
	DATAP_SMS_RETURN_FLAG,
	DATAP_SMS_RETURN_VAL,
	SWI_MR_UNZIP,
	DATAP_EXIT_CB,
	DATAP_EXIT_CB_DATA,
	DATAP_ENTRY,
	SWI_MR_PLATDRAWCHAR,
	DATAP_LG_MEM_FREE,
	SWI_MR_TRANSBITMAPDRAW,
	SWI_MR_DRAWREGION,
};

vm_info_t *vm_create(uint32 memSize)
{
	//内存分配
	uint32 cpu_mem_size = sizeof(vm_mem_map_t) + VM_MEM_OFFSET + memSize; //计算CPU内存大小
	memory_t *mem = mem_create(cpu_mem_size);							  //分配内存
	if (mem == null)
		return null;

	logi("vm_create(memSize=%d, totalSize=%d)", memSize, cpu_mem_size);

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
	memset(vm, 0, sizeof(vm_info_t));

	//初始化
	memMap->vmaddr = (uint64)vm;		//保存虚拟机地址
	vm->mem = memMap;					//保存内存映射
	vm->cpu = cpu;						//保存CPU信息
	gettimeofday(&vm->startTime, NULL); //初始化创建时间

	//设置一下虚拟机的栈，由于栈是从高往第的，所以SP指向栈的最后一个地址
	vm->cpu->registers[r_sp] = vm_mem_offset(vm->mem->stack + VM_STACK_SIZE);

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
		if (c_func_tab[i] == 0)
			continue;
		else if (c_func_tab[i] > 0)
			vm_install_func1(vm, i, c_func_tab[i]);
		else
		{
			switch (c_func_tab[i])
			{
			//内存相关的
			case DATAP_LG_MEM_BASE:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mem_state.base);
				break;
			case DATAP_LG_MEM_LEN:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mem_state.len);
				break;
			case DATAP_LG_MEM_END:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mem_state.end);
				break;
			case DATAP_LG_MEM_LEFT:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mem_state.left);
				break;
			case DATAP_LG_MEM_MIN:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mem_state.min);
				break;
			case DATAP_LG_MEM_TOP:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mem_state.top);
				break;
			case DATAP_LG_MEM_FREE:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mem_state.free);
				break;
			//屏幕相关的
			case DATAP_SCREENWIDTH:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.sysinfo.screen_width);
				break;
			case DATAP_SCREENHEIGHT:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.sysinfo.screen_height);
				break;
			case DATAP_SCREENBIT:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.sysinfo.screen_bits);
				break;
			case DATAP_SCREENBUF:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&vm->mem->video);
				break;
			//mrp包信息
			case DATAP_PACK_FILENAME:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.pack_filename);
				break;
			case DATAP_OLD_PACK_FILENAME:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.old_pack_filename);
				break;
			case DATAP_START_FILENAME:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.start_filename);
				break;
			case DATAP_OLD_START_FILENAME:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.old_start_filename);
				break;
			case DATAP_START_FILE_PARAMETER:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.start_file_parameter);
				break;
			case DATAP_ENTRY:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.entry);
				break;
			//系统相关
			case DATAP_SOUNDON:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_soundOn);
				break;
			case DATAP_SHAKEON:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_shakeOn);
				break;
			case DATAP_C_INTERNAL_TABLE:
				mrst.mr_c_internal_table[0] = vm_mem_offset(&mrst.mr_m0_files);
				mrst.mr_c_internal_table[1] = vm_mem_offset(&mrst.vm_state);
				mrst.mr_c_internal_table[2] = vm_mem_offset(&mrst.mr_state);
				mrst.mr_c_internal_table[3] = vm_mem_offset(&mrst.bi);
				mrst.mr_c_internal_table[4] = vm_mem_offset(&mrst.mr_timer_p);
				mrst.mr_c_internal_table[5] = vm_mem_offset(&mrst.mr_timer_state);
				mrst.mr_c_internal_table[6] = vm_mem_offset(&mrst.mr_timer_run_without_pause);
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_c_internal_table);
				break;
			case DATAP_RAM_FILE:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_ram_file);
				break;
			case DATAP_RAM_FILE_LEN:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_ram_file_len);
				break;
			case DATAP_C_PORT_TABLE:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_c_port_table);
				break;
			//短信
			case DATAP_SMS_RETURN_FLAG:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_sms_return_flag);
				break;
			case DATAP_SMS_RETURN_VAL:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_sms_return_val);
				break;
			case DATAP_SMS_CFG_BUF:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_sms_cfg_buf);
				break;
			//一些资源相关的
			case DATAP_BITMAP:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_bitmap);
				break;
			case DATAP_TILE:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_tile);
				break;
			case DATAP_MAP:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_map);
				break;
			case DATAP_SOUND:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_sound);
				break;
			case DATAP_SPRITE:
				vm->mem->mr_func_tab[i] = vm_mem_offset(&mrst.mr_sprite);
				break;
			default:
				println("FUNCTABLE TODO:%d", c_func_tab[i]);
				break;
			}
		}
	}
}

uint32 vm_run(vm_info_t *vm, uint32 pc)
{
	vm->cpu->registers[r_lr] = 0;  //先设置lr寄存器位0，以便函数调用结束后pc为0
	vm->cpu->registers[r_pc] = pc; //设置pc寄存器，指向函数入口

	//如果PC寄存器一直不是0则一直循环，当PC指定的函数运行结束并返回后，此时的PC应该是0
	while (vm->cpu->registers[r_pc] != 0)
	{
		uint32 inst = cpu_fetch_inst(vm->cpu); //取指令
		cpu_exec_inst(vm->cpu, inst);		   //执行指令
											   // cpu_print_regs(vm->cpu);
	}

	//返回函数执行后的返回结果
	return vm->cpu->registers[0];
}