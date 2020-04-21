#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <math.h>

#include "helper/elf.h"
#include "helper/swi.h"

#include "arm.h"
#include "mrp.h"
#include "utils/array.h"
#include "utils/gzip.h"

#define memSize (1024 * 1024 * 2);

//中断处理程序
void handle_interapt(vm_info_t *vm, uint32 nu)
{
	//处理中断号
	switch (nu)
	{
	case SWI_MR_C_FUNCTION_NEW:
		return swi_mr_c_function_new(vm);
	case SWI_MR_MALLOC:
		return swi_mr_malloc(vm);
	case SWI_MR_MEMMOVE:
		return swi_mr_memmove(vm);
	case SWI_MR_MEMSET:
		return swi_mr_memset(vm);
	case SWI_MR_MEMCPY:
		return swi_mr_memcmp(vm);
	case SWI_MR_MEMCHR:
		return swi_mr_memchr(vm);
	case SWI_MR_STRCPY:
		return swi_mr_strcpy(vm);
	case SWI_MR_STRNCPY:
		return swi_mr_strncpy(vm);
	case SWI_MR_STRCAT:
		return swi_mr_strcat(vm);
	case SWI_MR_STRNCAT:
		return swi_mr_strncat(vm);
	case SWI_MR_STRCMP:
		return swi_mr_strcmp(vm);
	case SWI_MR_STRNCMP:
		return swi_mr_strncmp(vm);
	case SWI_MR_STRCOLL:
		return swi_mr_strcoll(vm);
	case SWI_MR_STRLEN:
		return swi_mr_strlen(vm);
	case SWI_MR_STRSTR:
		return swi_mr_strstr(vm);
	case SWI_MR_ATOI:
		return swi_mr_atoi(vm);
	case SWI_MR_STRTOUL:
		return swi_mr_strtoul(vm);
	case SWI_MR_RAND:
		return swi_mr_rand(vm);
	case SWI_MR_SPRINTF:
		return swi_mr_sprintf(vm);
	case SWI_MR_PRINTF:
		return swi_mr_printf(vm);
	case SWI_MR_WSTRLEN:
		return swi_mr_wstrlen(vm);
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

int32 test_mrp()
{
	char *file = "%sdcard/mythroad/hello.mrp";

	//创建虚拟机
	vm_info_t *vm = vm_create(1024 * 400);
	mem_init(vm);		 //初始化虚拟机内存
	vm_install_func(vm); //安装函数

	//设置一下虚拟机信息
	mrst.sysinfo.screen_width = 240;
	mrst.sysinfo.screen_height = 320;
	mrst.sysinfo.screen_bits = 24; //这个填死就好
	mrst.sysinfo.networkID = 0;	   //中国移动（讨厌联通，用移动，不过也好不到哪里去）
	strcpy(mrst.sysinfo.IMEI, "123456789abcdef");
	strcpy(mrst.sysinfo.IMSI, "aadfsd");
	strcpy(mrst.sysinfo.manufactory, "YIZHI"); //厂商
	strcpy(mrst.sysinfo.type, "MRPEMU");	   //手机类型

	//启动mrp
	start_dsmC(vm, file);

	vm_free(vm);

	return 0;
}

int main(int32 argc, char **argv)
{
	{
		// char cwd[1024] = {0};
		// getcwd(cwd, 1024);
		// println("CWD: %s", cwd);
		// println("ARGS:");
		// for (uint32 i = 0; i < argc; i++)
		// {
		// 	println("\t[%2d]  %s", i + 1, *(argv + i));
		// }
	}
	// println("%s");
	return test_mrp();

	//打开测试用到的elf文件
	elf_head_t head;
	elf_sec_head_t *secs;

	// elf_t *elf = elf_open("/mnt/c/develop/arm/test1/test1_Data/DebugRel/test1.axf");
	elf_t *elf = elf_open("/mnt/c/develop/arm/test/test");
	elf_read_head(elf, &head);
	elf_print_head(&head);
	//获取段列表
	secs = (elf_sec_head_t *)malloc(sizeof(elf_sec_head_t) * head.e_shnum);
	elf_read_sec_heads(elf, &head, secs);
	for (uint32 i = 0; i < head.e_shnum; i++)
	{
		elf_print_sec_head(elf, &head, secs, i, i == 0);
	}

	uint32 mem_size = memSize; //10m内存

	memory_t *mem = mem_create(mem_size);

	cpu_state_t *st = cpu_create(mem);

	//初始化内存
	println("正在初始化内存...");
	for (uint32 i = 0; i < 50; i++)
	{
		mem_st32(mem, i * 4, i + 1);
	}
	//加载段
	println("正在加载程序");
	for (uint32 i = 0; i < head.e_shnum; i++)
	{
		elf_sec_head_t *sh = secs + i;
		//忽略无效段
		if (sh->sh_type == SHT_NULL)
			continue;

		//读取段数据
		fseek(elf->fp, sh->sh_offset, SEEK_SET);
		uint8 *pos = mem->buffer + sh->sh_addr;
		fread(pos, 1, sh->sh_size, elf->fp);
	}

	st->registers[r_pc] = 0x103d0; //head.e_entry;
	st->registers[r_sp] = 0x303d0; //head.e_entry;

	// psr_stval(&st->cpsr, 0x000000d3); //cpsr默认为d3
	psr_stval(&st->cpsr, 0x60000010); //cpsr默认为d3

	println("--------------------->");

	// st->registers[2] = 0x00e0;
	// st->registers[3] = 0xf0;

	// mem_st32(mem, 0x8004, 0xe16f1f10); //B --> 到下一条
	// mem_st32(mem, 0x8008, 0xe3a00011);

	cpu_print_regs(st);
	while (true)
	{
		uint32 inst = cpu_fetch_inst(st);
		println("inst: 0x%08x", inst);
		cpu_exec_inst(st, inst);
		cpu_print_regs(st);
	}

	cpu_destory(st);

	return 0;
}