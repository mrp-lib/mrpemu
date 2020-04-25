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
#include "mrp/swi.h"

#include "helper/mr.h"

#include "mrp/sys/dsm.h"
#include "mrp/sys/font.h"
#include "mrp/sys/util.h"

#include "mrp/mr/cport.h"
#include "mrp/mr/device.h"
#include "mrp/mr/draw.h"
#include "mrp/mr/dsm.h"
#include "mrp/mr/fs.h"
#include "mrp/mr/mem.h"
#include "mrp/mr/util.h"

#endif