#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arm.h"
#include "helper/elf.h"

#include <math.h>

#define memSize 0x200000 //1024 * 1024 * 2;

//中断处理程序
int32 on_sorftware_interrupt(cpu_state_t *st, uint32 nu)
{
	return 0;
}

int main()
{
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