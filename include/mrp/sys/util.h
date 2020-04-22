#ifndef __MRP_SYS_UTIL_H__
#define __MRP_SYS_UTIL_H__

#include "common/type.h"
#include "mrp/vm.h"

uint16 *mr_c2u(char *cp, int *err, int *size);

char *get_filename(vm_info_t *vm, char *outputbuf, char *filename);
FILE *open_file(vm_info_t *vm, char *filename, const char *mode);

#endif