#ifndef __MRP_SYS_DSM_H__
#define __MRP_SYS_DSM_H__

#include "common/type.h"
#include "mrp/vm.h"

/*
对于入口标志：
	该标志用于启动非dsm根目录的项目。
	启动dsm根目录时该参数填NULL（相当于运行”*A”固化应用）。
	若该参数以“*”开头，则执行固化应用。
		“*A”表示index为0的应用，
		”*B”表示index为1的应用，以此类推。
	若该参数以“%”开头，则执行根目录下的mrp文件，如“%QQ.mrp”执行QQ.mrp文件。
*/

/*
读取mrp文件
参数：
	vm			虚拟机
	filename	内部文件名
	len			读取到的长度
	lookfor		仅仅看看有没有文件
返回：
	null		失败
	!null		文件内容，这个指针需要free
*/
uint8 *read_mrp_file(vm_info_t *vm, char *filename, uint32 *len, bool lookfor);

/*
通过入口参数来启动mrp
参数：
	vm		虚拟机
	entry	入口标志
返回：
	MR_SUCCESS	成功启动
	MR_FAILED	失败
*/
int32 mr_start_dsm(vm_info_t *vm, char *entry);

/*
直接启动mrp文件，并且可以传参过去
参数：
	vm		虚拟机
	entry	入口标志
返回：
	MR_SUCCESS	成功启动
	MR_FAILED	失败
*/
int32 mr_start_dsmB(vm_info_t *vm, char *entry);

/*
通过入口参数来启动虚拟机
参数：
	vm		虚拟机
	entry	入口标志
返回：
	MR_SUCCESS	成功启动
	MR_FAILED	失败
*/
int32 mr_start_dsmC(vm_info_t *vm, char *entry);

/*
通过入口参数来启动虚拟机
参数：
	vm		虚拟机
	path	mrp文件路径
	entry	入口标志
返回：
	MR_SUCCESS	成功启动
	MR_FAILED	失败
*/
int32 mr_start_dsm_ex(vm_info_t *vm, char *path, char *entry);

int32 mr_testCom(vm_info_t *vm, int32 L, int input0, int input1);
int32 mr_TestCom1(vm_info_t *vm, int32 L, int input0, char *input1, int32 len);
int32 mr_testComC(vm_info_t *vm, int32 type, vmpt input, int32 len, int32 code);

/*取得毫秒时间*/
uint32 mr_getTime(vm_info_t *vm);

/*震动多少毫秒*/
int32 mr_startShake(vm_info_t *vm, int32 ms);

/*睡眠*/
int32 mr_sleep(vm_info_t *vm, uint32 ms);

/*
基本的平台调用接口
参数：
	vm		虚拟机
	code	扩展接口事件
	param	事件参数
*/
int32 mr_plat(vm_info_t *vm, int32 code, int32 param);

// int32 mr_platEx(int32 code, uint8 *input, int32 input_len, uint8 **output, int32 *output_len, MR_PLAT_EX_CB *cb);
/*
扩展的平台调用接口
参数：
	vm			虚拟机
	code		扩展接口事件
	input		输入参数的起始地址
	input_len	输入参数大小（字节数）
	output		输出参数的起始地址
	output_len	输出参数大小（字节数）
	cb			回调函数，由移植层返回给应用，如果应用成发现这个返回值为非空，那么需要在使用完返回数据后调用这个cb函数来释放移植层分配的资源
*/
int32 mr_platEx(vm_info_t *vm, int32 code, vmpt input, int32 input_len, vmpt output, vmpt output_len, vmpt cb);

#endif