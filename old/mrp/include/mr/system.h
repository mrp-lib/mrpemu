#ifndef __MR_SYSTEM_H__
#define __MR_SYSTEM_H__

#include "common/type.h"

typedef struct
{
	uint16 year;  //年
	uint8 month;  //月
	uint8 day;	  //日
	uint8 hour;	  //时，24小时制
	uint8 minute; //分
	uint8 second; //秒
} mr_datetime;

/** 网络ID信息 */
enum
{
	MR_NET_ID_MOBILE, //移动
	MR_NET_ID_CN,	  // 联通gsm
	MR_NET_ID_CDMA,	  //联通CDMA
	MR_NET_ID_NONE	  //未插卡
};

typedef int32 (*T_mr_div)(int32 a, int32 b);
typedef int32 (*T_mr_mod)(int32 a, int32 b);

typedef uint32 (*T_mr_getTime)(void);
typedef int32 (*T_mr_getDatetime)(mr_datetime *datetime);
typedef int32 (*T_mr_sleep)(uint32 ms);

typedef int32 (*T_mr_startShake)(int32 ms);
typedef int32 (*T_mr_stopShake)(void);
typedef int32 (*T_mr_playSound)(int type, const void *data, uint32 dataLen, int32 loop);
typedef int32 (*T_mr_stopSound)(int type);

typedef int32 (*T_mr_sendSms)(char *pNumber, char *pContent, int32 flags);
typedef void (*T_mr_call)(char *number);
typedef int32 (*T_mr_getNetworkID)(void);
typedef void (*T_mr_connectWAP)(char *wap);

/** 开始时间，一开始运行就应该初始化 */
extern struct timeval dsmStartTime;
/** 网络ID，开始时应该初始化 */
extern int32 networkID;

int32 mr_div(int32 a, int32 b);
int32 mr_mod(int32 a, int32 b);

uint32 mr_getTime(void);
int32 mr_getDatetime(mr_datetime *datetime);
int32 mr_sleep(uint32 ms);

int32 mr_startShake(int32 ms);
int32 mr_stopShake();

int32 mr_playSound(int type, const void *data, uint32 dataLen, int32 loop);
int32 mr_stopSound(int type);

int32 mr_sendSms(char *pNumber, char *pContent, int32 encode);
void mr_call(char *number);
int32 mr_getNetworkID(void);
void mr_connectWAP(char *wap);

#endif