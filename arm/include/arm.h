#ifndef __ARM_H__
#define __ARM_H__

#include "common/log.h"

//打印指令
#define loginst(name, hex) println("[INST]  name=%s, hex=0x%08x", name, hex)

#include "arm/cpu.h"
#include "arm/mem.h"
#include "arm/arm_inst.h"

#endif