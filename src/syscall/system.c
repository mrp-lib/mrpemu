#include "common/type.h"
#include "mrp.h"

#define mem ((vm_mem_map_t *)(VM_MEM_OFFSET))
#define syscall(nu)               \
	(mem->syscall.number = (nu)); \
	asm("swi #0x8900")

typedef struct
{
	uint32 a;
	uint32 b;
	uint32 c;
	uint32 d;
	uint32 e;
	uint32 f;
	uint32 g;
	uint32 h;
} test_struct;

extern uint32 mr_test1()
{
	mem->syscall.number = 4333;
	asm("swi #0x8900");
}

extern void mr_test2(uint32 a) {}

extern void mr_test3(uint32 a, uint32 b) {}

extern void mr_test4(uint32 a, uint32 b, uint32 c) {}

extern void mr_test5(uint32 a, uint32 b, uint32 c, uint32 d) {}

extern void mr_test6(uint32 a, uint32 b, uint32 c, uint32 d, uint32 e) {}

extern void mr_test7(uint32 a, uint32 b, uint32 c, uint32 d, uint32 e, uint32 f) {}

extern void mr_test8(uint8 a, uint16 b, uint32 c, uint64 d, uint32 e, uint32 f) {}

extern void mr_test9(test_struct stct) {}