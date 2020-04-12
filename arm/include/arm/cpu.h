#ifndef __ARM_CPU_H__
#define __ARM_CPU_H__

#include "common/type.h"
#include "arm/mem.h"

#define r_sp 13 //SP寄存器（栈）
#define r_lr 14 //LR寄存器（返回）
#define r_pc 15 //PC寄存器（程序计数器）

#define EXEC_SUCCESS 0		 //指令执行成功
#define EXEC_IGNORE 1		 //指令被忽略（条件不允许执行）
#define EXEC_UNPREDICTABLE 2 //不可预料
#define EXEC_NOT_IMPL 3		 //指令未实现
#define EXEC_ERROR -1		 //执行失败

#define lsl(val, shift) ((uint32)val << shift)					   //逻辑左移
#define asl(val, shift) (val << shift)							   //算术左移
#define lsr(val, shift) ((uint32)val >> shift)					   //逻辑右移
#define asr(val, shift) (val >> shift)							   //算术右移
#define ror(val, shift) (lsr(val, shift) | lsl(val, (32 - shift))) //循环右移

//CPU状态
typedef struct
{
	uint32 registers[16]; //寄存器列表
	//cpsr
	bool n;
	bool z;
	bool c;
	bool v;
	bool t;

	//内存空间
	memory_t *mem;

} cpu_state_t;

typedef int32 (*arm_inst_func_t)(cpu_state_t *st, uint32 inst); //arm指令处理函数

//创建一个CPU并制定内存大小
cpu_state_t *cpu_create(memory_t *mem);

//销毁CPU
void cpu_destory(cpu_state_t *st);

//取出指令
uint32 cpu_fetch_inst(cpu_state_t *st);

//执行指令
int32 cpu_exec_inst(cpu_state_t *st, uint32 inst);

//打印寄存器信息
void cpu_print_regs(cpu_state_t *st);

#endif