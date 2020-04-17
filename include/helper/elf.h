#ifndef __HELPER_ELF_H__
#define __HELPER_ELF_H__

#include "stdio.h"
#include "common/type.h"

#define SHT_NULL 0	   //无效段
#define SHT_PROGBITS 1 //程序段、代码段、数据段都是此种类型
#define SHT_SYMTAB 2   //表示该段的类内容为符号表
#define SHT_STRTAB 3   //表示该段的内容是字符串表
#define SHT_RELA 4	   //重定位表，该段包含了重定位信息
#define SHT_HASH 5	   //符号表的哈希表
#define SHT_DYNAMIC 6  //动态链接信息
#define SHT_NOTE 7	   //提示性信息
#define SHT_NOBITS 8   //表示该段在文件中没有内容，比如.bss段
#define SHT_REL 9	   //该段包含了重定位信息
#define SHT_SHLIB 10   //该段保留
#define SHT_DNYSYM 11  //动态链接的符号表

#define SHF_WRITE 1		//表示该段在进程空间中可写
#define SHF_ALLOC 2		//表示该段需要在进程空间中分配空间
#define SHF_EXECINSTR 4 //表示该段在进程空间中可以被执行

typedef struct
{
	//魔术字段
	struct
	{
		char type[4];	  //7f 45 4c 46
		uint8 bits;		  //是多少位的，01:32位，02:64位
		uint8 endian;	  //小端还是大端，01:小端，02:大端
		uint8 version;	  //版本号，一般是01
		uint8 reserve[8]; //保留
	} magic;

	uint16 e_type;		//类型, 01: ET_REL(可重定位文件，一般位.o文件), 02: ET_EXEC(可执行文件), 03: ET_DYN(共享目标文件，一般位.so文件)
	uint16 e_machine;	//系统架构
	uint32 e_version;	//版本号
	uint32 e_entry;		//入口点地址
	uint32 e_phoff;		//程序头位置
	uint32 e_shoff;		//段头位置
	uint32 e_flags;		//标记
	uint16 e_ehsize;	//当前头的大小
	uint16 e_phentsize; //程序头大小
	uint16 e_phnum;		//程序头数量
	uint16 e_shentsize; //段头大小
	uint16 e_shnum;		//段头数量
	uint16 e_shstrndx;	//段头字符串表的位置

} elf_head_t;

//段表结构
typedef struct
{
	uint32 sh_name;	  //section的名字
	uint32 sh_type;	  //section类别
	uint32 sh_flags;  //section在进程中执行的特性（读、写）
	uint32 sh_addr;	  //在内存中开始的虚地址
	uint32 sh_offset; //此section在文件中的偏移
	uint32 sh_size;
	uint32 sh_link;
	uint32 sh_info;
	uint32 sh_addralign;
	uint32 sh_entsize;
} elf_sec_head_t;

typedef struct
{
	FILE *fp;
} elf_t;

/**
 * 打开ELF文件
 * 参数：
 * 		filename		文件名
 * 返回：
 * 		!=null		成功
 * 		null		失败
 */
elf_t *elf_open(char *filename);

/**
 * 关闭ELF文件
 * 参数：
 * 		elf		elf文件
 */
void elf_close(elf_t *elf);

/**
 * 读取ELF头
 * 参数：
 * 		elf		elf文件
 * 		head	头结构
 * 返回：
 * 		0		成功
 * 		-1		失败
 * 		-2		不是elf文件
 */
uint32 elf_read_head(elf_t *elf, elf_head_t *head);

/**
 * 打印elf头
 */
void elf_print_head(elf_head_t *head);

/**
 * 获取所有的段头
 * 参数：
 * 		elf		elf文件
 * 		head	elf头信息
 * 		shs		段头列表的指针（这个指针指向多个段哦）
 * 返回：
 * 		0		成功
 * 		-1		失败
 */
uint32 elf_read_sec_heads(elf_t *elf, elf_head_t *head, elf_sec_head_t *shs);

/**
 * 打印段头
 * 参数：
 * 		elf			elf文件（如果是null则不会打印名称）
 * 		sh			头信息
 * 		index		位置信息
 * 		has_th		是否打印表头
 */
void elf_print_sec_head(elf_t *elf, elf_head_t *head, elf_sec_head_t *shs, uint32 index, bool has_th);

#endif