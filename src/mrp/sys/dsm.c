#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "mrp.h"
#include "utils/gzip.h"

#define START_FILE_NAME "cfunction.ext"
#define START_EXT "cfunction.ext"

#define MR_READ_MAX_LEN (1024 * 400) //读取mrp时分配的最大内存空间，额，800k足够用了吧

uint32 read_mrp_file(vm_info_t *vm, char *filename, uint8 *buffer)
{
	uint32 buflen = 0;
	//如果是固化应用
	if (mrst.pack_filename[0] == '*' || mrst.pack_filename[0] == '$')
	{
	}
	//否则从mrp文件读取
	else
	{
		//打开mrp文件
		mrp_reader_t *reader = mrp_open(mrst.pack_filename);
		if (reader == null)
			return 0;

		//看看是否存在给定文件
		mrp_file_info *file = mrp_file(reader, filename);
		if (file == null)
		{
			mrp_close(reader);
			return 0;
		}

		//读取文件
		if (0 != mrp_read(reader, filename, buffer))
		{
			mrp_close(reader);
			return 0;
		}

		//读取完成
		buflen = file->size;
		mrst.appInfo.id = reader->info.id;
		mrst.appInfo.ver = reader->info.ver;
		mrst.appInfo.sidName = 0;

		//关闭文件
		mrp_close(reader);
	}

	//gzip解压
	if (buffer[0] == 0x1f && buffer[1] == 0x8b)
	{
		uint32 zlen;
		uint8 *dst = ungzip(buffer, buflen, null, &zlen);
		if (dst == null)
			return 0;
		//拷贝过去
		memcpy(buffer, dst, zlen);
		buflen = zlen;
	}

	return buflen;
}

int32 do_ext(vm_info_t *vm, char *filename)
{
	//读取ext文件
	uint8 *ext_data = malloc(MR_READ_MAX_LEN);
	uint32 len = read_mrp_file(vm, filename, ext_data);
	if (len)
	{
		//读取之后装入到虚拟机内存
		vmpt addr = mem_malloc(vm, len);			   //分配虚拟机内存
		memcpy(vmpt_real(uint8, addr), ext_data, len); //装入
		free(ext_data);								   //释放先前分配的内存

		//调用相关的函数启动虚拟机
		mr_testCom(vm, 0, 0xe2d, 0xb61);
		if (mr_testComC(vm, 800, addr, len, 0)) //用于调用mr_c_function_load
		{
			mem_free(vm, addr, len);
			return -1;
		}
		else
		{
			uint32 ret = mr_testComC(vm, 801, addr, 0x7cd, 6);
			ret = mr_testComC(vm, 801, vm_mem_offset(&mrst.appInfo), 16, 8);
			ret = mr_testComC(vm, 801, addr, 0x7cd, 0);
			return ret;
		}
	}
	else
	{
		free(ext_data);
		return -1;
	}
}

int32 intra_start(vm_info_t *vm, char *filename, char *entry)
{
	//TODO 这个函数还有一些状态没有处理
	if (entry == null)
		entry = "_dsm";

	memset(mrst.entry, 0, MR_FILE_MAX_LEN);
	strncpy(mrst.entry, entry, MR_FILE_MAX_LEN - 1);
	mrst.mr_state = 1;

	int32 ret = do_ext(vm, filename);
	if (ret != 0)
		ret = do_ext(vm, START_EXT);

	if (ret != 0)
	{
		mrst.mr_state = 5;
		return MR_FAILED;
	}

	return MR_SUCCESS;
}

int32 start_dsm(vm_info_t *vm, char *entry)
{
	memset(mrst.pack_filename, 0, MR_FILE_MAX_LEN);
	if (NULL != entry && *entry == '*')
		strcpy(mrst.pack_filename, entry);
	else if (NULL != entry && *entry == '%')
		strcpy(mrst.pack_filename, entry + 1);
	else if (NULL != entry && *entry == '#' && entry[1] == '<')
		strcpy(mrst.pack_filename, entry + 2);
	else
		strcpy(mrst.pack_filename, "*A");

	memset(mrst.old_pack_filename, 0, MR_FILE_MAX_LEN);
	memset(mrst.old_start_filename, 0, MR_FILE_MAX_LEN);
	memset(mrst.start_file_parameter, 0, MR_FILE_MAX_LEN);

	logi("mr_start_dsm(vm=%p, entry=%s, pack_filename=%s)", vm, entry, mrst.pack_filename);

	return intra_start(vm, START_FILE_NAME, entry);
}

int32 start_dsmB(vm_info_t *vm, char *entry)
{
	memset(mrst.pack_filename, 0, MR_FILE_MAX_LEN);
	if (null != entry && *entry == '*')
	{
		strcpy(mrst.pack_filename, entry);
	}
	else if (null != entry && *entry == '%')
	{
		char *p = strchr(entry, ',');

		if (NULL != p)
		{
			*p = 0;
			strcpy(mrst.pack_filename, entry + 1);
			*p = ',';
		}
		else
		{
			strcpy(mrst.pack_filename, entry + 1);
		}
	}
	else if (null != entry && *entry == '#' && entry[1] == '<')
	{
		strcpy(mrst.pack_filename, entry + 2);
	}
	else
	{
		strcpy(mrst.pack_filename, "*A");
	}

	memset(mrst.old_pack_filename, 0, MR_FILE_MAX_LEN);
	memset(mrst.old_start_filename, 0, MR_FILE_MAX_LEN);
	memset(mrst.start_file_parameter, 0, MR_FILE_MAX_LEN);

	logi("start_dsmB(vm=%p, entry=%s, pack_filename=%s)", vm, entry, mrst.pack_filename);

	return intra_start(vm, START_FILE_NAME, entry);
}

int32 start_dsmC(vm_info_t *vm, char *entry)
{
	memset(mrst.pack_filename, 0, MR_FILE_MAX_LEN);
	if (null != entry && *entry == '*')
		strcpy(mrst.pack_filename, entry);
	else if (null != entry && *entry == '%')
		strcpy(mrst.pack_filename, entry + 1);
	else if (null != entry && *entry == '#' && entry[1] == '<')
		strcpy(mrst.pack_filename, entry + 2);
	else
		strcpy(mrst.pack_filename, "*A");

	memset(mrst.old_pack_filename, 0, MR_FILE_MAX_LEN);
	memset(mrst.old_start_filename, 0, MR_FILE_MAX_LEN);
	memset(mrst.start_file_parameter, 0, MR_FILE_MAX_LEN);

	logi("start_dsmC(vm=%p, entry=%s, pack_filename=%s)", vm, entry, mrst.pack_filename);

	return intra_start(vm, START_FILE_NAME, entry);
}

int32 start_dsm_ex(vm_info_t *vm, char *path, char *entry)
{

	memset(mrst.pack_filename, 0, MR_FILE_MAX_LEN);
	if (null != path && *path == '*')
		strcpy(mrst.pack_filename, path);
	else if (null != path && *path == '%')
		strcpy(mrst.pack_filename, path + 1);
	else if (null != path && *path == '#' && path[1] == '<')
		strcpy(mrst.pack_filename, path + 2);
	else
		strcpy(mrst.pack_filename, "*A");

	memset(mrst.old_pack_filename, 0, MR_FILE_MAX_LEN);
	memset(mrst.old_start_filename, 0, MR_FILE_MAX_LEN);
	memset(mrst.start_file_parameter, 0, MR_FILE_MAX_LEN);

	logi("start_dsm_ex(vm=%p, path=%s, entry=%s, pack_filename=%s)", vm, path, entry, mrst.pack_filename);
	return intra_start(vm, START_FILE_NAME, entry);
}

int32 mr_testCom(vm_info_t *vm, int32 L, int input0, int input1)
{
	logi("mr_testCom(vm=%p, L=%d, input0=%d, input1=%d)", vm, L, input0, input1);

	uint32 ret = MR_SUCCESS;
	switch (input0)
	{
	case 0x01:
		ret = mr_getTime(vm);
		break;
	case 0x02:
		mrst.mr_event_function = (vmpt)input1;
		break;
	case 0x03:
		mrst.mr_timer_function = (vmpt)input1;
		break;
	case 0x04:
		mrst.mr_stop_function = (vmpt)input1;
		break;
	case 0x05:
		mrst.mr_pauseApp_function = (vmpt)input1;
		break;
	case 0x06:
		mrst.mr_resumeApp_function = (vmpt)input1;
		break;
	case 0x07:
		ret = input1;
		break;
	case 0x64:
		ret = mrst.mem_state.min;
		break;
	case 0x65:
		ret = mrst.mem_state.top;
		break;
	case 0x66:
		ret = mrst.mem_state.left;
		break;
	case 0xc8:
		ret = mr_startShake(vm, input1);
		break;
	case 0x12c:
		mrst.mr_soundOn = (int8)input1;
		break;
	case 0x12d:
		mrst.mr_shakeOn = (int8)input1;
	case 0x12e:
		mrst.bi |= 0x4;
		break;
	case 0x12f:
		mrst.bi &= ~0x4;
		break;
	case 0x130:
		mrst.bi |= 0x8;
		break;
	case 0x131:
		mrst.bi &= ~0x8;
		break;
	case 0x132:
		mrst.mr_sms_return_flag = 1;
		mrst.mr_sms_return_val = input1;
		break;
	case 0x133:
		mrst.mr_sms_return_flag = 0;
		break;
	case 0x190:
		mr_sleep(vm, input1);
		break;
	case 0x191:
		ret = mrst.sysinfo.screen_width;
		mrst.sysinfo.screen_width = input1;
		break;
	case 0x194:
		// _mr_newSIMInd((int16)input1, NULL);
		break;
	case 0x195:
		// ret = mr_closeNetwork();
		break;
	case 0x196:
		ret = mrst.sysinfo.screen_height;
		mrst.sysinfo.screen_height = input1;
		break;
	case 0x197:
		// mr_timer_run_without_pause = (void *)input1;
		// mr_plat(1202, input1);
		break;
	case 0x198:
		//TODO
		break;
	case 0x1f4:
		// ret = _mr_load_sms_cfg();
		break;
	case 0x1f7:
		// {
		// 	uint8 dst;
		// 	_mr_smsGetBytes(5, &dst, 1);
		// 	ret = dst;
		// }
		break;
	case 0x1f8:
		// ret = _mr_save_sms_cfg(input1);
		break;

	case 0xcb3:
		if (0x9E67A == input1)
			mrst.bi &= ~0x2;
		break;
	case 0xe2d:
		if (0xb61 == input1)
			mrst.bi |= 0x1;
		break;
	case 0xf51:
		if (0x18030 == input1)
			mrst.bi |= 0x2;
		break;
	default:
		logw("mr_testCom got unkown param: code=%d", input0);
		ret = MR_IGNORE;
	}
	return ret;
}

int32 mr_TestCom1(vm_info_t *vm, int32 L, int input0, char *input1, int32 len)
{
	return MR_IGNORE;
}

int32 mr_testComC(vm_info_t *vm, int32 type, vmpt input, int32 len, int32 code)
{
	logi("testComC(mv=%p, type=%d, input=0x%08x, len=%d, code=%d)", vm, type, input, len, code);
	//调用mr_load_c_function函数
	if (type == 800)
	{
		*vmpt_real(uint32, input) = (uint32)vm_mem_offset(vm->mem->mr_func_tab); //设置函数表
		mrst.mr_c_function_load = input + 8;									 //保存入口函数
		//启动程序
		return vm_run(vm, mrst.mr_c_function_load);
	}
	//调用mr_c_function函数，这个函数在mr_c_function_new中被设置
	else
	{
		vmreg(0) = mrst.mr_c_function_P; //参数1： mr_c_function_P
		vmreg(1) = code;				 //参数2： code
		vmreg(2) = input;				 //参数3： input
		vmreg(3) = len;					 //参数4： len
		return vm_run(vm, mrst.mr_c_function);
	}
}

uint32 mr_getTime(vm_info_t *vm)
{
	struct timeval t;
	int ret = gettimeofday(&t, NULL);

	if (t.tv_usec < 0)
	{
		t.tv_sec--;
		t.tv_usec += 1000000;
	}

	return (ret == 0) ? ((t.tv_sec - vm->startTime.tv_sec) * 1000 + (t.tv_usec - vm->startTime.tv_usec) / 1000) : 0;
}

//
int32 mr_startShake(vm_info_t *vm, int32 ms)
{
	//需要检测是否已经在震动
	//TODO手机震动
	return MR_SUCCESS;
}

int32 mr_sleep(vm_info_t *vm, uint32 ms)
{
	//TODO这里应该让虚拟机睡眠，而不是系统睡眠
	usleep(ms);
	return MR_SUCCESS;
}

int32 mr_plat(vm_info_t *vm, int32 code, int32 param)
{
	return MR_IGNORE;
}

int32 mr_platEx(vm_info_t *vm, int32 code, vmpt input, int32 input_len, vmpt output, vmpt output_len, vmpt cb)
{
	return MR_IGNORE;
}