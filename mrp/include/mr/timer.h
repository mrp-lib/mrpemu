#ifndef __MR_TIMER_H__
#define __MR_TIMER_H__

#include "common/type.h"

typedef int32 (*T_mr_timerStart)(uint16 t);
typedef int32 (*T_mr_timerStop)(void);

int32 mr_timerStart(uint16 t);

int32 mr_timerStop(void);

#endif