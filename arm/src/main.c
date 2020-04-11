#include <stdio.h>
#include "arm.h"

int main()
{
	println("--------------------->");

	uint32 mem_size = 1024 * 1024 * 10; //10m内存

	cpu_state_t *st = cpu_create(mem_size);

	uint32 inst = cpu_fetch_inst(st);

	cpu_exec_inst(st, inst);

	cpu_destory(st);

	// uint32 a = 0x80000000;
	// uint32 b = 0x0;
	// uint32 result = a - b;
	// uint32 c = result > a;
	// uint32 v = (a ^ b) & (a ^ result);
	// println("%d-%d=%d，%u-%u=%u, c=%d, v=%d", a, b, result, a, b, result, c, v);

	return 0;
}