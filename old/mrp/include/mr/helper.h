#ifndef __MR_HELPER_H__
#define __MR_HELPER_H__
#include "common/type.h"

#include "mr/cport.h"
#include "mr/draw.h"
#include "mr/dsm.h"
#include "mr/file.h"
#include "mr/gb2312.h"
#include "mr/gui.h"
#include "mr/md5.h"
#include "mr/mem.h"
#include "mr/mythroad.h"
#include "mr/network.h"
#include "mr/system.h"
#include "mr/timer.h"

typedef int32 (*MR_C_FUNCTION)(void *P, int32 code, uint8 *input, int32 input_len, uint8 **output, int32 *output_len);

typedef void (*T_mrp_settop)(int32 L, int idx);
typedef int32 (*T_mrp_error)(int32 L);

typedef struct
{
	uint8 **mr_m0_files;
	uint32 *vm_state;
	int32 *mr_state;
	int32 *_bi;

	void **mr_timer_p;
	uint32 *mr_timer_state;
	void *mr_timer_run_without_pause;

	void *mr_gzInBuf;
	void *mr_gzOutBuf;
	void *LG_gzinptr;
	void *LG_gzoutcnt;

	int32 *mr_sms_cfg_need_save;
	void *_mr_smsSetBytes;
	void *_mr_smsAddNum;
	void *_mr_newSIMInd;

	void *_mr_isMr;

	void *mrp_gettop; //1937
	T_mrp_settop mrp_settop;
	void *mrp_pushvalue;
	void *mrp_remove;
	void *mrp_insert;
	void *mrp_replace;

	void *mrp_isnumber;
	void *mrp_isstring;
	void *mrp_iscfunction;
	void *mrp_isuserdata;
	void *mrp_type;
	void *mrp_typename;
	void *mrp_shorttypename;

	void *mrp_equal;
	void *mrp_rawequal;
	void *mrp_lessthan;

	void *mrp_tonumber;
	void *mrp_toboolean;
	void *mrp_tostring;
	void *mrp_strlen;
	void *mrp_tostring_t;
	void *mrp_strlen_t;
	void *mrp_tocfunction;
	void *mrp_touserdata;
	void *mrp_tothread;
	void *mrp_topointer;

	void *mrp_pushnil;
	void *mrp_pushnumber;
	void *mrp_pushlstring;
	void *mrp_pushstring;
	void *mrp_pushvfstring;
	void *mrp_pushfstring;
	void *mrp_pushboolean;
	void *mrp_pushcclosure;

	void *mrp_gettable;
	void *mrp_rawget;
	void *mrp_rawgeti;
	void *mrp_newtable;
	void *mrp_getmetatable;

	void *mrp_settable;
	void *mrp_rawset;
	void *mrp_rawseti;

	void *mrp_call;
	void *mrp_pcall;
	void *mrp_load;

	void *mrp_getgcthreshold;
	void *mrp_setgcthreshold;

	T_mrp_error mrp_error;

	void *mrp_checkstack;
	void *mrp_newuserdata;
	void *mrp_getfenv;
	void *mrp_setfenv;
	void *mrp_setmetatable;
	void *mrp_cpcall;
	void *mrp_next;
	void *mrp_concat;
	void *mrp_pushlightuserdata;
	void *mrp_getgccount;
	void *mrp_dump;
	void *mrp_yield;
	void *mrp_resume;

} mr_internal_table;

typedef int32 (*T_mr_c_gcfunction)(int32 code);

typedef struct
{
	void *reserve0;
	void *reserve1;
	void *reserve2;
	T_mr_c_gcfunction mr_c_gcfunction;
} mr_c_port_table;

typedef int32 (*T__mr_c_function_new)(MR_C_FUNCTION f, int32 len);

typedef int32 (*T_mr_exit)(void);

typedef struct
{
	uint16 w;
	uint16 h;
	uint32 buflen;
	uint32 type;
	uint16 *p;
} mr_bitmapSt;

typedef struct
{
	int16 x;
	int16 y;
	uint16 w;
	uint16 h;
	int16 x1;
	int16 y1;
	int16 x2;
	int16 y2;
	uint16 tilew;
	uint16 tileh;
} mr_tileSt;

typedef struct
{
	void *p;
	uint32 buflen;
	int32 type;
} mr_soundSt;

typedef struct
{
	uint16 h;
} mr_spriteSt;

typedef int32 (*T__mr_load_sms_cfg)(void);
typedef int32 (*T__mr_save_sms_cfg)(int32 f);


typedef int32 (*T_mr_registerAPP)(uint8 *p, int32 len, int32 index);

typedef int32 (*T__mr_TestCom)(int32 L, int input0, int input1);
typedef int32 (*T__mr_TestCom1)(int32 L, int input0, char *input1, int32 len);

typedef void (*mrc_timerCB)(int32 data);

typedef struct
{
	T_mr_malloc mr_malloc;
	T_mr_free mr_free;
	T_mr_realloc mr_realloc;

	T_memcpy memcpy;
	T_memmove memmove;
	T_strcpy strcpy;
	T_strncpy strncpy;
	T_strcat strcat;
	T_strncat strncat;
	T_memcmp memcmp;
	T_strcmp strcmp;
	T_strncmp strncmp;
	T_strcoll strcoll;
	T_memchr memchr;
	T_memset memset;
	T_strlen strlen;
	T_strstr strstr;
	T_sprintf sprintf;
	T_atoi atoi;
	T_strtoul strtoul;
	T_rand rand;

	void *reserve0;
	void *reserve1;

	mr_internal_table *_mr_c_internal_table;
	mr_c_port_table *_mr_c_port_table;
	T__mr_c_function_new _mr_c_function_new;

	T_mr_printf mr_printf;
	T_mr_mem_get mr_mem_get;
	T_mr_mem_free mr_mem_free;
	T_mr_drawBitmap mr_drawBitmap;
	T_mr_getCharBitmap mr_getCharBitmap;
	T_mr_timerStart mr_timerStart;
	T_mr_timerStop mr_timerStop;
	T_mr_getTime mr_getTime;
	T_mr_getDatetime mr_getDatetime;
	T_mr_getUserInfo mr_getUserInfo;
	T_mr_sleep mr_sleep;

	T_mr_plat mr_plat;
	T_mr_platEx mr_platEx;

	T_mr_ferrno mr_ferrno;
	T_mr_open mr_open;
	T_mr_close mr_close;
	T_mr_info mr_info;
	T_mr_write mr_write;
	T_mr_read mr_read;
	T_mr_seek mr_seek;
	T_mr_getLen mr_getLen;
	T_mr_remove mr_remove;
	T_mr_rename mr_rename;
	T_mr_mkDir mr_mkDir;
	T_mr_rmDir mr_rmDir;
	T_mr_findStart mr_findStart;
	T_mr_findGetNext mr_findGetNext;
	T_mr_findStop mr_findStop;

	T_mr_exit mr_exit;
	T_mr_startShake mr_startShake;
	T_mr_stopShake mr_stopShake;
	T_mr_playSound mr_playSound;
	T_mr_stopSound mr_stopSound;

	T_mr_sendSms mr_sendSms;
	T_mr_call mr_call;
	T_mr_getNetworkID mr_getNetworkID;
	T_mr_connectWAP mr_connectWAP;

	T_mr_menuCreate mr_menuCreate;
	T_mr_menuSetItem mr_menuSetItem;
	T_mr_menuShow mr_menuShow;
	void *reserve;
	T_mr_menuRelease mr_menuRelease;
	T_mr_menuRefresh mr_menuRefresh;
	T_mr_dialogCreate mr_dialogCreate;
	T_mr_dialogRelease mr_dialogRelease;
	T_mr_dialogRefresh mr_dialogRefresh;
	T_mr_textCreate mr_textCreate;
	T_mr_textRelease mr_textRelease;
	T_mr_textRefresh mr_textRefresh;
	T_mr_editCreate mr_editCreate;
	T_mr_editRelease mr_editRelease;
	T_mr_editGetText mr_editGetText;
	T_mr_winCreate mr_winCreate;
	T_mr_winRelease mr_winRelease;

	T_mr_getScreenInfo mr_getScreenInfo;

	T_mr_initNetwork mr_initNetwork;
	T_mr_closeNetwork mr_closeNetwork;
	T_mr_getHostByName mr_getHostByName;
	T_mr_socket mr_socket;
	T_mr_connect mr_connect;
	T_mr_closeSocket mr_closeSocket;
	T_mr_recv mr_recv;
	T_mr_recvfrom mr_recvfrom;
	T_mr_send mr_send;
	T_mr_sendto mr_sendto;

	uint16 **mr_screenBuf;
	int32 *mr_screen_w;
	int32 *mr_screen_h;
	int32 *mr_screen_bit;
	mr_bitmapSt *mr_bitmap;
	mr_tileSt *mr_tile;
	int16 **mr_map;
	mr_soundSt *mr_sound;
	mr_spriteSt *mr_sprite;

	char *pack_filename;
	char *start_filename;
	char *old_pack_filename;
	char *old_start_filename;

	char **mr_ram_file;
	int32 *mr_ram_file_len;

	int8 *mr_soundOn;
	int8 *mr_shakeOn;

	char **LG_mem_base; //VM 内存基址
	int32 *LG_mem_len;	//VM 内存大小
	char **LG_mem_end;	//VM 内存终止
	int32 *LG_mem_left; //VM 剩余内存

	uint8 *mr_sms_cfg_buf;

	T_mr_md5_init mr_md5_init;
	T_mr_md5_append mr_md5_append;
	T_mr_md5_finish mr_md5_finish;

	T__mr_load_sms_cfg _mr_load_sms_cfg;
	T__mr_save_sms_cfg _mr_save_sms_cfg;

	T__DispUpEx _DispUpEx;
	T__DrawPoint _DrawPoint;
	T__DrawBitmap _DrawBitmap;
	T__DrawBitmapEx _DrawBitmapEx;
	T_DrawRect DrawRect;
	T__DrawText _DrawText;
	T__BitmapCheck _BitmapCheck;
	T__mr_readFile _mr_readFile;
	T_mr_wstrlen mr_wstrlen;
	T_mr_registerAPP mr_registerAPP;
	T__DrawTextEx _DrawTextEx; //1936
	T__mr_EffSetCon _mr_EffSetCon;
	T__mr_TestCom _mr_TestCom;
	T__mr_TestCom1 _mr_TestCom1; //1938
	T_mr_c2u mr_c2u;			 //1939
	T_mr_div mr_div;			 //1941
	T_mr_mod mr_mod;

	uint32 *LG_mem_min;
	uint32 *LG_mem_top;
	void *mr_updcrc;		   //1943
	char *start_fileparameter; //1945
	void *mr_sms_return_flag;  //1949
	void *mr_sms_return_val;
	void *mr_unzip;					   //1950
	mrc_timerCB *mr_exit_cb;		   //1951
	int32 *mr_exit_cb_data;			   //1951
	char *mr_entry;					   //1952,V2000-V2002不支持
	T_mr_platDrawChar mr_platDrawChar; //1961
	uint32 *LG_mem_free;			   //2010
	void *mr_transbitmapDraw;		   //2010
	void *mr_drawRegion;			   //2010
} mr_table;

#endif