#ifndef __MRP_MR_CPORT_H__
#define __MRP_MR_CPORT_H__

#include "mrp/common/helper.h"

void swi_mr_memcpy(vm_info_t *vm);
void swi_mr_memmove(vm_info_t *vm);
void swi_mr_memset(vm_info_t *vm);
void swi_mr_memcmp(vm_info_t *vm);
void swi_mr_memchr(vm_info_t *vm);
void swi_mr_strcpy(vm_info_t *vm);
void swi_mr_strncpy(vm_info_t *vm);
void swi_mr_strcat(vm_info_t *vm);
void swi_mr_strncat(vm_info_t *vm);
void swi_mr_strcmp(vm_info_t *vm);
void swi_mr_strncmp(vm_info_t *vm);
void swi_mr_strcoll(vm_info_t *vm);
void swi_mr_strlen(vm_info_t *vm);
void swi_mr_strstr(vm_info_t *vm);
void swi_mr_atoi(vm_info_t *vm);
void swi_mr_strtoul(vm_info_t *vm);
void swi_mr_rand(vm_info_t *vm);
void swi_mr_sprintf(vm_info_t *vm);
void swi_mr_printf(vm_info_t *vm);
void swi_mr_wstrlen(vm_info_t *vm);

#endif