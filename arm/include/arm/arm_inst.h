#ifndef __ARM_ARM_INST_H__
#define __ARM_ARM_INST_H__

#include "common/type.h"
#include "cpu.h"

//检测是不是偶数
#define even(num) (((num)&0x0001) == 0)

//快速匹配条件是否满足
#define cond_ok() check_cond(st, inst)

//对于地址模式4，检测是否含有给定寄存器
#define has_reg_m4(reg) ((inst >> reg) & 0b0001)

//针对于ldm命令来说，用来检测rn是否等于pc
#define ldm_rn_pc_check()                \
	if (((inst >> 16) & 0x000f) == r_pc) \
	return EXEC_UNPREDICTABLE

#define overflow_add(result, num1, num2) (((result) ^ (num1)) & ((result) ^ (num2)) & 0x80000000) //检测结果是否溢出(不考虑是否有C位，因为如果本身溢出了，再加上或减去C位还是溢出)，顺序为：结果，被加数，加数
#define overflow_sub(result, num1, num2) overflow_add(num1, result, num2)						  //检测结果是否溢出(不考虑是否有C位，因为如果本身溢出了，再加上或减去C位还是溢出)，顺序为：结果，被减数，减数
#define carry(result, num1) ((result) < (num1))													  //检测是否进位(都加上一个数了结果却变小？)
#define borrow(result, num1) ((result) > (num1))												  //检测是否借位(都减掉一个数了结果却变大？)

bool check_cond(cpu_state_t *st, uint32 inst);												//条件检测
bool shifter_operand(cpu_state_t *st, uint32 inst, uint32 *operand);						//地址模式1
uint32 addr_mode_2(cpu_state_t *st, uint32 inst);											//地址模式2
uint32 addr_mode_3(cpu_state_t *st, uint32 inst);											//地址模式3
void addr_mode_4(cpu_state_t *st, uint32 inst, uint32 *start_address, uint32 *end_address); //地址模式4
int32 signed_sat(int64 val, int32 min, int32 max, bool *issat);								//有符号饱和处理
uint32 unsigned_sat(int64 val, uint32 max, bool *issat);									//无符号饱和处理

int32 arm_inst_adc(cpu_state_t *st, uint32 inst);
int32 arm_inst_add(cpu_state_t *st, uint32 inst);
int32 arm_inst_and(cpu_state_t *st, uint32 inst);
int32 arm_inst_b_bl(cpu_state_t *st, uint32 inst);
int32 arm_inst_bic(cpu_state_t *st, uint32 inst);
int32 arm_inst_blx_1(cpu_state_t *st, uint32 inst);
int32 arm_inst_blx_2(cpu_state_t *st, uint32 inst);
int32 arm_inst_brpt(cpu_state_t *st, uint32 inst);
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
int32 arm_inst_ldrd(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldrex(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldrh(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldrsb(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldrsh(cpu_state_t *st, uint32 inst);
int32 arm_inst_ldrt(cpu_state_t *st, uint32 inst);
int32 arm_inst_mcr(cpu_state_t *st, uint32 inst);
int32 arm_inst_mcrr(cpu_state_t *st, uint32 inst);
int32 arm_inst_mla(cpu_state_t *st, uint32 inst);
int32 arm_inst_mov(cpu_state_t *st, uint32 inst);
int32 arm_inst_mrc(cpu_state_t *st, uint32 inst);
int32 arm_inst_mrrc(cpu_state_t *st, uint32 inst);
int32 arm_inst_mrs(cpu_state_t *st, uint32 inst);
int32 arm_inst_msr(cpu_state_t *st, uint32 inst);
int32 arm_inst_msr(cpu_state_t *st, uint32 inst);
int32 arm_inst_mul(cpu_state_t *st, uint32 inst);
int32 arm_inst_mvn(cpu_state_t *st, uint32 inst);
int32 arm_inst_orr(cpu_state_t *st, uint32 inst);
int32 arm_inst_pkhbt(cpu_state_t *st, uint32 inst);
int32 arm_inst_pkhtb(cpu_state_t *st, uint32 inst);
int32 arm_inst_pld(cpu_state_t *st, uint32 inst);
int32 arm_inst_qadd(cpu_state_t *st, uint32 inst);
int32 arm_inst_qadd16(cpu_state_t *st, uint32 inst);
int32 arm_inst_qadd8(cpu_state_t *st, uint32 inst);
int32 arm_inst_qaddsubx(cpu_state_t *st, uint32 inst);
int32 arm_inst_qdadd(cpu_state_t *st, uint32 inst);
int32 arm_inst_qdsub(cpu_state_t *st, uint32 inst);
int32 arm_inst_qsub(cpu_state_t *st, uint32 inst);
int32 arm_inst_qsub16(cpu_state_t *st, uint32 inst);
int32 arm_inst_qsub8(cpu_state_t *st, uint32 inst);
int32 arm_inst_qsubaddx(cpu_state_t *st, uint32 inst);
int32 arm_inst_rev(cpu_state_t *st, uint32 inst);
int32 arm_inst_rev16(cpu_state_t *st, uint32 inst);
int32 arm_inst_revsh(cpu_state_t *st, uint32 inst);
int32 arm_inst_rfe(cpu_state_t *st, uint32 inst);
int32 arm_inst_rsb(cpu_state_t *st, uint32 inst);
int32 arm_inst_rsc(cpu_state_t *st, uint32 inst);
int32 arm_inst_sadd16(cpu_state_t *st, uint32 inst);
int32 arm_inst_sadd8(cpu_state_t *st, uint32 inst);
int32 arm_inst_saddsubx(cpu_state_t *st, uint32 inst);
int32 arm_inst_sbc(cpu_state_t *st, uint32 inst);
int32 arm_inst_sel(cpu_state_t *st, uint32 inst);
int32 arm_inst_setend(cpu_state_t *st, uint32 inst);
int32 arm_inst_shadd16(cpu_state_t *st, uint32 inst);
int32 arm_inst_shadd8(cpu_state_t *st, uint32 inst);
int32 arm_inst_shaddsubx(cpu_state_t *st, uint32 inst);
int32 arm_inst_shsub16(cpu_state_t *st, uint32 inst);
int32 arm_inst_shsub8(cpu_state_t *st, uint32 inst);
int32 arm_inst_shsubaddx(cpu_state_t *st, uint32 inst);
int32 arm_inst_smla_x_y(cpu_state_t *st, uint32 inst);
int32 arm_inst_smlad(cpu_state_t *st, uint32 inst);
int32 arm_inst_smlal(cpu_state_t *st, uint32 inst);
int32 arm_inst_smlal_x_y(cpu_state_t *st, uint32 inst);
int32 arm_inst_smlald(cpu_state_t *st, uint32 inst);
int32 arm_inst_smlaw(cpu_state_t *st, uint32 inst);
int32 arm_inst_smlsd(cpu_state_t *st, uint32 inst);
int32 arm_inst_smlsld(cpu_state_t *st, uint32 inst);
int32 arm_inst_smmla(cpu_state_t *st, uint32 inst);
int32 arm_inst_smmls(cpu_state_t *st, uint32 inst);
int32 arm_inst_smmul(cpu_state_t *st, uint32 inst);
int32 arm_inst_smuad(cpu_state_t *st, uint32 inst);
int32 arm_inst_smul_x_y(cpu_state_t *st, uint32 inst);
int32 arm_inst_smull(cpu_state_t *st, uint32 inst);
int32 arm_inst_smulw_y(cpu_state_t *st, uint32 inst);
int32 arm_inst_smusd(cpu_state_t *st, uint32 inst);
int32 arm_inst_srs(cpu_state_t *st, uint32 inst);
int32 arm_inst_ssat(cpu_state_t *st, uint32 inst);
int32 arm_inst_ssat16(cpu_state_t *st, uint32 inst);
int32 arm_inst_ssub16(cpu_state_t *st, uint32 inst);
int32 arm_inst_ssub8(cpu_state_t *st, uint32 inst);
int32 arm_inst_ssubaddx(cpu_state_t *st, uint32 inst);
int32 arm_inst_stc(cpu_state_t *st, uint32 inst);
int32 arm_inst_stm_1(cpu_state_t *st, uint32 inst);
int32 arm_inst_stm_2(cpu_state_t *st, uint32 inst);
int32 arm_inst_str(cpu_state_t *st, uint32 inst);
int32 arm_inst_strb(cpu_state_t *st, uint32 inst);
int32 arm_inst_strbt(cpu_state_t *st, uint32 inst);
int32 arm_inst_strd(cpu_state_t *st, uint32 inst);
int32 arm_inst_strex(cpu_state_t *st, uint32 inst);
int32 arm_inst_strh(cpu_state_t *st, uint32 inst);
int32 arm_inst_strt(cpu_state_t *st, uint32 inst);
int32 arm_inst_sub(cpu_state_t *st, uint32 inst);
int32 arm_inst_swi(cpu_state_t *st, uint32 inst);
int32 arm_inst_swp(cpu_state_t *st, uint32 inst);
int32 arm_inst_swpb(cpu_state_t *st, uint32 inst);
int32 arm_inst_sxtab(cpu_state_t *st, uint32 inst);
int32 arm_inst_sxtab16(cpu_state_t *st, uint32 inst);
int32 arm_inst_sxtah(cpu_state_t *st, uint32 inst);
int32 arm_inst_sxtb(cpu_state_t *st, uint32 inst);
int32 arm_inst_sxtb16(cpu_state_t *st, uint32 inst);
int32 arm_inst_sxth(cpu_state_t *st, uint32 inst);
int32 arm_inst_teq(cpu_state_t *st, uint32 inst);
int32 arm_inst_tst(cpu_state_t *st, uint32 inst);
int32 arm_inst_uadd16(cpu_state_t *st, uint32 inst);
int32 arm_inst_uadd8(cpu_state_t *st, uint32 inst);
int32 arm_inst_uaddsubx(cpu_state_t *st, uint32 inst);
int32 arm_inst_uhadd16(cpu_state_t *st, uint32 inst);
int32 arm_inst_uhadd8(cpu_state_t *st, uint32 inst);
int32 arm_inst_uhaddsubx(cpu_state_t *st, uint32 inst);
int32 arm_inst_uhsub16(cpu_state_t *st, uint32 inst);
int32 arm_inst_uhsub8(cpu_state_t *st, uint32 inst);
int32 arm_inst_uhsubaddx(cpu_state_t *st, uint32 inst);
int32 arm_inst_umaal(cpu_state_t *st, uint32 inst);
int32 arm_inst_umlal(cpu_state_t *st, uint32 inst);
int32 arm_inst_umull(cpu_state_t *st, uint32 inst);
int32 arm_inst_uqadd16(cpu_state_t *st, uint32 inst);
int32 arm_inst_uqadd8(cpu_state_t *st, uint32 inst);
int32 arm_inst_uqaddsubx(cpu_state_t *st, uint32 inst);
int32 arm_inst_uqsub16(cpu_state_t *st, uint32 inst);
int32 arm_inst_uqsub8(cpu_state_t *st, uint32 inst);
int32 arm_inst_uqsubaddx(cpu_state_t *st, uint32 inst);
int32 arm_inst_usad8(cpu_state_t *st, uint32 inst);
int32 arm_inst_usada8(cpu_state_t *st, uint32 inst);
int32 arm_inst_usat(cpu_state_t *st, uint32 inst);
int32 arm_inst_usat16(cpu_state_t *st, uint32 inst);
int32 arm_inst_usub16(cpu_state_t *st, uint32 inst);
int32 arm_inst_usub8(cpu_state_t *st, uint32 inst);
int32 arm_inst_usubaddx(cpu_state_t *st, uint32 inst);
int32 arm_inst_uxtab(cpu_state_t *st, uint32 inst);
int32 arm_inst_uxtab16(cpu_state_t *st, uint32 inst);
int32 arm_inst_uxtah(cpu_state_t *st, uint32 inst);
int32 arm_inst_uxtb(cpu_state_t *st, uint32 inst);
int32 arm_inst_uxtb16(cpu_state_t *st, uint32 inst);
int32 arm_inst_uxth(cpu_state_t *st, uint32 inst);

#endif