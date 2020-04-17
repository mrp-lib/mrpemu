
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <malloc.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "mrp.h"

uint32 memSize = 1024 * 1024 * 1;

char *LG_mem_base; //VM 内存基址
int32 LG_mem_len;  //VM 内存大小
char *LG_mem_end;  //VM 内存终址
int32 LG_mem_left; //VM 剩余内存
uint32 LG_mem_min; //VM 内存底值
uint32 LG_mem_top; //VM 内存峰值
mem_header LG_mem_free;

char *vm_mem_base; //虚拟机内存地址
int32 vm_mem_len;  //虚拟机内存大小
char *vm_mem_end;  //虚拟机内存地址

#define NO_EXRAM 0
#define realLGmemSize(x) (((x) + 7) & (0xfffffff8))

#define handle_error(msg)   \
	do                      \
	{                       \
		perror(msg);        \
		exit(EXIT_FAILURE); \
	} while (0)

static char *Origin_LG_mem_base;
static uint32 Origin_LG_mem_len;

int cacheflush(void *beg, void *end, int flag)
{
	register unsigned long _beg __asm("a1") = (unsigned long)beg;
	register unsigned long _end __asm("a2") = (unsigned long)end;
	register unsigned long _flg __asm("a3") = flag;

	__asm __volatile("swi 0x9f0002        @ cacheflush"
					 : "=r"(_beg)
					 : "0"(_beg),
					   "r"(_end),
					   "r"(_flg));

	return _beg;

	// const int syscall = 0xf0002;
	// __asm __volatile(
	// 	"mov     r0, %0\n"
	// 	"mov     r1, %1\n"
	// 	"mov     r7, %2\n"
	// 	"mov     r2, #0x0\n"
	// 	"svc     0x00000000\n"
	// 	:
	// 	: "r"(beg), "r"(end), "r"(syscall)
	// 	: "r0", "r1", "r7");
}

int32 mr_mem_init(void)
{
	logi("mr_mem_init()");

	mem_header *head;
	int ret;

	ret = mr_mem_get(&Origin_LG_mem_base, &Origin_LG_mem_len);
	if (0 != ret)
		return -1;

	LG_mem_base = (char *)(((uint32)Origin_LG_mem_base + 3) & 0xfffffffc);
	LG_mem_len = (Origin_LG_mem_base + Origin_LG_mem_len - LG_mem_base) & 0xfffffffc;
	LG_mem_end = LG_mem_base + LG_mem_len;
	LG_mem_free.used = 0;
	LG_mem_free.left = 0;

	head = (mem_header *)LG_mem_base;
	head->used = LG_mem_len;
	head->left = LG_mem_len;
	LG_mem_left = LG_mem_len;
	LG_mem_min = LG_mem_len;
	LG_mem_top = 0;

	return 0;
}

void *mr_malloc(uint32 len)
{
	logi("mr_malloc(%d)", len);

	uint32 len8;
	mem_header *pNext = NULL;
	mem_header *pTop = NULL;
	mem_header *pCur = NULL;

	len8 = (len + 7) & 0xfffffff8;
	if (len8 < LG_mem_left)
	{
		if (len8)
		{
			if (&LG_mem_base[LG_mem_free.used] <= LG_mem_end)
			{
				pTop = &LG_mem_free;
				pCur = (mem_header *)&LG_mem_base[LG_mem_free.used];

				while (1)
				{
					if ((uint32)pCur >= (uint32)LG_mem_end)
					{
						printf("mr_malloc no memory\n");
						break;
					}

					if (pCur->left == len8)
					{
						pTop->used = pCur->used;
						LG_mem_left -= len8;
						goto ok;
					}
					else if (pCur->left > len8)
					{
						pNext = (mem_header *)((char *)pCur + len8);
						pNext->used = pCur->used;
						pNext->left = pCur->left - len8;
						pTop->used += len8;
						LG_mem_left -= len8;
						goto ok;
					}

					pTop = pCur;
					pCur = (mem_header *)&LG_mem_base[pCur->used];
				}
			}
			else
			{
				mr_printf("mr_malloc corrupted memory\n");
			}
		}
		else
		{
			printf("mr_malloc invalid memory request\n");
		}
	}
	else
	{
		printf("mr_malloc no memory\n");
	}

err:
	return NULL;

ok:
	if (LG_mem_left < LG_mem_min)
		LG_mem_min = LG_mem_left;
	if (LG_mem_top < pTop->used)
		LG_mem_top = pTop->used;

	return pCur;
}

void mr_free(void *p, uint32 len)
{
	logi("mr_free(%p, %d)", p, len);

	uint32 len8;
	mem_header *pCur = NULL;
	mem_header *pTop = NULL;

	len8 = (len + 7) & 0xfffffff8;
	if (len8 > 0 && NULL != p && (char *)p >= LG_mem_base && (char *)p < LG_mem_end && (char *)p + len8 <= LG_mem_end && (char *)p + len8 > LG_mem_base)
	{
		pTop = &LG_mem_free;
		for (pCur = (mem_header *)(LG_mem_base + LG_mem_free.used);
			 (char *)pCur < LG_mem_end && (char *)pCur < (char *)p; pCur =
																		(mem_header *)(LG_mem_base + pCur->used))
			pTop = pCur;
		if (p != pTop && p != pCur)
		{
			if (pTop == &LG_mem_free || (mem_header *)((char *)pTop + pTop->left) != p)
			{
				pTop->used = (char *)p - LG_mem_base;
				pTop = (mem_header *)p;
				pTop->used = (char *)pCur - LG_mem_base;
				pTop->left = len8;
			}
			else
			{
				pTop->left += len8;
			}

			if ((uint32)pCur < (uint32)LG_mem_end && (mem_header *)((char *)p + len8) == pCur)
			{
				pTop->used = pCur->used;
				pTop->left += pCur->left;
			}
			LG_mem_left += len8;
		}
		else
		{
			printf("mr_free:already free\n");
		}
	}
	else
	{
		printf("mr_free invalid\n");
		printf("%#p,%d,base=%#p,end=%p\n", p, len8, LG_mem_base, LG_mem_end);
	}
}

void *mr_realloc(void *p, uint32 oldlen, uint32 newlen)
{
	logi("mr_realloc(%p, %d, %d)", p, oldlen, newlen);

	uint32 len;
	void *Dst = NULL;

	if (oldlen <= newlen)
		len = oldlen;
	else
		len = newlen;

	if (NULL != p)
	{
		if (newlen > 0)
		{
			Dst = (void *)mr_malloc(newlen);
			if (NULL != Dst)
			{
				memmove(Dst, p, len);
				mr_free(p, oldlen);
			}
		}
		else
		{
			mr_free(p, oldlen);
		}
	}
	else
	{
		Dst = (void *)mr_malloc(newlen);
	}

	return Dst;
}

int32 mr_mem_get(char **mem_base, uint32 *mem_len)
{
	logi("mr_mem_get(%p, %p)", mem_base, mem_len);

	char *buffer;
	int pagesize, pagecount, len = memSize;

	pagesize = sysconf(_SC_PAGE_SIZE);
	if (pagesize == -1)
		handle_error("sysconf");

	pagecount = len / pagesize;
	len = pagesize * pagecount;
	buffer = memalign(pagesize, len);
	if (buffer == NULL)
		handle_error("memalign");

	//设置内存可执行权限
	if (mprotect(buffer, len, PROT_EXEC | PROT_WRITE | PROT_READ) == -1)
	{
		free(buffer);
		handle_error("mprotect");
	}

	*mem_base = buffer;
	*mem_len = len;

	vm_mem_base = buffer;
	vm_mem_len = len;
	vm_mem_end = buffer + len;

	println("mr_mem_get base=%p len=%x end=%p", vm_mem_base, len, vm_mem_end);

	return MR_SUCCESS;
}

int32 mr_mem_free(char *mem, uint32 mem_len)
{
	logi("mr_mem_free(%p, %d)", mem, mem_len);

	free(mem);

	vm_mem_base = null;
	vm_mem_len = 0;

	printf("mr_mem_free");

	return MR_SUCCESS;
}

void mr_mem_end(void)
{
	mr_mem_free(Origin_LG_mem_base, Origin_LG_mem_len);
}

void mr_cacheFlush(int id)
{
	logi("mr_cacheFlush(%d)", id);
	cacheflush(vm_mem_base, vm_mem_base + vm_mem_len, 0);
}

int32 mr_cacheSync(void *addr, int32 len)
{
	logi("mr_cacheSync(%p, %d)", addr, len);
	cacheflush(addr, addr + len, 0);
	return MR_SUCCESS;
}