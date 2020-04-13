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

#define lsl(val, shift) ((uint32)(val) << (shift))					 //逻辑左移
#define asl(val, shift) ((val) << (shift))							 //算术左移
#define lsr(val, shift) ((uint32)(val) >> (shift))					 //逻辑右移
#define asr(val, shift) ((val) >> (shift))							 //算术右移
#define ror(val, shift) (lsr(val, shift) | lsl(val, (32 - (shift)))) //循环右移

#define regv(R) (st->registers[R]) //获取寄存器的值

typedef struct
{
	//31  30  29  28  27  26-25   24  23-20      19-16    15-10     9  8  7  6  5  4-0
	//N   Z   C   V   Q   Res     J   RESERVED   GE[3:0]  RESERVED  E  A  I  F  T  M[4:0]

	bool n;
	bool z;
	bool c;
	bool v;

	bool q;

	bool j;
	uint32 ge;

	bool e;
	bool a;
	bool i;
	bool f;
	bool t;

	uint32 mode;

} cpu_psr_t;

//CPU状态
typedef struct
{
	uint32 registers[16]; //寄存器列表
	cpu_psr_t cpsr;		  //cpsr寄存器

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

//取cpsr寄存器
uint32 cpu_cpsr(cpu_state_t *st);

//执行指令
int32 cpu_exec_inst(cpu_state_t *st, uint32 inst);

//打印寄存器信息
void cpu_print_regs(cpu_state_t *st);

//取得psr寄存器的值
uint32 psr_ldval(cpu_psr_t *psr);
//设置psr寄存器的值
uint32 psr_stval(cpu_psr_t *psr, uint32 val);

#endif