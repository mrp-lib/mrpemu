#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elf.h"

#define elf_ok(elf) (elf != null && elf->fp != null)

//打开ELF文件
elf_t *elf_open(char *filename)
{
	//打开文件
	FILE *fp = fopen(filename, "r");
	if (fp == null)
		return null;

	elf_t *elf = malloc(sizeof(elf_t));
	if (elf == null)
	{
		fclose(fp);
		return null;
	}

	elf->fp = fp;

	return elf;
}

//关闭ELF文件
void elf_close(elf_t *elf)
{
	if (elf != null)
	{
		if (elf->fp != null)
			fclose(elf->fp);
		free(elf);
	}
}

//读取ELF头
uint32 elf_read_head(elf_t *elf, elf_head_t *head)
{
	//正常再读取
	if (elf_ok(elf))
	{
		if (-1 == fseek(elf->fp, 0, SEEK_SET))
			return -1;
		//读取
		size_t readed = fread(head, 1, sizeof(elf_head_t), elf->fp);
		if (readed != sizeof(elf_head_t))
		{
			memset(head, 0, sizeof(elf_head_t));
			return -1;
		}

		//检测是不是elf文件
		if (head->magic.type[0] != 0x7f || memcmp(head->magic.type + 1, "ELF", 3) != 0)
		{
			memset(head, 0, sizeof(elf_head_t));
			return -2;
		}

		return 0;
	}
	//否则直接失败
	return -1;
}

//打印头
void elf_print_head(elf_head_t *head)
{
	if (head == null)
	{
		printf("ELF_HEAD: NULL\n");
	}
	else
	{

		char head_type_dict[][10] = {"", "ET_REL", "ET_EXEC", "ET_DYN"};

		printf("ELF_HEAD:\n");

		printf("  MAGIC:\n");
		printf("    type:     7f 45 4c 46\n");
		printf("    endian:   %s\n", (head->magic.endian == 1) ? "small" : "big");
		printf("    version:  %d\n", head->magic.version);
		printf("    bit:      %d\n", (head->magic.bits == 1) ? 32 : 64);

		printf("  TYPE:                              %s\n", head_type_dict[head->e_type]);
		printf("  MACHINE:                           %d\n", head->e_machine);
		printf("  VERSION:                           %d\n", head->e_version);
		printf("  ENTRY:                             0x%08x(%d)\n", head->e_entry, head->e_entry);
		printf("  PROGRAM HEADER OFFSET:             0x%08x(%d)\n", head->e_phoff, head->e_phoff);
		printf("  SECTION HEADER OFFSET:             0x%08x(%d)\n", head->e_shoff, head->e_shoff);
		printf("  FLAGS:                             0x%08x(%d)\n", head->e_flags, head->e_flags);
		printf("  HEADER SIZE:                       0x%08x(%d)\n", head->e_ehsize, head->e_ehsize);
		printf("  PROGRAM HEADER SIZE:               0x%08x(%d)\n", head->e_phentsize, head->e_phentsize);
		printf("  PROGRAM HEADERS NUM:               0x%08x(%d)\n", head->e_phnum, head->e_phnum);
		printf("  SECTION HEADER SIZE:               0x%08x(%d)\n", head->e_shentsize, head->e_shentsize);
		printf("  SECTION HEADERS NUM:               0x%08x(%d)\n", head->e_shnum, head->e_shnum);
		printf("  SECTION HEADER STRING TABLE INDEX: 0x%08x(%d)\n", head->e_shstrndx, head->e_shstrndx);
		printf("\n");
	}
}

//读取段头列表
uint32 elf_read_sec_heads(elf_t *elf, elf_head_t *head, elf_sec_head_t *shs)
{
	if (elf_ok(elf) && head != null && shs != null)
	{
		if (-1 == fseek(elf->fp, head->e_shoff, SEEK_SET))
			return -1;

		uint32 size = head->e_shnum * sizeof(elf_sec_head_t);
		if (size != fread(shs, 1, size, elf->fp))
			return -1;

		return 0;
	}
	return -1;
}

//获取名称

//打印段头
void elf_print_sec_head(elf_t *elf, elf_head_t *head, elf_sec_head_t *shs, uint32 index, bool has_th)
{
	elf_sec_head_t *sh = shs + index;
	if (sh != null)
	{
		if (has_th)
			printf("[Nr] Name              Addr      Off       Size      Flg\n");

		if (sh != null)
		{
			char name[16] = {0};
			char flag[4] = {0};

			//读取名称
			if (elf != null && sh->sh_type != SHT_NULL)
			{
				if (0 == fseek(elf->fp, (shs + head->e_shstrndx)->sh_offset + sh->sh_name, SEEK_SET))
				{
					if (sizeof(name) != fread(name, 1, sizeof(name), elf->fp))
					{
						memset(name, 0, sizeof(name));
					}
				}
			}

			//取得TAG
			if ((sh->sh_flags & SHF_WRITE) == SHF_WRITE)
				sprintf(flag, "%s%c", flag, 'W');
			if ((sh->sh_flags & SHF_ALLOC) == SHF_ALLOC)
				sprintf(flag, "%s%c", flag, 'A');
			if ((sh->sh_flags & SHF_EXECINSTR) == SHF_EXECINSTR)
				sprintf(flag, "%s%c", flag, 'X');

			printf("[%2d] %-16.16s  %-8x  %-8x  %-8x  %3s\n", index, name, sh->sh_addr, sh->sh_offset, sh->sh_size, flag);
		}
	}
}