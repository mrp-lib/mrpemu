#ifndef __MR_MYTHROAD_H__
#define __MR_MYTHROAD_H__

#include "common/type.h"

/** 启动文件 */
#define START_FILE_NAME "cfunction.ext"

// mr_plat枚举
#define MR_NES_SET_WRITE_ADDR 102
#define MR_NES_GET_READ_ADDR 103
#define MR_GOTO_BASE_WIN 104
#define MR_LIST_CREATE 1
#define MR_LIST_SET_ITEM 2
#define MR_SET_ACTIVE_SIM 1004
#define MR_SET_VOL 1302
#define MR_CONNECT 1001
#define MR_SET_SOCTIME 1002
#define MR_BIND_PORT 1003
#define MR_ACTIVE_APP 1003
#define MR_CHARACTER_HEIGHT 1201

// mr_platEx枚举
#define MR_MALLOC_EX 1001
#define MR_MFREE_EX 1002
#define MR_BACKSTAGE 1004
#define MR_SHOW_PIC 1005
#define MR_STOP_SHOW_PIC 1006
#define MR_APPEND_SMS 1007
#define MR_MALLOC_SCRRAM 1014
#define MR_FREE_SCRRAM 1015

#define MR_GET_CHARACTOR_INFO 1201
#define MR_SET_EVENT_FLAG 1202
#define MR_SEND_MMS 1203
#define MR_SWITCHPATH 1204
#define MR_CHECK_TOUCH 1205
#define MR_GET_HANDSET_LG 1206
#define MR_UCS2GB 1207
#define MR_SET_RING 1208
#define MR_GET_AUDIO_INFO 1209
#define MR_GET_KEYPAD_MAP 1210
#define MR_GET_RAND 1211
#define MR_GET_SCENE 1213
#define MR_SET_KEY_END 1214
#define MR_GET_CELL_ID_START 1215
#define MR_GET_CELL_ID_STOP 1216
#define MR_WEATHER_EXIT 1217
#define MR_GET_NES_DEFAULT_DIR 1220
#define MR_GET_APPLIST_TITLE 1221
#define MR_TURONBACKLIGHT 1222
#define MR_TUROFFBACKLIGHT 1223
#define MR_GET_CELL_INFO 1224
#define MR_GET_FILE_POS 1231
#define MR_GET_FREE_SPACE 1305
#define MR_GET_SIM_INFO 1307

#define MR_MEDIA_INIT 201
#define MR_MEDIA_BUF_LOAD 203
#define MR_MEDIA_FILE_LOAD 202
#define MR_MEDIA_PLAY_CUR_REQ 204
#define MR_MEDIA_PAUSE_REQ 205
#define MR_MEDIA_RESUME_REQ 206
#define MR_MEDIA_STOP_REQ 207
#define MR_MEDIA_CLOSE 208
#define MR_MEDIA_GET_STATUS 209
#define MR_MEDIA_SETPOS 210
#define MR_MEDIA_GETTIME 211
#define MR_MEDIA_GET_TOTAL_TIME 212
#define MR_MEDIA_GET_CURTIME 213
#define MR_MEDIA_GET_CURTIME_MSEC 215
#define MR_MEDIA_FREE 216
#define MR_MEDIA_ALLOC_INRAM 220
#define MR_MEDIA_FREE_INRAM 221

#define MR_MEDIA_OPEN_MUTICHANNEL 222
#define MR_MEDIA_PLAY_MUTICHANNEL 223
#define MR_MEDIA_STOP_MUTICHANNEL 224
#define MR_MEDIA_CLOSE_MUTICHANNEL 225

#define MR_GET_IMG_INFO 3001
#define MR_DECODE_IMG 3002
#define MR_DECODE_STATUS 3003
#define MR_GIF_DECODE 3004
#define MR_GIF_RELEASE 3005
#define MR_DRAW_BUFFER 3007
#define MR_GET_ACT_LAYER 3008
#define MR_DISPLAY_LCD 3009

#define MR_PLAT_VALUE_BASE 1000

/** 屏幕信息定义 */
typedef struct
{
	/** 屏幕宽 */
	uint32 width;
	/** 屏幕高 */
	uint32 height;
	/** 屏幕象素深度，单位bit */
	uint32 bit;
} mr_screeninfo;

typedef struct
{
	uint8 IMEI[16];		 //IMEI len eq 15
	uint8 IMSI[16];		 //IMSI len not more then 15
	char manufactory[8]; //厂商名，最大7个字符，空字节填\0
	char type[8];		 //mobile type，最大7个字符，空字节填\0
	uint32 ver;			 //SW ver
	uint8 spare[12];	 //备用
} mr_userinfo;

//应用信息
typedef struct
{
	int32 id;
	int32 ver;
	char *sidName;
	int32 ram;
} mrc_appInfoSt_st;

//存储空间信息
typedef struct
{
	uint32 total;
	uint32 tUnit;
	uint32 account;
	uint32 unit;
} T_DSM_DISK_INFO;

//小区信息
typedef struct
{
	uint16 lac;
	uint16 cell_id;
	uint8 mnc[2];
	uint8 mcc[3];
	uint8 mnc3[4];
} T_DSM_CELL_INFO;

//信号强度
typedef struct
{
	uint8 level;
	uint8 current_band;
	uint8 rat;
	uint8 flag;
} T_RX;

//获取图片信息
typedef struct
{
	char *src;
	int32 len;
	int32 src_type; // MRAPP_SRC_TYPE
} MRAPP_IMAGE_ORIGIN_T;

typedef struct
{
	int32 width;  //图片的宽度
	int32 height; //图片的高度
} MRAPP_IMAGE_SIZE_T;

//绘图
typedef struct
{
	char *src;
	int32 src_len;
	int32 src_type;
	int32 ox;
	int32 oy;
	int32 w;
	int32 h;
} T_DRAW_DIRECT_REQ;

//屏幕类型
typedef enum
{
	MR_NORMAL_SCREEN = MR_PLAT_VALUE_BASE,
	MR_TOUCH_SCREEN,
	MR_ONLY_TOUCH_SCREEN
} MR_SCREEN_TYPE;

//SD卡信息
enum
{
	MR_MSDC_NOT_EXIST = MR_PLAT_VALUE_BASE,
	MR_MSDC_OK,
	MR_MSDC_NOT_USEFULL /*可能在usb模式导致无法操作t卡*/
};

//语言
typedef enum
{
	MR_CHINESE = MR_PLAT_VALUE_BASE,
	MR_ENGLISH,
	MR_TCHINESE,   //繁体
	MR_SPANISH,	   //西班牙
	MR_DANISH,	   //丹麦语
	MR_POLISH,	   //波兰
	MR_FRENCH,	   //法国
	MR_GERMAN,	   //德国
	MR_ITALIAN,	   //意大利
	MR_THAI,	   //泰语
	MR_RUSSIAN,	   // 俄罗斯
	MR_BULGARIAN,  //保加利亚
	MR_UKRAINIAN,  //乌克兰
	MR_PORTUGUESE, //葡萄牙
	MR_TURKISH,	   //土耳其
	MR_VIETNAMESE, //越南
	MR_INDONESIAN, //印度尼西亚
	MR_CZECH,	   //捷克
	MR_MALAY,	   //马来西亚
	MR_FINNISH,	   //芬兰的
	MR_HUNGARIAN,  //匈牙利
	MR_SLOVAK,	   //斯洛伐克
	MR_DUTCH,	   //荷兰
	MR_NORWEGIAN,  //挪威
	MR_SWEDISH,	   //瑞典
	MR_CROATIAN,   //克罗地亚
	MR_ROMANIAN,   //罗马尼亚
	MR_SLOVENIAN,  //斯洛文尼亚
	MR_GREEK,	   //希腊语
	MR_HEBREW,	   //希伯来
	MR_ARABIC,	   //阿拉伯
	MR_PERSIAN,	   //波斯
	MR_URDU,	   //乌尔都语
	MR_HINDI,	   //北印度
	MR_MARATHI,	   //马拉地语(属印欧语系印度语族)
	MR_TAMIL,	   //泰米尔语
	MR_BENGALI,	   //孟加拉人(语
	MR_PUNJABI,	   //印度西北部的一地方
	MR_TELUGU	   //泰卢固语(印度东部德拉维拉语言)
} MR_LANGUAGE;

typedef int32 (*T_mr_getScreenInfo)(mr_screeninfo *screeninfo);
typedef int32 (*T_mr_getUserInfo)(mr_userinfo *info);

typedef int32 (*T_mr_plat)(int32 code, int32 param);
typedef void (*MR_PLAT_EX_CB)(uint8 *output, int32 output_len);
typedef int32 (*T_mr_platEx)(int32 code, uint8 *input, int32 input_len, uint8 **output, int32 *output_len, MR_PLAT_EX_CB *cb);

/** 屏幕信息，一般来说在正式运行之前应该初始化此变量 */
extern mr_screeninfo screen;

/** 手机信息，运行前应该初始化此变量，（需要初始化IMEI、IMSI、manufactory和type参数，其他不用处理） */
extern mr_userinfo userInfo;

//应用信息
extern mrc_appInfoSt_st mrc_appInfo;

extern int8 mr_soundOn;
extern int8 mr_shakeOn;

int32 getAppInfo();

int32 mr_getScreenInfo(mr_screeninfo *s);
int32 mr_getUserInfo(mr_userinfo *info);

int32 mr_plat(int32 code, int32 param);
int32 mr_platEx(int32 code, uint8 *input, int32 input_len, uint8 **output, int32 *output_len, MR_PLAT_EX_CB *cb);

int32 _mr_isMr(void *buf);

#endif