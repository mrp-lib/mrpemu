#ifndef __MR_MEM_H__
#define __MR_MEM_H__

#include "common/type.h"

typedef void *(*T_mr_malloc)(uint32 len);
typedef void (*T_mr_free)(void *p, uint32 len);
typedef void *(*T_mr_realloc)(void *p, uint32 oldlen, uint32 len);
typedef int32 (*T_mr_mem_get)(char **mem_base, uint32 *mem_len);
typedef int32 (*T_mr_mem_free)(char *mem, uint32 mem_len);

typedef struct mem_header_st
{
	uint32 used;
	uint32 left;
} mem_header;

/** 为虚拟机分配的内存的大小（默认1M） */
extern uint32 memSize;

extern char *LG_mem_base; //VM 内存基址
extern int32 LG_mem_len;  //VM 内存大小
extern char *LG_mem_end;  //VM 内存终址
extern int32 LG_mem_left; //VM 剩余内存
extern uint32 LG_mem_min; //VM 内存底值
extern uint32 LG_mem_top; //VM 内存峰值
extern mem_header LG_mem_free;

extern char *vm_mem_base; //虚拟机内存地址
extern int32 vm_mem_len;  //虚拟机内存大小
extern char *vm_mem_end;  //虚拟机内存地址

/*内存申请*/
extern int32 mr_mem_get(char **mem_base, uint32 *mem_len);
/*内存释放*/
extern int32 mr_mem_free(char *mem, uint32 mem_len);

void mr_mem_end(void);

/**初始化内存*/
int32 mr_mem_init(void);

/*分配内存*/
void *mr_malloc(uint32 len);

/*释放内存*/
void mr_free(void *p, uint32 len);

/*追加内存*/
void *mr_realloc(void *p, uint32 oldlen, uint32 len);

void mr_cacheFlush(int id);

int32 mr_cacheSync(void *addr, int32 len);

#endif