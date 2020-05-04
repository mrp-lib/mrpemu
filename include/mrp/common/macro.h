#ifndef __MRP_COMMON_MACRO_H__
#define __MRP_COMMON_MACRO_H__

#define MAX_FILE_PATH_LEN 256u //最大文件路径长度

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

#endif