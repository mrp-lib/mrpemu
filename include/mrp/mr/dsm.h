#ifndef __MRP_MR_DSM_H__
#define __MRP_MR_DSM_H__

#include "mrp/common/helper.h"

typedef int32 (*MR_LOAD_C_FUNCTION)(int32 code);
typedef int32 (*mrc_extMainSendAppMsg_t)(int32 app, int32 code, int32 param0, int32 param1, int32 param2, int32 param3, int32 param4, int32 extCode);

typedef struct _mr_c_function_st mr_c_function_st;

typedef struct _mrc_extChunk_st
{
	int32 check; //0x7FD854EB 标志

	// MR_LOAD_C_FUNCTION c_load_func; //mr_c_function_load 函数指针
	vmpt c_load_func;
	// MR_C_FUNCTION event; //mr_helper 函数指针
	vmpt event;

	// uint8 *code_buf; //ext内存地址
	vmpt code_buf;	//ext内存地址
	int32 code_len; //ext长度

	// uint8 *var_buf; //RW段地址
	vmpt var_buf;
	int32 var_len; //RW段长度

	mr_c_function_st *global_p_buf; //mr_c_function_st 表地址
	int32 global_p_len;				//mr_c_function_st 表长度

	int32 timer;

	// mrc_extMainSendAppMsg_t sendAppEvent;
	vmpt sendAppEvent;
	// mr_table *extMrTable; // mr_table函数表。
	vmpt extMrTable;

	int32 isPause; /*1: pause 状态0:正常状态*/
} mrc_ext_chunk_t;

typedef struct _mr_c_function_st
{
	// uint8 *start_of_ER_RW; // RW段指针
	vmpt start_of_ER_RW;
	uint32 ER_RW_Length; // RW长度
	int32 ext_type;		 // ext启动类型，为1时表示ext启动
	// mrc_extChunk_st *mrc_extChunk; // ext模块描述段，下面的结构体。
	vmpt mrc_extChunk;
	int32 stack; // stack shell 2008-2-28
} mr_c_function_t;

void swi_mr_c_function_new(vm_info_t *vm);

void swi_mr_testcom(vm_info_t *vm);

void swi_mr_testcom1(vm_info_t *vm);

#endif