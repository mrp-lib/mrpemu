#ifndef __MR_DSM_H__
#define __MR_DSM_H__

#include "common/type.h"
#include "mr/helper.h"

typedef struct _mr_c_event_st
{
	int32 code;
	int32 param0;
	int32 param1;
	int32 param2;
	int32 param3;
} mr_c_event_st;

typedef int32 (*MR_EVENT_FUNCTION)(int16 type, int32 param1, int32 param2);
typedef int32 (*MR_TIMER_FUNCTION)(void);
typedef int32 (*MR_STOP_FUNCTION)(void);
typedef int32 (*MR_PAUSEAPP_FUNCTION)(void);
typedef int32 (*MR_RESUMEAPP_FUNCTION)(void);

typedef int32 (*MR_LOAD_C_FUNCTION)(int32 code);
typedef int32 (*MR_C_FUNCTION)(void *P, int32 code, uint8 *input, int32 input_len, uint8 **output, int32 *output_len);
typedef int32 (*mrc_extMainSendAppMsg_t)(int32 extCode, int32 app, int32 code, int32 param0, int32 param1);

typedef struct _mrc_extChunk_st mrc_extChunk_st;

typedef struct _mr_c_function_st
{
	uint8 *start_of_ER_RW;		   // RW段指针
	uint32 ER_RW_Length;		   // RW长度
	int32 ext_type;				   // ext启动类型，为1时表示ext启动
	mrc_extChunk_st *mrc_extChunk; // ext模块描述段，下面的结构体。
	int32 stack;				   // stack shell 2008-2-28
} mr_c_function_st;

typedef struct _mrc_extChunk_st
{
	int32 check; //0x7FD854EB 标志

	MR_LOAD_C_FUNCTION c_load_func; //mr_c_function_load 函数指针
	MR_C_FUNCTION event;			//mr_helper 函数指针

	uint8 *code_buf; //ext内存地址
	int32 code_len;	 //ext长度

	uint8 *var_buf; //RW段地址
	int32 var_len;	//RW段长度

	mr_c_function_st *global_p_buf; //mr_c_function_st 表地址
	int32 global_p_len;				//mr_c_function_st 表长度

	int32 timer;

	mrc_extMainSendAppMsg_t sendAppEvent;
	mr_table *extMrTable; // mr_table函数表。

	int32 isPause; /*1: pause 状态0:正常状态*/
} mrc_extChunk_st;

extern char pack_filename[128];
extern char start_filename[128];
extern char old_pack_filename[128];
extern char old_start_filename[128];
extern char start_fileparameter[128];

extern MR_EVENT_FUNCTION mr_event_function;
extern MR_TIMER_FUNCTION mr_timer_function;
extern MR_STOP_FUNCTION mr_stop_function;
extern MR_PAUSEAPP_FUNCTION mr_pauseApp_function;
extern MR_RESUMEAPP_FUNCTION mr_resumeApp_function;

extern MR_LOAD_C_FUNCTION mr_load_c_function;
extern MR_C_FUNCTION mr_c_function;
extern mr_c_function_st *mr_c_function_P;
extern uint32 mr_c_function_P_len;
extern void *mr_c_function_fix_p;

extern char *mr_ram_file;
extern int32 mr_ram_file_len;
extern uint8 *mr_m0_files[50];

extern uint32 vm_state;
extern int32 mr_state;
extern int32 bi;

extern char *mr_exception_str;

/** 启动dsm */
int32 mr_start_dsm(const char *entry);

int32 mr_doExt(const char *filename);

int32 _mr_TestCom(int32 L, int input0, int input1);

int32 _mr_TestComC(int32 type, uint8 *input, int32 len, int32 code);

int32 _mr_TestCom1(int32 L, int input0, char *input1, int32 len);

int32 _mr_c_function_new(MR_C_FUNCTION f, int32 len);

int32 mr_stop(void);
int32 mr_stop_ex(int16 freemem);
int32 mr_registerAPP(uint8 *p, int32 len, int32 index);
int32 mr_event(int16 type, int32 param1, int32 param2);
int32 mr_resumeApp(void);
int32 mr_pauseApp(void);

#endif