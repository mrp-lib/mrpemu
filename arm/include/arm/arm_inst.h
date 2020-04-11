#ifndef __ARM_ARM_INST_H__
#define __ARM_ARM_INST_H__

#include "common/type.h"
#include "cpu.h"

typedef int32 (*arm_inst_func_t)(cpu_state_t *st, uint32 inst); //arm指令处理函数

//指令函数表项
typedef struct
{
	uint32 match;			  //匹配的值
	uint32 result;			  //结果
	arm_inst_func_t resolver; //处理函数
} arm_inst_table_item_t;

uint32 addr_mode_2(cpu_state_t *st, uint32 inst);
bool shifter_operand(cpu_state_t *st, uint32 inst, uint32 *operand);

int32 arm_inst_adc(cpu_state_t *st, uint32 inst);
int32 arm_inst_add(cpu_state_t *st, uint32 inst);
int32 arm_inst_and(cpu_state_t *st, uint32 inst);
int32 arm_inst_b(cpu_state_t *st, uint32 inst);
int32 arm_inst_bic(cpu_state_t *st, uint32 inst);
int32 arm_inst_brpt(cpu_state_t *st, uint32 inst);
int32 arm_inst_blx_1(cpu_state_t *st, uint32 inst);
int32 arm_inst_blx_2(cpu_state_t *st, uint32 inst);
int32 arm_inst_bx(cpu_state_t *st, uint32 inst);
int32 arm_inst_bxj(cpu_state_t *st, uint32 inst);
int32 arm_inst_cdp(cpu_state_t *st, uint32 inst);
int32 arm_inst_clz(cpu_state_t *st, uint32 inst);
int32 arm_inst_cmn(cpu_state_t *st, uint32 inst);
int32 arm_inst_cmp(cpu_state_t *st, uint32 inst);
int32 arm_inst_cps(cpu_state_t *st, uint32 inst);
int32 arm_inst_cpy(cpu_state_t *st, uint32 inst);
int32 arm_inst_eor(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldc(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldm_1(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldm_2(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldm_3(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldr(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldrb(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldrbt(cpu_state_t *st, uint32 inst);

#endif