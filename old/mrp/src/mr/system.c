#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "mrp.h"

struct timeval dsmStartTime;
int32 networkID;

char exts[][5] = {".mid", ".wav", ".mp3", ".amr"};

int32 mr_div(int32 a, int32 b)
{
	return a / b;
}

int32 mr_mod(int32 a, int32 b)
{
	return a % b;
}

/*取得时间，单位ms*/
uint32 mr_getTime(void)
{
	logi("mr_getTime()");

	struct timeval t;
	int ret = gettimeofday(&t, null);

	/**
	 * 考虑到 微秒可能会 <0
	 *
	 * 2013-3-22 20:47:23
	 */
	if (t.tv_usec < 0)
	{
		t.tv_sec--;
		t.tv_usec += 1000000;
	}
	uint32 s = ret == 0 ? ((t.tv_sec - dsmStartTime.tv_sec) * 1000 + (t.tv_usec - dsmStartTime.tv_usec) / 1000) : 0;

	//	LOGI("mr_getTime 0x%08x", s);

	return s;
}

/*获取系统日期时间。*/
int32 mr_getDatetime(mr_datetime *datetime)
{
	logi("mr_getDatetime(%p)", datetime);

	if (!datetime)
		return MR_FAILED;

	time_t now;
	struct tm *t;

	time(&now);
	t = localtime(&now);

	//2013-3-22 13:08:50 修正需要加上时间 1900
	datetime->year = t->tm_year + 1900;
	datetime->month = t->tm_mon + 1;
	datetime->day = t->tm_mday;
	datetime->hour = t->tm_hour;
	datetime->minute = t->tm_min;
	datetime->second = t->tm_sec;

	logi("mr_getDatetime [%d/%d/%d %d:%d:%d]", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	return MR_SUCCESS;
}

/*任务睡眠，单位ms*/
int32 mr_sleep(uint32 ms)
{
	logi("mr_sleep(%d)", ms);

	usleep(ms * 1000); //注意 usleep 传的是 微秒 ，所以要 *1000

	return MR_SUCCESS;
}

int32 mr_startShake(int32 ms)
{
	logi("mr_startShake(%d)", ms);
	//TODO
	return MR_SUCCESS;
}

int32 mr_stopShake()
{
	logi("mr_stopShake()");
	//TODO
	return MR_SUCCESS;
}

int32 mr_playSound(int type, const void *data, uint32 dataLen, int32 loop)
{
	logi("mr_playSound(%d, %p, %d, %d)", type, data, dataLen, loop);
	if (type >= 4)
	{
		logw("sound id %d not support!", type);
		return MR_FAILED;
	}
	return MR_FAILED;
	/*

	int32 fd;

	if (MR_IS_DIR != mr_info(".tmp"))
		mr_mkDir(".tmp");

	char buf[128];
	sprintf(buf, ".tmp/tmp%s", exts[type]);

	MR_CHECK_AND_REMOVE(buf);

	fd = mr_open(buf, MR_FILE_RDWR | MR_FILE_CREATE);
	if (fd > 0)
	{
		mr_write(fd, (void *)data, dataLen);
		mr_close(fd);
	}

	char fullpathname[DSM_MAX_FILE_LEN] = {0};
	emu_palySound(get_filename(fullpathname, buf), loop);

	return MR_SUCCESS;
	*/
}

int32 mr_stopSound(int type)
{
	logi("mr_stopSound(%d)", type);
	//TODO
	return MR_SUCCESS;
}

int32 mr_sendSms(char *pNumber, char *pContent, int32 encode)
{
	logi("mr_sendSms(%s, %s, %d)", pNumber, pContent, encode);
	//TODO
	return MR_SUCCESS;
}

void mr_call(char *number)
{
	logi("mr_call(%s)", number);
}

int32 mr_getNetworkID(void)
{
	logi("mr_getNetworkID()");
	return networkID;
}

void mr_connectWAP(char *wap)
{
	logi("mr_connectWAP(%s)", wap);
	//TODO
}