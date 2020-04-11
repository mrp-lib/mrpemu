

#include "mrp.h"

int32 mrc_GetMrpInfoClose(int32 IsFixed, int32 handle)
{
	if (!handle)
		return MR_FAILED;
	if (IsFixed)
		return MR_SUCCESS;
	else
		return mr_close(handle);
}

int32 mrc_GetMrpInfoOpen(char *MrpName, int32 *IsFixed)
{
	uint32 Handle = 0;
	if (!IsFixed || !MrpName)
		return 0;
	if (MrpName[0] == '*')
	{ /*m0 file?*/
		*IsFixed = 1;
		Handle = (uint32)mr_m0_files[MrpName[1] - 0x41];
	}
	else if (MrpName[0] == '$')
	{
		*IsFixed = 1;
		Handle = (uint32)mr_ram_file;
	}
	else
	{
		*IsFixed = 0;
		Handle = mr_open(MrpName, MR_FILE_RDONLY);
	}
	return Handle;
}

static int32 mrc_GetMrpInfoRead(int32 IsFixed, int32 Handle, int32 Offset, uint8 *RecBuf, int32 BufLen)
{
	int32 ret;
	if (!RecBuf)
		return MR_FAILED;

	if (IsFixed)
	{
		memcpy(RecBuf, (uint8 *)Handle + Offset, BufLen);
		ret = BufLen;
	}
	else
	{
		ret = mr_seek(Handle, Offset, MR_SEEK_SET);
		if (MR_SUCCESS != ret)
			return MR_FAILED;
		ret = mr_read(Handle, RecBuf, BufLen);
		if (ret != BufLen)
			return MR_FAILED;
	}
	return ret;
}

int32 mrc_GetMrpInfoEx(int32 IsFixed, int32 Handle, E_MRP_INFOID CMD, uint8 *RecBuf, int32 BufLen)
{
	int32 ret = MR_FAILED, DotOffset = 0;
	uint32 temp, *p32;
	//LIB_EXB_LOG(("mrc_GetMrpInfo:开始入参检查。"));
	memset(RecBuf, 0, BufLen);
	switch (CMD)
	{
	case MRP_APPID: //每个应用对应一个唯一的ID，uint32类型。
		if (BufLen < 4)
			break;
		ret = mrc_GetMrpInfoRead(IsFixed, Handle, 192, RecBuf, 4);
		p32 = (uint32 *)RecBuf;
		temp = *p32;
#ifndef MR_SPREADTRUM_MOD //LIB_SPREADTRUM_MOD
		//MTK平台，需要转为网络字节序
		temp = htonl(temp);
		//LIB_DBG(("MTK平台，转换为网络字节序后temp=%0x",temp));
#endif
		memcpy(RecBuf, &temp, 4);
		break;
	case MRP_APPVER: //uint32类型
		if (BufLen < 4)
			break;
		ret = mrc_GetMrpInfoRead(IsFixed, Handle, 196, RecBuf, 4);
		p32 = (uint32 *)RecBuf;
		temp = *p32;
#ifndef MR_SPREADTRUM_MOD //LIB_SPREADTRUM_MOD
		//MTK平台，需要转为网络字节序
		temp = htonl(temp);
		//LIB_DBG(("MTK平台，转换为网络字节序后temp=%0x",temp));
#endif
		memcpy(RecBuf, &temp, 4);
		break;
	case MRP_RAM: //uint32类型
	{
		uint16 ram, ram_check;
		if (BufLen < 4)
			break;
		ret = mrc_GetMrpInfoRead(IsFixed, Handle, 228, (uint8 *)&ram, 2);
		ret = mrc_GetMrpInfoRead(IsFixed, Handle, 230, (uint8 *)&ram_check, 2);
#ifndef MR_SPREADTRUM_MOD //LIB_SPREADTRUM_MOD
		//MTK平台，需要转为网络字节序
		ram = htonl(ram);
		ram_check = htonl(ram_check);
		//LIB_DBG(("MTK平台，转换为网络字节序后temp=%0x",temp));
#endif
		mr_updcrc(NULL, 0); /* initialize crc */
		mr_updcrc((unsigned char *)&ram, 2);
		mr_updcrc((unsigned char *)&ram, 2);
		if (ram_check == (mr_updcrc((unsigned char *)&ram, 0) & 0xFF))
		{
			*(uint32 *)RecBuf = ram;
		}
		break;
	}
	default:;
	}
	return ret;
}