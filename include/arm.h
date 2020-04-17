#ifndef __ARM_H__
#define __ARM_H__

/*
这里说明一下arm架构的版本
*/
#define ARCH_V_4 (0)	  //4
#define ARCH_V_4T5T		  //4T、5T
#define ARCH_V_5TE		  //5TE、 5TExP
#define ARCH_V_5TEJ		  //5TEJ
#define ARCH_V_6		  //6
#define ARCH_VER ARCH_V_4 //这里制定一下

#include "common/bits.h"
#include "common/log.h"
#include "common/type.h"

//打印指令
#define loginst(name, hex) println("[INST]  name=%s, hex=0x%08x", name, hex)

#include "arm/cpu.h"
#include "arm/mem.h"
#include "arm/arm_inst.h"

#endif