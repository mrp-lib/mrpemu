#include <stdio.h>
#include <stdlib.h>
#include "arm.h"
#include "elf.h"

#include <math.h>

int main()
{
	// println("int32 %d ~ %d", (int32)(-powl(2, 31)),(int32)(powl(2, 31) - 1));
	// println("uint32 0~%d", (int32)(powl(2, 32)-1));
	// printf("\n");
	// println("int16 %d ~ %d", (int32)(-powl(2, 15)),(int32)(powl(2, 15) - 1));
	// println("uint16 0~%d", (int32)(powl(2, 16)-1));
	// printf("\n");
	// println("int8 %d ~ %d", (int32)(-powl(2, 7)),(int32)(powl(2, 7) - 1));
	// println("uint8 0~%d", (int32)(powl(2, 8)-1));

	println("--------------------->");
	//打开测试用到的elf文件
	elf_head_t head;
	elf_sec_head_t *secs;

	elf_t *elf = elf_open("/mnt/c/develop/arm/test1/test1_Data/DebugRel/test1.axf");
	elf_read_head(elf, &head);
	elf_print_head(&head);
	//获取段列表
	secs = malloc(sizeof(elf_sec_head_t) * head.e_shnum);
	elf_read_sec_heads(elf, &head, secs);
	for (uint32 i = 0; i < head.e_shnum; i++)
	{
		elf_print_sec_head(elf, &head, secs, i, i == 0);
	}

	uint32 mem_size = 1024 * 1024 * 10; //10m内存

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

	st->registers[r_pc] = head.e_entry;
	psr_stval(&st->cpsr, 0x000000d3); //cpsr默认为d3

	println("--------------------->");

	st->registers[1] = 8;
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