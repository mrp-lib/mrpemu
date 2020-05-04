#ifndef __MRP_VM_H__
#define __MRP_VM_H__

#include <sys/time.h>
#include "common/type.h"
#include "arm.h"
#include "mrp/common/helper.h"
#include "mrp/common/state.h"
#include "sys/font.h"

#define VM_MEM_OFFSET 0x10		  //内存装载偏移量
#define VM_MAX_PIXELS (480 * 800) //最大分辨率
#define VM_STACK_SIZE (1024 * 20) //栈大小
#define VM_POINTERS_MAX 5		  //虚拟机特殊指针个数

#define vm_mem_offset(addr) ((uint8 *)(addr) - (uint8 *)(vm->cpu->mem->buffer)) //用来获取某些值得内存偏移
#define vm_mem_buf (vm->cpu->mem->buffer)										//获取虚拟机内存的实际地址
#define vmreg(i) (vm->cpu->registers[i])										//获取某个寄存器
#define vmstack(i) (*((uint32 *)(vm_mem_buf + vmreg(r_sp)) + i))				//获取当前栈顶偏移某个位置的值

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
	void *vmaddr;

	/*程序栈空间*/
	uint8 stack[VM_STACK_SIZE];

	/*显存*/
	uint16 video[VM_MAX_PIXELS];

	/*
	字体信息，必要的时候会自动加载，虚拟机释放时自动释放。
	*/
	font_info_t font;

	/*
	这里面存储的是一些指针，在mr_func_tab中会遇到一些指向指针的指针，把那些部分存在这里
	为了简单，也分配和mr_func_tab相等大小的空间，这样也就可以一一对应了。
	*/
	uint32 mr_func_tab_p[200];

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

	/*堆内存开始地址*/
	uint8 heap[0];

} vm_mem_map_t;

/*
虚拟机信息
*/
typedef struct vm_info_st
{
	vm_mem_map_t *mem;				 //内存映射信息
	cpu_state_t *cpu;				 //cpu信息
	struct timeval startTime;		 //创建时间
	vm_callback_t callbacks;		 //虚拟机回调函数列表
	void *pointers[VM_POINTERS_MAX]; //一些特殊的指针，调用者可以从vm信息中反向得到事先指定的资源
} vm_info_t;

/*额，用于外部调用进行虚拟机配置*/
typedef struct
{
	uint32 screenWidth;
	uint32 screenHeight;
	char *IMEI;
	char *IMSI;
	char *manufactory;
	char *type;
	uint32 networkId;
	char *sdcardDir;
	char *dsmDir;
} vm_config_t;

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
指定PC寄存器运行虚拟机，运行结束时此函数退出，并返回退出前r0的寄存器的值。
当虚拟机栈已经清空了就会退出虚拟机。
参数：
	vm		虚拟机
	pc		pc寄存器的值
返回：
	额，也就是pc指向的函数执行后的返回值
*/
uint32 vm_run(vm_info_t *vm, uint32 pc);

/*获取显存地址及长度*/
extern uint16 *vm_getVideo(vm_info_t *vm, uint32 *size);

/*进行虚拟机配置*/
extern void vm_setConfig(vm_info_t *vm, vm_config_t *conf);

/*用来获取虚拟机特殊指针*/
extern void **vm_getPointers(vm_info_t *vm);

/*获取回调函数空间地址*/
extern vm_callback_t *vm_getCallbacks(vm_info_t *vm);

#endif