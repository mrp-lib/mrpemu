#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mrp.h"

#define DSM_PLAT_VERSION (8)
#define DSM_FAE_VERSION (182)

#define DSM_ENWORD_H 16
#define DSM_ENWORD_W 8
#define DSM_CHWORD_H 16
#define DSM_CHWORD_W 16

//屏幕信息
mr_screeninfo screen;
//手机信息
mr_userinfo userInfo;
//应用信息
mrc_appInfoSt_st mrc_appInfo;

int8 mr_soundOn;
int8 mr_shakeOn;

uint16 *cacheScreenBuffer; //缓冲屏幕地址
uint16 *realScreenBuffer;

T_DSM_DISK_INFO dsmDiskInfo;
T_DSM_CELL_INFO dsmCellInfo;

//获取屏幕信息
int32 mr_getScreenInfo(mr_screeninfo *s)
{
	logi("mr_getScreenInfo(%p)", s);
	if (s)
	{
		s->width = screen.width;
		s->height = screen.height;
		s->bit = screen.bit;
	}
	return MR_SUCCESS;
}

int32 mr_getUserInfo(mr_userinfo *info)
{
	logi("mr_getUserInfo(%p)", info);

	if (info == NULL)
		return MR_FAILED;

	memset(info, 0, sizeof(mr_userinfo));
	memcpy(info->IMEI, userInfo.IMEI, 15);
	memcpy(info->IMSI, userInfo.IMSI, 15);

	strncpy(info->manufactory, userInfo.manufactory, 7);
	strncpy(info->type, userInfo.type, 7);

	info->ver = 101000000 + DSM_PLAT_VERSION * 10000 + DSM_FAE_VERSION;

	memset(info->spare, 0, sizeof(info->spare));

	return MR_SUCCESS;
}

int32 getAppInfo()
{
	int32 IsFixed = 0, Handle = 0;

	Handle = mrc_GetMrpInfoOpen(pack_filename, &IsFixed);
	if (Handle == 0)
	{
		return MR_FAILED;
	}

	mrc_GetMrpInfoEx(IsFixed, Handle, MRP_APPID, (uint8 *)&mrc_appInfo.id, 4);
	mrc_GetMrpInfoEx(IsFixed, Handle, MRP_APPVER, (uint8 *)&mrc_appInfo.ver, 4);
	if (mrc_appInfo.ram == 0)
	{
		mrc_GetMrpInfoEx(IsFixed, Handle, MRP_RAM, (uint8 *)&mrc_appInfo.ram, 4);
	}

	mrc_appInfo.sidName = NULL;
	mrc_GetMrpInfoClose(IsFixed, Handle);
	return MR_SUCCESS;
}

/*平台扩展接口*/
int32 mr_plat(int32 code, int32 param)
{
	logi("mr_plat(%d, %d)", code, param);

	switch (code)
	{
	case MR_GET_FILE_POS: //获取文件读写指针
	{
		int32 ret = lseek(param - 5, 0, SEEK_CUR);
		if (ret >= 0)
			return (ret + MR_PLAT_VALUE_BASE);
		else
			return MR_FAILED;
	}

	case MR_CONNECT:
	{
		// return mr_getSocketState(param);
		return MR_SUCCESS;
	}

	case MR_SET_SOCTIME:
	{
		return MR_IGNORE;
	}

	case MR_GET_RAND: //1211
	{
		srand(mr_getTime());
		return (MR_PLAT_VALUE_BASE + rand() % param);
	}

	case MR_CHECK_TOUCH: //是否支持触屏
	{
		return MR_NORMAL_SCREEN;
	}

	case MR_GET_HANDSET_LG: //获取语言
	{
		return MR_CHINESE;
	}

	case 1391: //查询后台支持
	{
		return MR_IGNORE;
	}

	case 1106: //获取短信中心
	{
		const char *args[1];
		args[0] = "getSmsCenter";
		// N2J_callVoidMethodString(1, args);
		return MR_WAITING;
	}

	case 1016: //初始化信号强度
		return MR_SUCCESS;

	case 1302: // 设置音量
		//TODO
		// emu_musicCMD(1302, param, param);
		return MR_SUCCESS;

	case 1214:
		return MR_SUCCESS;

	case 1327: //查询 WIFI 是否可用
		return MR_IGNORE;
		//return (emu_getIntSysinfo(SYSINFO_NETTYPE)==NETTYPE_WIFI? MR_SUCCESS : MR_FAILED);
	case 1328: //设置是否使用 WIFI
		return MR_SUCCESS;

	case 1011: //设置收到新短信的时候是否显示提示界面。1提示
		return MR_SUCCESS;

	case 1218:
		return MR_MSDC_OK;

	default:
		logw("mr_plat(code:%d, param:%d) not impl!", code, param);
		break;
	}

	return MR_IGNORE;
}

int32 mr_platEx(int32 code, uint8 *input, int32 input_len, uint8 **output, int32 *output_len, MR_PLAT_EX_CB *cb)
{
	logi("mr_platEx(%d,%p,%d,%p,%p,%p)", code, input, input_len, output, output_len, cb);

	switch (code)
	{
	case MR_MALLOC_EX: //1001申请屏幕缓冲区，这里给的值即 VM 的屏幕缓冲区
	{
		*output = (uint8 *)cacheScreenBuffer;
		*output_len = screen.width * screen.height * 2;
		logi("mocall ram2 addr=%p l=%d", output, *output_len);
		return MR_SUCCESS;
	}

	case MR_MFREE_EX: //1002
	{
		return MR_SUCCESS;
	}

	case 1012: //申请内部cache
	{
		*output = NULL;
		return MR_SUCCESS;
	}
	case 1013: //释放内部cache
	{
		return MR_SUCCESS;
	}

	case 1014: //申请拓展内存
	{
		// if (gEmuEnv.enableExram)
		// {
		*output_len = screen.width * screen.height * 4;
		*output = malloc(*output_len);
		logi("malloc exRam addr=%p len=%d", output, output_len);
		return MR_SUCCESS;
		// }
		// else
		// {
		// 	*output = NULL;
		// 	*output_len = 0;
		// 	return MR_IGNORE;
		// }
	}

	case 1015: //释放拓展内存
	{
		// if (gEmuEnv.enableExram)
		// {
		logi("free exRam");
		free(input);
		return MR_SUCCESS;
		// }

		// return MR_IGNORE;
	}

	case MR_TUROFFBACKLIGHT: //关闭背光常亮
	{
		//TODO
		return MR_SUCCESS;
	}
	case MR_TURONBACKLIGHT: //开启背光常亮
	{
		//TODO
		return MR_SUCCESS;
	}

	case MR_SWITCHPATH: //切换跟目录 1204
		// return dsmSwitchPath(input, input_len, output, output_len);
		//TODO
		return MR_SUCCESS;

	case MR_GET_FREE_SPACE:
	{
		//TODO
		// if (dsmGetFreeSpace(input, input_len, &dsmDiskInfo) == MR_SUCCESS)
		// {
		// 	*output = (uint8 *)&dsmDiskInfo;
		// 	*output_len = sizeof(T_DSM_DISK_INFO);
		// 	return MR_SUCCESS;
		// }
		return MR_FAILED;
	}

	case MR_UCS2GB: //1207
	{
		if (!input || input_len == 0)
		{
			loge("mr_platEx(1207) input err");
			return MR_FAILED;
		}

		if (!*output)
		{
			loge("mr_platEx(1207) ouput err");
			return MR_FAILED;
		}

		//input is bigend
		{
			int len = UCS2_strlen(input);
			char *buf = malloc(len + 2);

			int gbBufLen = len + 1;
			char *gbBuf = malloc(gbBufLen);

			memcpy(buf, input, len + 2);
			UCS2ByteRev(buf);
			UCS2ToGBString((uint16 *)buf, gbBuf, gbBufLen);

			strcpy(*output, gbBuf);
			free(buf);
		}

		return MR_SUCCESS;
	}

	case MR_CHARACTER_HEIGHT:
	{
		static int32 wordInfo = (DSM_ENWORD_H << 24) | (DSM_ENWORD_W << 16) | (DSM_CHWORD_H << 8) | (DSM_CHWORD_W);
		*output = (unsigned char *)&wordInfo;
		*output_len = 4;

		return MR_SUCCESS;
	}

	case 1116: //获取编译时间
	{
		static char buf[32];

		int l = snprintf(buf, sizeof(buf), "%s %s", __TIME__, __DATE__);
		*output = (uint8 *)buf; //"2013/3/21 21:36";
		*output_len = l + 1;

		logi("build time %s", buf);

		return MR_SUCCESS;
	}

	case 1307: //获取SIM卡个数，非多卡多待直接返回 MR_INGORE
		return MR_IGNORE;

	case 0x90004: //获取型号强度
		return 5;

	case 1224: //小区信息ID
	{
		memset(&dsmCellInfo, 0, sizeof(T_DSM_CELL_INFO));
		*output = (uint8 *)&dsmCellInfo;
		*output_len = sizeof(T_DSM_CELL_INFO);
		logw("get T_DSM_CELL_INFO");
		return MR_IGNORE;
	}

	case 1017: //获得信号强度。
	{
		static T_RX rx = {3, 5, 5, 1};

		*output = (uint8 *)&rx;
		*output_len = sizeof(T_RX);
		return MR_SUCCESS;
	}

	case 3001: //获取图片信息
	{
		//TODO
		// MRAPP_IMAGE_ORIGIN_T *t = (MRAPP_IMAGE_ORIGIN_T *)input;

		// *output = (uint8 *)dsmGetImageSize(t);
		// *output_len = sizeof(MRAPP_IMAGE_SIZE_T);

		return MR_SUCCESS;
	}

	case 3010: //画图
	{
		//TODO
		// T_DRAW_DIRECT_REQ *t = (T_DRAW_DIRECT_REQ *)input;
		// dsmDrawImage(t);
		return MR_SUCCESS;
	}

	default:
		logw("mr_platEx not impl for %d", code);
		break;
	}

	int cmd = code / 10;
	switch (cmd)
	{
	case MR_MEDIA_INIT:
		//TODO
		// emu_musicCMD(cmd, 0, 0);
		return MR_SUCCESS;

	case MR_MEDIA_BUF_LOAD:
	{
		//TODO
		// char fullpathname[DSM_MAX_FILE_LEN] = {0};
		// get_filename(fullpathname, "musicbuf");
		// writeDataToTempFile(fullpathname, input, input_len);
		// emu_musicLoadFile(fullpathname);

		return MR_SUCCESS;
	}

	case MR_MEDIA_FILE_LOAD:
	{
		//TODO
		// char fullpathname[DSM_MAX_FILE_LEN] = {0};
		// emu_musicLoadFile((const char *)get_filename(fullpathname, (const char *)input));
		return MR_SUCCESS;
	}

	case MR_MEDIA_PLAY_CUR_REQ:
	{
		//TODO
		// if (input_len >= sizeof(T_DSM_MEDIA_PLAY) && input != NULL)
		// {
		// 	lgoi("mediaplay need callback!");

		// 	T_DSM_MEDIA_PLAY *pt = (T_DSM_MEDIA_PLAY *)input;

		// 	dsmMediaPlay.cb = pt->cb;

		// 	emu_musicCMD(MR_MEDIA_PLAY_CUR_REQ, pt->loop, 1);
		// }
		// else
		// {
		// 	emu_musicCMD(MR_MEDIA_PLAY_CUR_REQ, 0, 0);
		// }

		return MR_SUCCESS;
	}

	case MR_MEDIA_PAUSE_REQ:
	case MR_MEDIA_RESUME_REQ:
	case MR_MEDIA_STOP_REQ:
	case MR_MEDIA_CLOSE:
		//TODO
		// emu_musicCMD(cmd, 0, 0);
		return MR_SUCCESS;

	case MR_MEDIA_GET_STATUS:
		//TODO
		// return emu_musicCMD(cmd, 0, 0);
		return MR_FAILED;

	case MR_MEDIA_SETPOS:
	{
		//TODO
		// T_SET_PLAY_POS *pos = (T_SET_PLAY_POS *)input;
		// emu_musicCMD(cmd, pos->pos * 1000, 0); //上层需要 ms
		return MR_SUCCESS;
	}

	case MR_MEDIA_GET_TOTAL_TIME:
	case MR_MEDIA_GET_CURTIME:
	case MR_MEDIA_GET_CURTIME_MSEC:
	{
		//TODO
		// int ret = emu_musicCMD(cmd, 0, 0);
		// if (ret == -1)
		// {
		// 	LOGE("emu_musicCMD(%d) error", cmd);
		// 	return MR_FAILED;
		// }
		// else
		// {
		// 	dsmCommonRsp.pos = ret;
		// 	*output = (uint8 *)&dsmCommonRsp;
		// 	*output_len = sizeof(int32);
		// 	return MR_SUCCESS;
		// }
		return MR_FAILED;
	}

	case MR_MEDIA_FREE:
		//TODO
		// emu_musicCMD(cmd, 0, 0);
		return MR_SUCCESS;

	default:
		logw("mr_platEx(code=%d, input=%p, il=%d) not impl!", code, (void *)input, input_len);
		break;
	}

	return MR_IGNORE;
}

int32 _mr_isMr(void *buf)
{
	logi("_mr_isMr(%p)", buf);
	return 0;
}