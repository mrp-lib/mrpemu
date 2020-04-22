#ifndef __HELPER_MR_H__
#define __HELPER_MR_H__

#include "common/type.h"
#include "mrp/mem.h"

#define MAX_FILE_PATH_LEN 256 //最大文件路径长度

#define MR_SUCCESS 0 //成功
#define MR_FAILED -1 //失败
#define MR_IGNORE 1	 //不关心
#define MR_WAITING 2 //异步(非阻塞)模式

#define mr_ret(val) (vm->cpu->registers[0] = (val)) //从系统调用中返回一个值给调用函数
#define mrst (vm->mem->mr_state)					//获取当前虚拟机中的一些运行时状态

#define MR_FILE_MAX_LEN 128		   //mrp文件名及mrp内部文件名的最大长度
#define MRST_MAX_0FILES 50		   //注册固化应用的最大数量
#define MRST_INTERNAL_TABLE_MAX 16 //c_internal_table的最大长度
#define MRST_CPORT_TABLE_MAX 4	   //c_port_table的最大长度

#define BITMAPMAX 30
#define SPRITEMAX 10
#define TILEMAX 3
#define SOUNDMAX 5

typedef struct
{
	uint16 w;
	uint16 h;
	uint32 buflen;
	uint32 type;
	// uint16 *p;
	vmpt p;
} mr_bitmap_t;

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
} mr_tile_t;

typedef struct
{
	//    void*            p;
	vmpt p;
	uint32 buflen;
	int32 type;
} mr_sound_t;

typedef struct
{
	uint16 h;
} mr_sprite_t;

/*
系统信息，这里的信息是可以被修改的。
*/
typedef struct mr_sys_info_st
{
	/*
		网络ID
		0	移动
		1	联动GSM
		2	联通CDMA
		3	未插卡
	*/
	uint32 networkID;

	uint32 screen_width;  //屏幕宽度
	uint32 screen_height; //屏幕高度
	uint32 screen_bits;	  //屏幕色深，24位

	uint8 IMEI[16];		 //IMEI
	uint8 IMSI[16];		 //IMSI
	char manufactory[8]; //厂商名，最大7个字符，空字节填\0
	char type[8];		 //mobile type，最大7个字符，空字节填\0
	uint32 ver;			 //SW ver
	uint8 spare[12];	 //备用

	char sdcard_dir[MAX_FILE_PATH_LEN]; //SD卡目录
	char dsm_dir[MAX_FILE_PATH_LEN];	//dsm目录，一般是mythroad啦

} mr_sys_info_t;

/*
应用信息
*/
typedef struct mrc_appInfo_st
{
	int32 id;
	int32 ver;
	vmpt sidName;
} mrc_appInfo_t;

/*
mrp运行过程中的一些状态
这些状态会被装在到虚拟机中，并可以被虚拟机访问
*/
typedef struct
{
	/*
	系统信息
	这个信息需在虚拟机被创建之后mrp启动之前进行配置。
	当函数表安装函数被调用时，这些信息的指针会被填写到函数表的位置，因为是指针，所以什么时候设置都可以用，只要是在mrp启动前就好。
	*/
	mr_sys_info_t sysinfo;
	/*
	从mrp文件中读取到内容+8的数据(也就是程序指令部分)，将该位置的内存指针存放在这里。
	也就是说，这个函数是一个入口函数。
	这个函数会在程序装在到内存之后进行设置。
	原型: uint32 mr_c_function_load(uint32 code);
	*/
	uint32 mr_c_function_load;
	/*
	这里存放的是mr_c_function_st结构的指针，当mr_c_function_new被调用时会分配该结构，并将指针放在此处。
	*/
	vmpt mr_c_function_P;
	uint32 mr_c_function_P_Len; //也就是上面那个结构的长度啦。
	/*
	mr_c_function函数的指针
	这个函数会在SWI_MR_C_FUNCTION_NEW中断发生时传递过来。
	*/
	uint32 mr_c_function;
	/*
	注册固化列表，当调用mr_registerAPP函数时会往这里面填写
	在初始化mr函数表的时候回被填写到c_internal_table的第0个位置
	*/
	vmpt mr_m0_files[MRST_MAX_0FILES];
	/*
	这里面存储的是一些状态吧，有些不是太明白
	*/
	vmpt mr_c_internal_table[MRST_INTERNAL_TABLE_MAX];
	vmpt mr_c_port_table[MRST_CPORT_TABLE_MAX];
	/*
	内存状态
	在调用内存初始化函数的时候回填充此结构，并将此结构中的值填充到函数表的特定位置。
	*/
	mr_mem_state_t mem_state;

	/*
	包名称，在调用start_dsmX函数时填入，同样的，这两个名称也会映射到mr函数表中
	*/
	char pack_filename[MR_FILE_MAX_LEN];
	char old_pack_filename[MR_FILE_MAX_LEN];
	char start_filename[MR_FILE_MAX_LEN];
	char old_start_filename[MR_FILE_MAX_LEN];
	char start_file_parameter[MR_FILE_MAX_LEN];
	char entry[MR_FILE_MAX_LEN];

	/*应用信息，在启动mrp的时候回填充此结构*/
	mrc_appInfo_t appInfo;

	vmpt mr_ram_file;
	uint32 mr_ram_file_len;

	mr_bitmap_t mr_bitmap[BITMAPMAX + 1];
	mr_tile_t mr_tile[TILEMAX];
	vmpt mr_map[TILEMAX];
	mr_sound_t mr_sound[SOUNDMAX];
	mr_sprite_t mr_sprite[SPRITEMAX];

	/*
	下面这几个函数是由mrp通过testCom注册的
	*/
	vmpt mr_event_function;		//原型： (int32 code, int32 param0, int32 param1) -> int32
	vmpt mr_timer_function;		//原型： (void) -> int32
	vmpt mr_stop_function;		//原型： (void) -> int32
	vmpt mr_pauseApp_function;	//原型： (void) -> int32
	vmpt mr_resumeApp_function; //原型： (void) -> int32

	//一些标志状态
	int8 mr_soundOn;
	int8 mr_shakeOn;
	int32 bi;

	vmpt vm_state; //mrp_State*
	int32 mr_state;

	vmpt mr_timer_p; //void **
	int32 mr_timer_state;
	int32 mr_timer_run_without_pause;

	uint8 mr_sms_return_flag;
	int32 mr_sms_return_val;
	vmpt mr_sms_cfg_buf; //实际类型uint8*

} mr_state_t;

typedef int32 (*MR_C_FUNCTION)(void *P, int32 code, uint8 *input, int32 input_len, uint8 **output, int32 *output_len);

#endif