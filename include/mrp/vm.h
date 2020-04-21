#ifndef __MRP_VM_H__
#define __MRP_VM_H__

#include <sys/time.h>
#include "common/type.h"
#include "arm.h"
#include "helper/mr.h"

#define VM_MEM_OFFSET 0x10		  //内存装载偏移量
#define VM_MAX_PIXELS (480 * 800) //最大分辨率
#define VM_STACK_SIZE (1024 * 20) //栈大小

#define vm_mem_offset(addr) ((uint8 *)(addr) - (uint8 *)(vm->cpu->mem->buffer)) //用来获取某些值得内存偏移
#define vm_mem_buf (vm->cpu->mem->buffer)										//获取虚拟机内存的实际地址
#define vmreg(i) (vm->cpu->registers[i])										//获取某个寄存器

/*
	虚拟机内存映射表
		此表定义了mrp运行期间的内存规划
		此表会装在到VM_MEM_OFFSET指定的偏移量中
		此表之后的内存区域作为堆内存
*/
typedef struct vm_mem_map_st
{
	/*
		虚拟机实际的地址
		当虚拟机完成创建后，会将malloc得到的地址放在这里
		通过这个地址，就能反向得到虚拟机信息
	*/
	uint64 vmaddr;

	/*程序栈空间*/
	uint8 stack[VM_STACK_SIZE];

	/*显存*/
	uint8 video[VM_MAX_PIXELS * 3];

	/*mr函数表，这里存的是函数具体的地址*/
	uint32 mr_func_tab[200];

	/*
	这里存放的mr函数列表，通过mr_func_tab中的来指定。
	这里面的函数只需发起一个系统调用即可，具体的实现是交给实体机实现的。
		每条指令都是一个32位的数字，使用两条指令来处理，一条中断，一条返回
	*/
	uint32 mr_funcs[2 * 200];

	/*
	mrp运行时的一些状态
	*/
	mr_state_t mr_state;

} vm_mem_map_t;

/*
虚拟机信息
*/
typedef struct vm_info_st
{
	vm_mem_map_t *mem;		  //内存映射信息
	cpu_state_t *cpu;		  //cpu信息
	struct timeval startTime; //创建时间
} vm_info_t;

/*
创建一个虚拟机
参数：
	memSize		堆内存大小
*/
vm_info_t *vm_create(uint32 memSize);

/*
释放虚拟机
参数：
	vm		虚拟机
*/
void vm_free(vm_info_t *vm);

/*
安装函数表，调用此函数会安装mrp执行时所需的函数表。
参数：
	vm		虚拟机
*/
void vm_install_func(vm_info_t *vm);

/*
指定PC寄存器运行虚拟机，运行结束时此函数退出，并返回退出前r0的寄存器的值。
当虚拟机栈已经清空了就会退出虚拟机。
参数：
	vm		虚拟机
	pc		pc寄存器的值
返回：
	额，也就是pc指向的函数执行后的返回值
*/
uint32 vm_run(vm_info_t *vm, uint32 pc);

#endif