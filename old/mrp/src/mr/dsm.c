#include <stdio.h>
#include <string.h>

#include "mrp.h"

#define BITMAPMAX 30
#define SPRITEMAX 10
#define TILEMAX 3
#define SOUNDMAX 5

#define MYTHROAD_VERSION 2012

char pack_filename[128];
char start_filename[128];
char old_pack_filename[128];
char old_start_filename[128];
char start_fileparameter[128];

MR_EVENT_FUNCTION mr_event_function = null;
MR_TIMER_FUNCTION mr_timer_function = null;
MR_STOP_FUNCTION mr_stop_function = null;
MR_PAUSEAPP_FUNCTION mr_pauseApp_function = null;
MR_RESUMEAPP_FUNCTION mr_resumeApp_function = null;

MR_LOAD_C_FUNCTION mr_load_c_function;
MR_C_FUNCTION mr_c_function;
mr_c_function_st *mr_c_function_P;
uint32 mr_c_function_P_len;
void *mr_c_function_fix_p;

char *mr_ram_file;
int32 mr_ram_file_len;
uint8 *mr_m0_files[50];

mr_bitmapSt mr_bitmap[BITMAPMAX + 1];
mr_tileSt mr_tile[TILEMAX];
mr_soundSt mr_sound[SOUNDMAX];
mr_spriteSt mr_sprite[SPRITEMAX];
int16 *mr_map[3];

uint32 vm_state;
int32 mr_state;
int32 bi;

void *mr_timer_p;
uint32 mr_timer_state;
void *mr_timer_run_without_pause;

char mr_entry[128];
char *mr_exception_str;

mrc_timerCB mr_exit_cb; //1951
int32 mr_exit_cb_data;	//1951

mr_c_event_st c_event_st;

int32 _mr_intra_start(const char *filename, const char *entry);

mr_c_port_table _mr_c_port_table = {
	NULL,
	NULL,
	NULL,
	NULL //mr_c_gcfunction
};

mr_internal_table _mr_c_internal_table = {
	mr_m0_files,
	&vm_state,
	&mr_state,
	&bi,

	&mr_timer_p,
	&mr_timer_state,
	&mr_timer_run_without_pause,

	&mr_gzInBuf,
	&mr_gzOutBuf,
	&LG_gzinptr,
	&LG_gzoutcnt,

	&mr_sms_cfg_need_save,
	_mr_smsSetBytes,
	_mr_smsAddNum,
	_mr_newSIMInd,

	_mr_isMr,

	// 以下省略部分只有在支持mr解释器的版本使用
};

const mr_table _mr_c_function_table = {
	(void *)mr_malloc,
	(void *)mr_free,
	(void *)mr_realloc,

	(void *)mr_memcpy,
	(void *)mr_memmove,
	(void *)mr_strcpy,
	(void *)mr_strncpy,
	(void *)mr_strcat,
	(void *)mr_strncat,
	(void *)mr_memcmp,
	(void *)mr_strcmp,
	(void *)mr_strncmp,
	(void *)mr_strcoll,
	(void *)mr_memchr,
	(void *)mr_memset,
	(void *)mr_strlen,
	(void *)mr_strstr,
	(void *)mr_sprintf,
	(void *)mr_atoi,
	(void *)mr_strtoul,
	(void *)mr_rand,

	null, // reserve
	(void *)mr_stop_ex,

	(void *)&_mr_c_internal_table,
	(void *)&_mr_c_port_table,
	(void *)_mr_c_function_new,

	(void *)mr_printf,
	(void *)mr_mem_get,
	(void *)mr_mem_free,
	(void *)mr_drawBitmap,
	(void *)mr_getCharBitmap,
	(void *)mr_timerStart,
	(void *)mr_timerStop,
	(void *)mr_getTime,
	(void *)mr_getDatetime,
	(void *)mr_getUserInfo,
	(void *)mr_sleep,

	(void *)mr_plat,
	(void *)mr_platEx,

	//file io
	(void *)mr_ferrno,
	(void *)mr_open,
	(void *)mr_close,
	(void *)mr_info,
	(void *)mr_write,
	(void *)mr_read,
	(void *)mr_seek,
	(void *)mr_getLen,
	(void *)mr_remove,
	(void *)mr_rename,
	(void *)mr_mkDir,
	(void *)mr_rmDir,
	(void *)mr_findStart,
	(void *)mr_findGetNext,
	(void *)mr_findStop,

	null, //mr_exit,
	(void *)mr_startShake,
	(void *)mr_stopShake,
	(void *)mr_playSound,
	(void *)mr_stopSound,

	(void *)mr_sendSms,
	(void *)mr_call,
	(void *)mr_getNetworkID,
	(void *)mr_connectWAP,

	(void *)mr_menuCreate,
	(void *)mr_menuSetItem,
	(void *)mr_menuShow,
	null, // reserve
	(void *)mr_menuRelease,
	(void *)mr_menuRefresh,

	(void *)mr_dialogCreate,
	(void *)mr_dialogRelease,
	(void *)mr_dialogRefresh,

	(void *)mr_textCreate,
	(void *)mr_textRelease,
	(void *)mr_textRefresh,

	(void *)mr_editCreate,
	(void *)mr_editRelease,
	(void *)mr_editGetText,

	(void *)mr_winCreate,
	(void *)mr_winRelease,

	(void *)mr_getScreenInfo,

	(void *)mr_initNetwork,
	(void *)mr_closeNetwork,
	(void *)mr_getHostByName,
	(void *)mr_socket,
	(void *)mr_connect,
	(void *)mr_closeSocket,
	(void *)mr_recv,
	(void *)mr_recvfrom,
	(void *)mr_send,
	(void *)mr_sendto,

	(void *)&mr_screenBuf,
	(void *)&screen.width,
	(void *)&screen.height,
	(void *)&screen.bit,

	// 资源
	(void *)mr_bitmap,
	(void *)mr_tile,
	(void *)mr_map,
	(void *)mr_sound,
	(void *)mr_sprite,

	//pack
	(void *)pack_filename,
	(void *)start_filename,
	(void *)old_pack_filename,
	(void *)old_start_filename,

	(void *)&mr_ram_file,
	(void *)&mr_ram_file_len,

	(void *)&mr_soundOn,
	(void *)&mr_shakeOn,

	(void *)&LG_mem_base,
	(void *)&LG_mem_len,
	(void *)&LG_mem_end,
	(void *)&LG_mem_left,

	(void *)mr_sms_cfg_buf,

	(void *)mr_md5_init,
	(void *)mr_md5_append,
	(void *)mr_md5_finish,

	(void *)_mr_load_sms_cfg,
	(void *)_mr_save_sms_cfg,

	(void *)_DispUpEx,
	(void *)_DrawPoint,
	(void *)_DrawBitmap,
	(void *)_DrawBitmapEx,
	(void *)DrawRect,
	(void *)_DrawText,
	(void *)_BitmapCheck,
	(void *)_mr_readFile,
	(void *)mr_wstrlen,
	(void *)mr_registerAPP,
	(void *)_DrawTextEx,
	(void *)_mr_EffSetCon,
	(void *)_mr_TestCom,
	(void *)_mr_TestCom1,
	(void *)mr_c2u,
	(void *)mr_div,
	(void *)mr_mod,

	(void *)&LG_mem_min,
	(void *)&LG_mem_top,

	(void *)mr_updcrc,

	(void *)start_fileparameter,

	(void *)&mr_sms_return_flag,
	(void *)&mr_sms_return_val,

	(void *)mr_unzip,

	(void *)&mr_exit_cb,
	(void *)&mr_exit_cb_data,
	(void *)mr_entry,
	(void *)mr_platDrawChar,
	(void *)&LG_mem_free,
	(void *)mr_transbitmapDraw,
	(void *)mr_drawRegion,

	//-- 占坑函数 -------
};

/*当启动DSM应用的时候，用以对DSM平台进行初始化*/
int32 mr_start_dsm(const char *entry)
{
	logi("mr_start_dsm(%s)", entry);

	memset(pack_filename, 0, 0x80);
	if (null != entry && *entry == '*')
		strcpy(pack_filename, entry);
	else if (null != entry && *entry == '%')
		strcpy(pack_filename, entry + 1);
	else if (null != entry && *entry == '#' && entry[1] == '<')
		strcpy(pack_filename, entry + 2);
	else
		strcpy(pack_filename, "*A");

	memset(old_pack_filename, 0, 0x80);
	memset(old_start_filename, 0, 0x80);
	memset(start_fileparameter, 0, 0x80);

	return _mr_intra_start(START_FILE_NAME, entry);
}

int32 _mr_intra_start(const char *filename, const char *entry)
{
	logi("_mr_intra_start(%s, %s)", filename, entry);
	int32 i;

	if (-1 == mr_mem_init())
		return -1;

	mr_event_function = null;
	mr_timer_function = null;
	mr_stop_function = null;
	mr_pauseApp_function = null;
	mr_resumeApp_function = null;

	mr_ram_file = null;
	mr_c_function_P = null;
	mr_c_function_P_len = 0;
	mr_c_function_fix_p = null;
	mr_exception_str = null;

	int32 len = 0;
	mr_screenBuf = null;

	int32 ret = mr_platEx(1001, NULL, 0, (uint8 **)&mr_screenBuf, &len, NULL);
	if (ret == MR_SUCCESS)
	{
		if (null != mr_screenBuf)
		{
			if (len >= 2 * screen.width * screen.height)
			{
				mr_bitmap[BITMAPMAX].type = 1;
				mr_bitmap[BITMAPMAX].buflen = len;
			}
			else
			{
				mr_platEx(1002, (uint8 *)mr_screenBuf, len, NULL, NULL, NULL);
				mr_screenBuf = NULL;
			}
		}
	}

	if (null == mr_screenBuf)
	{
		mr_screenBuf = mr_malloc(2 * screen.width * screen.height);
		mr_bitmap[BITMAPMAX].type = 0;
		mr_bitmap[BITMAPMAX].buflen = 2 * screen.width * screen.height;
	}

	vm_state = 0;
	mr_timer_state = 0;
	mr_timer_run_without_pause = 0;
	bi &= 2;

	memset(mr_bitmap, 0, sizeof(mr_bitmapSt) * BITMAPMAX);
	memset(mr_sound, 0, sizeof(mr_soundSt) * SOUNDMAX);
	memset(mr_sprite, 0, sizeof(mr_spriteSt) * SPRITEMAX);
	memset(mr_tile, 0, sizeof(mr_tileSt) * TILEMAX);
	memset(mr_map, 0, 12);

	for (i = 0; i < TILEMAX; i++)
	{
		mr_tile[i].x1 = 0;
		mr_tile[i].y1 = 0;
		mr_tile[i].x2 = (int16)(screen.width & 0xffff);
		mr_tile[i].y2 = (int16)(screen.height & 0xffff);
	}

	if (NULL == entry)
		entry = "_dsm";

	mr_strncpy(mr_entry, entry, 0x7F);
	mr_state = 1;

	ret = mr_doExt(filename);
	if (0 != ret)
		ret = mr_doExt("logo.ext"); //尝试加载 logo.ext

	if (0 != ret)
	{
		mr_state = 5;
		mr_stop();
		loge("_mr_intra_start, init failed");
		mr_connectWAP("http://wap.mrpej.com");
	}

	return 0;
}

int32 mr_doExt(const char *filename)
{
	logi("mr_doExt(%s)", filename);

	int32 len;
	uint8 *data = _mr_readFile(filename, &len, 0);
	if (NULL != data)
	{
		_mr_TestCom(0, 0xe2d, 0xb61);
		if (_mr_TestComC(800, data, len, 0))
		{
			loge("mr_doExt err:%d", 11002);
			return MR_FAILED;
		}
		else
		{
			int32 ret = _mr_TestComC(801, data, MYTHROAD_VERSION, 6);
			ret = getAppInfo();
			ret = _mr_TestComC(801, (uint8 *)&mrc_appInfo, 16, 8);
			ret = _mr_TestComC(801, data, MYTHROAD_VERSION, 0);
			return MR_SUCCESS;
		}
	}
	else
	{
		loge("mr_doExt err:%d", 11001);
		return MR_FAILED;
	}
}

int32 _mr_TestCom(int32 L, int input0, int input1)
{
	logi("_mr_TestCom(%d, %d, %d)", L, input0, input1);

	int32 ret = 0;

	switch (input0)
	{
	case 0x1:
		ret = mr_getTime();
		break;
	case 0x2:
		mr_event_function = (MR_EVENT_FUNCTION)input1;
		break;
	case 0x3:
		mr_timer_function = (MR_TIMER_FUNCTION)input1;
		break;
	case 0x4:
		mr_stop_function = (MR_STOP_FUNCTION)input1;
		break;
	case 0x5:
		mr_pauseApp_function = (MR_PAUSEAPP_FUNCTION)input1;
		break;
	case 0x6:
		mr_resumeApp_function = (MR_RESUMEAPP_FUNCTION)input1;
		break;
	case 0x7:
		ret = input1;
		break;
	case 0x64:
		ret = LG_mem_min;
		break;
	case 0x65:
		ret = LG_mem_top;
		break;
	case 0x66:
		ret = LG_mem_left;
		break;
	case 0xc8:
		if (1 == mr_state && 0 != mr_shakeOn)
			ret = mr_startShake(input1);
		break;
	case 0x12c:
		mr_soundOn = (int8)input1;
		break;
	case 0x12d:
		mr_shakeOn = (int8)input1;
		break;
	case 0x12e:
		bi |= 0x4;
		break;
	case 0x12f:
		bi &= ~0x4;
		break;
	case 0x130:
		bi |= 0x8;
		break;
	case 0x131:
		bi &= ~0x8;
		break;
	case 0x132:
		mr_sms_return_flag = 1;
		mr_sms_return_val = input1;
		break;
	case 0x133:
		mr_sms_return_flag = 0;
		break;
	case 0x190:
		mr_sleep(input1);
		break;
	case 0x191:
		ret = screen.width;
		screen.width = input1;
		break;
	case 0x194:
		_mr_newSIMInd((int16)input1, NULL);
		break;
	case 0x195:
		ret = mr_closeNetwork();
		break;
	case 0x196:
		ret = screen.height;
		screen.height = input1;
		break;
	case 0x197:
		mr_timer_run_without_pause = (void *)input1;
		mr_plat(1202, input1);
		break;
	case 0x198:
		if (0 == mr_bitmap[BITMAPMAX].type)
		{
			mr_bitmap[BITMAPMAX].p = mr_malloc(input1);
			if (NULL == mr_bitmap[BITMAPMAX].p)
			{
				ret = -1;
			}
			else
			{
				mr_free(mr_screenBuf, mr_bitmap[BITMAPMAX].buflen);
				mr_screenBuf = mr_bitmap[BITMAPMAX].p;
				mr_bitmap[BITMAPMAX].buflen = input1;
			}
		}
		else if (1 == mr_bitmap[BITMAPMAX].type)
		{
			if (mr_bitmap[BITMAPMAX].buflen > input1)
			{
				ret = -1;
			}
		}
		break;
	case 0x1f4:
		ret = _mr_load_sms_cfg();
		break;
	case 0x1f7:
	{
		uint8 dst;

		_mr_smsGetBytes(5, &dst, 1);
		ret = dst;
	}
	break;
	case 0x1f8:
		ret = _mr_save_sms_cfg(input1);
		break;

	case 0xcb3:
		if (0x9E67A == input1)
			bi &= ~0x2;
		break;
	case 0xe2d:
		if (0xb61 == input1)
			bi |= 0x1;
		break;
	case 0xf51:
		if (0x18030 == input1)
			bi |= 0x2;
		break;
	default:
		logw("unknown mr_TestCom(%d, %d, %d)!", L, input0, input1);
		ret = MR_IGNORE;
		break;
	}

	return ret;
}

int32 _mr_TestComC(int32 type, uint8 *input, int32 len, int32 code)
{
	logi("_mr_TestComC(%d, %p, %d, %d)", type, input, len, code);

	if (type == 800)
	{
		mr_load_c_function = (MR_LOAD_C_FUNCTION)(input + 8);
		*(uint32 *)input = (uint32)&_mr_c_function_table;

		mr_cacheSync((void *)((uint32)(input) & (~0x0000001F)),
					 ((len + 0x0000001F * 3) & (~0x0000001F)));

		mr_load_c_function(code);
	}
	else if (type == 801)
	{
		mr_c_function(mr_c_function_P, code, input, len, NULL, NULL);
	}

	return 0;
}

int32 _mr_TestCom1(int32 L, int input0, char *input1, int32 len)
{
	logi("mr_TestCom1(%d, %d, %#p, %d)", L, input0, input1, len);

	int32 ret = 0;

	switch (input0)
	{
	case 2:
		if (null != mr_ram_file)
		{
			mr_free(mr_ram_file, mr_ram_file_len);
			mr_ram_file = null;
		}
		mr_ram_file = input1;
		mr_ram_file_len = len;
		break;
	case 3:
		memset(old_pack_filename, 0, 128);
		if (null != input1)
		{
			strncpy(old_pack_filename, input1, 127);
		}
		memset(old_start_filename, 0, 128);
		strncpy(old_start_filename, START_FILE_NAME, 127);
		break;
	case 4:
		memset(start_fileparameter, 0, 128);
		if (null != input1)
		{
			strncpy(start_fileparameter, input1, 127);
		}
		break;
	case 5:
		mr_exception_str = input1;
		break;
	case 6:
		mr_exception_str = NULL;
		break;
	case 7:
		ret = 0;
		break;
	case 8:
		ret = 0;
		break;
	case 9:
	{
		mr_cacheSync((void *)input1, len);
		ret = 0;

		mr_cacheSync((void *)((uint32)(input1) & (~0x0000001F)),
					 ((len + 0x0000001F * 3) & (~0x0000001F)));

		break;
	}
	case 200:
		mr_updcrc(NULL, 0);
		mr_updcrc(input1, len);
		ret = mr_updcrc(input1, 0);
		break;
	case 700:
		ret = _mr_newSIMInd(((len << 16) >> 16), input1);
		break;
	case 701:
		ret = _mr_smsIndiaction(input1, len, input1 + 1, *(input1 + 2));
		break;
	case 900:
		ret = mr_platEx(200001, (uint8 *)&_mr_c_port_table, 0x10, NULL, NULL, NULL);
		break;
	default:
		logw("unknown mr_testCom1(%d,%d,%#p,%d)!", L, input0, input1, len);
		ret = MR_IGNORE;
		break;
	}

	return ret;
}

int32 _mr_c_function_new(MR_C_FUNCTION f, int32 len)
{
	logi("_mr_c_function_new(%p, %d)", f, len);

	if (NULL != mr_c_function_P)
		mr_free(mr_c_function_P, mr_c_function_P_len);

	mr_c_function_P = (mr_c_function_st *)mr_malloc(len);
	if (NULL != mr_c_function_P)
	{
		mr_c_function_P_len = len;
		memset(mr_c_function_P, 0, mr_c_function_P_len);
		mr_c_function = f;
	}
	else
	{
		mr_state = 5;
		return -1;
	}

	if (NULL == mr_c_function_fix_p)
	{
		*(uint32 *)((uint32)mr_load_c_function - 4) = (uint32)mr_c_function_P;
	}
	else
	{
		*(uint32 *)((uint32)mr_c_function_fix_p + 4) = (uint32)mr_c_function_P;
	}

	mr_cacheFlush(1);

	return MR_SUCCESS;
}

/*退出Mythroad并释放相关资源*/
int32 mr_stop(void)
{
	int32 ret;

	if (NULL != mr_stop_function)
	{
		ret = mr_stop_function();
		mr_stop_function = NULL;
		if (1 != ret)
			return 1;
	}

	return mr_stop_ex(1);
}

int32 mr_stop_ex(int16 freemem)
{
	logi("mr_stop_ex(%d)", freemem);

	if (0 == mr_state)
		return MR_IGNORE;

	if (mr_state == 1 || mr_state == 2)
	{
		c_event_st.code = 8;
		c_event_st.param0 = 0;
		c_event_st.param1 = 0;
		_mr_TestComC(801, (uint8 *)&c_event_st, sizeof(c_event_st), 1);
	}

	mr_state = 0;
	mr_timer_state = 0;
	mr_timer_run_without_pause = 0;

	if (0 != freemem)
	{
		if (mr_bitmap[BITMAPMAX].type != 0 && mr_bitmap[BITMAPMAX].type == 1)
			mr_platEx(1002, (uint8 *)mr_screenBuf, mr_bitmap[BITMAPMAX].buflen, 0, 0, 0);
		mr_screenBuf = NULL;
		mr_mem_end();
	}

	return MR_SUCCESS;
}

int32 mr_registerAPP(uint8 *p, int32 len, int32 index)
{
	if (index >= 50)
	{
		loge("mr_registerAPP err!");
		return -1;
	}
	else
	{
		mr_m0_files[index] = p;
		return 0;
	}
}

/*在Mythroad平台中对事件进行处理*/
int32 mr_event(int16 type, int32 param1, int32 param2)
{
	logi("mr_event(%d, %d, %d)", type, param1, param2);

	if (mr_state == 1 || (mr_timer_run_without_pause && mr_state == 2))
	{
		if (!mr_event_function || (mr_event_function(type, param1, param2) == 1))
		{
			c_event_st.code = type;
			c_event_st.param0 = param1;
			c_event_st.param1 = param2;
			_mr_TestComC(801, (uint8 *)&c_event_st, sizeof(c_event_st), 1);
			return 0;
		}
	}

	return 1;
}

/*恢复应用*/
int32 mr_resumeApp(void)
{
	logi("mr_resumeApp() [mr_state=%d,func=%#p]", mr_state, mr_resumeApp_function);

	if (mr_state == 2)
	{
		mr_state = 1;
		if (!mr_resumeApp_function || (mr_resumeApp_function() == 1))
		{
			_mr_TestComC(801, 0, 1, 5);
			if (mr_timer_state == 2)
			{
				mr_timerStart(300);
				mr_timer_state = 1;
			}
			return 0;
		}
	}
	else if (mr_state == 3)
	{
		mr_timer_p = "restart";
		mr_timerStart(100);
		mr_timer_state = 1;

		return 0;
	}

	return 1;
}

/*暂停应用*/
int32 mr_pauseApp(void)
{
	logi("mr_pauseApp() [mr_state=%d,func=%#p]", mr_state, mr_pauseApp_function);

	if (mr_state == 1)
	{
		mr_state = 2;
		if (!mr_pauseApp_function || (mr_pauseApp_function() == 1))
		{
			_mr_TestComC(801, 0, 1, 4);
			if (!mr_timer_run_without_pause && mr_timer_state == 1)
			{
				mr_timerStop();
				mr_timer_state = 2;
			}
			return 0;
		}
	}
	else if (mr_state == 3)
	{
		mr_timerStop();
		mr_timer_state = 0;
		return 0;
	}

	return 1;
}
