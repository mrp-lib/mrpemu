#ifndef __MRP_MR_FS_H__
#define __MRP_MR_FS_H__

#include "common/type.h"
#include "mrp/vm.h"

#define MR_FILE_RDONLY 1	//以只读的方式打开文件。
#define MR_FILE_WRONLY 2	//以只写的方式打开文件。
#define MR_FILE_RDWR 4		//以读写的方式打开文件。
#define MR_FILE_CREATE 8	//如果文件不存在，创建该文件。
#define MR_FILE_RECREATE 16 //无论文件存不存在，都重新创建该文件。

#define MR_IS_FILE 1	//文件
#define MR_IS_DIR 2		//目录
#define MR_IS_INVALID 8 //无效(非文件、非目录)

void swi_mr_ferrno(vm_info_t *vm);
void swi_mr_open(vm_info_t *vm);
void swi_mr_close(vm_info_t *vm);
void swi_mr_info(vm_info_t *vm);
void swi_mr_write(vm_info_t *vm);
void swi_mr_read(vm_info_t *vm);
void swi_mr_seek(vm_info_t *vm);
void swi_mr_getLen(vm_info_t *vm);
void swi_mr_remove(vm_info_t *vm);
void swi_mr_rename(vm_info_t *vm);
void swi_mr_mkDir(vm_info_t *vm);
void swi_mr_rmDir(vm_info_t *vm);
void swi_mr_findStart(vm_info_t *vm);
void swi_mr_findGetNext(vm_info_t *vm);
void swi_mr_findStop(vm_info_t *vm);

#endif