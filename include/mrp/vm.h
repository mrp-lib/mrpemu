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
#define vmstack(i) (*((uint32 *)(vm_mem_buf + vmreg(r_sp)) + i))				//获取当前栈顶偏移某个位置的值

/*
屏幕刷新回调函数，次函数由调用者传递。
*/
typedef void (*on_screen_refresh_t)(uint16 *bmp, int16 x, int16 y, uint16 w, uint16 h);

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
	uint16 video[VM_MAX_PIXELS];

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
	vm_mem_map_t *mem;			   //内存映射信息
	cpu_state_t *cpu;			   //cpu信息
	struct timeval startTime;	   //创建时间
	on_screen_refresh_t onRefresh; //屏幕刷新回调
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

/*
获取显存地址及长度
这个函数主要提供给外部使用，外部通过调用这个函数可以读取显存并显示到屏幕上
*/
extern uint16 *vm_getVideo(vm_info_t *vm, uint32 *size);

/*
设置屏幕信息
这个函数主要提供给外部使用，外部通过调用这个函数可以实现屏幕分辨率的设置，不过不能超过最大分辨率
*/
extern void vm_setScreen(vm_info_t *vm, uint32 width, uint32 height, uint32 bits);

/*
设置设备信息
这个函数主要提供给外界使用，外部调用此接口实现系统信息的设置
*/
extern void vm_setSystemInfo(vm_info_t *vm, char *IMEI, char *IMSI, char *manufactory, char *type);

/*
设置网络ID: 0移动, 1联动GSM, 2联通CDMA, 3未插卡
这个函数主要提供给外界使用，用来设置系统的网络ID
*/
extern void vm_setNetworkId(vm_info_t *vm, uint32 networkId);

/*
设置存储空间
这个函数主要提供给外部使用，用于设置sdcard目录以及mythroad目录
*/
extern void vm_setStorage(vm_info_t *vm, char *sdcard_dir, char *dsm_dir);

/*
设置屏幕刷新回调
这个函数主要提供给外部使用，用来监听屏幕刷新，在回调函数中可以对数据进行绘制。
*/
extern void vm_setOnRefresh(vm_info_t *vm, on_screen_refresh_t cb);

#endif