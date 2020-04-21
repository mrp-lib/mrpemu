#ifndef __MRP_H__
#define __MRP_H__

#include "common/bits.h"
#include "common/log.h"
#include "common/type.h"

#define logsysc(fmt, ...) \
	printf("[SYSCAL] ");  \
	println(fmt, ##__VA_ARGS__)

#include "mrp/info.h"
#include "mrp/vm.h"
#include "mrp/mem.h"

#include "helper/mr.h"

#include "mrp/sys/dsm.h"

#include "mrp/mr/cport.h"
#include "mrp/mr/dsm.h"
#include "mrp/mr/mem.h"

#endif