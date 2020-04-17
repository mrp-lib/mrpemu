#ifndef __MR_MRP_H__
#define __MR_MRP_H__

#include "common/type.h"

typedef enum
{
	MRP_ORICODING,	 //直接读取编码。只有斯凯的特权用户方可执行该类型操作。
	MRP_FILENAME,	 //MRP文件名称，最大11个有效字符，不包括'\0'
	MRP_APPNAME,	 //MRP应用名称，用户可见的应用名，最大23个有效字符，不包括'\0'，例如"雷电1944"。
	MRP_APPID,		 //每个应用对应一个唯一的ID，uint32类型。
	MRP_APPVER,		 //uint32类型
	MRP_VENDER,		 //开发商信息，最大39个有效字符，不包括'\0'。
	MRP_DESCRIPTION, //应用描述信息 ，最大63个有效字符
	MRP_VERSION,	 //计费通道版本编号,uint16类型，两个字节。
	MRP_CHANNEL,	 //计费通道编号,两个字节，低地址的RecBuf[0]是移动，高地址的RecBuf[1]是联通。
	MRP_RAM,		 //MRP启动需要的RAM大小
	MRP_NONE
} E_MRP_INFOID;

int32 mrc_GetMrpInfoClose(int32 IsFixed, int32 handle);
int32 mrc_GetMrpInfoOpen(char *MrpName, int32 *IsFixed);
static int32 mrc_GetMrpInfoRead(int32 IsFixed, int32 Handle, int32 Offset, uint8 *RecBuf, int32 BufLen);
int32 mrc_GetMrpInfoEx(int32 IsFixed, int32 Handle, E_MRP_INFOID CMD, uint8 *RecBuf, int32 BufLen);

#endif