#ifndef __MRP_VM_H__
#define __MRP_VM_H__

#include "common/type.h"
#include "arm.h"

#define VM_MEM_OFFSET 0x10		  //内存装载偏移量
#define VM_MAX_PIXELS (480 * 800) //最大分辨率
#define VM_STACK_SIZE (1024 * 20) //栈大小

#define VM_MEM_BLOCK_SIZE 64 //内存块大小

#define VM_MEM_BLK_USD 0x01 //内存被使用
#define VM_MEM_BLK_BEG 0x02 //内存先导块，这个块之后的块都属于这个块

#define SYSCALL_PARAM_MAX 40 //系统调用支持的最大参数数量

/*
系统信息
*/
typedef struct
{
	/*
		虚拟机实际的地址
		当虚拟机完成创建后，会将malloc得到的地址放在这里
		通过这个地址，就能反向得到虚拟机信息
	*/
	uint64 vmaddr;

	/*
		网络ID
		0	移动
		1	联动GSM
		2	联通CDMA
		3	未插卡
	*/
	uint32 networkID;

	uint32 screen_width;  //屏幕宽度
	uint32 screen_height; //屏幕高度
	uint32 screen_bits;	  //屏幕色深，24位

	uint8 IMEI[16];		 //IMEI
	uint8 IMSI[16];		 //IMSI
	char manufactory[8]; //厂商名，最大7个字符，空字节填\0
	char type[8];		 //mobile type，最大7个字符，空字节填\0
	uint32 ver;			 //SW ver
	uint8 spare[12];	 //备用

} vm_sys_info_t;

/*
系统调用信息
*/
typedef struct
{
	/*系统调用编号*/
	uint32 number;
	/*系统调用的返回值*/
	uint32 result;
	/*参数列表，这里面存放的是指针*/
	uint32 params[SYSCALL_PARAM_MAX];
} vm_syscall_t;

/*
一些常用的偏移量
*/
typedef struct
{
	uint32 sys_info;	//系统信息所在的偏移量
	uint32 stack;		//栈偏移量
	uint32 video;		//显存偏移量
	uint32 mr_func_tab; //mr函数表偏移量
	uint32 syscall;		//系统调用偏移量
	uint32 heap;		//堆偏移量
} vm_offsets_t;

#define vm_mem_offset(addr) ((void *)(addr) - (void *)(vm->cpu->mem->buffer))

/*
	虚拟机内存映射表
		此表定义了mrp运行期间的内存规划
		此表会装在到VM_MEM_OFFSET指定的偏移量中
		此表之后的内存区域作为堆内存
*/
typedef struct
{
	/*系统信息*/
	vm_sys_info_t sys_info;

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
	系统调用信息
	当发生系统调用后，可以从这个信息中取得相关的系统调用信息以进行处理
	*/
	vm_syscall_t syscall;
} vm_mem_map_t;

/*
虚拟机信息
*/
typedef struct
{
	vm_mem_map_t *mem; //内存映射信息
	cpu_state_t *cpu;  //cpu信息

	/*
	虚拟机内存分配是按照固定大小的块来分配的：
		将所有内存划分成相等大小的块，分配时寻找连续的块进行分配。
	*/
	uint8 *heap_start;	//堆内存开始地址
	uint8 *heap_end;	//堆内存结束地址
	uint32 heap_size;	//总的堆内存大小（不是结束-开始，而是可用的部分的大小）
	uint32 heap_free;	//剩余的堆内存的空间
	uint32 heap_blks;	//内存块总数
	uint8 *heap_blk_tb; //内存块使用情况

	/*这里是一些常用的偏移量*/
	vm_offsets_t offsets;
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
虚拟机内存分配
参数：
	vm		虚拟机
	len	内存大小
返回：
	!null		分配成功，得到的内存地址是相对于虚拟机内存开始地址的地址
	null		分配失败
*/
uint8 *vm_mem_alloc(vm_info_t *vm, int32 len);

/*
虚拟机内存释放
参数：
	vm		虚拟机
	mem		要释放的内存的地址，这个地址应该是一个先导块，如果不是先导块的话会向前查找到先导块，如果是空闲块则忽略
返回：
	true	正常释放
	false	非正常释放，比如说越界啦，已释放啦，释放的不是先导块啦。
*/
bool vm_mem_free(vm_info_t *vm, uint8 *mem);

/*
安装函数表，调用此函数会安装mrp执行时所需的函数表。
参数：
	vm		虚拟机
*/
void vm_install_func(vm_info_t *vm);

#endif