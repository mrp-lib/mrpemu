#ifndef __HELPER_SWI_H__
#define __HELPER_SWI_H__

#define SWI_MR_MALLOC 0x1001
#define SWI_MR_FREE 0x1002
#define SWI_MR_REALLOC 0x1003

#define SWI_MR_MEMCPY 0x1011
#define SWI_MR_MEMMOVE 0x1012
#define SWI_MR_STRCPY 0x1013
#define SWI_MR_STRNCPY 0x1014
#define SWI_MR_STRCAT 0x1015
#define SWI_MR_STRNCAT 0x1016
#define SWI_MR_MEMCMP 0x1017
#define SWI_MR_STRCMP 0x1018
#define SWI_MR_STRNCMP 0x1019
#define SWI_MR_STRCOLL 0x101a
#define SWI_MR_MEMCHR 0x101b
#define SWI_MR_MEMSET 0x101c
#define SWI_MR_STRLEN 0x101d
#define SWI_MR_STRSTR 0x101e
#define SWI_MR_SPRINTF 0x101f
#define SWI_MR_ATOI 0x1020
#define SWI_MR_STRTOUL 0x1021
#define SWI_MR_RAND 0x1022

#define SWI_MR_STOP_EX 0x1030
#define SWI_MR_C_FUNCTION_NEW 0x1031

#define SWI_MR_PRINTF 0x1041
#define SWI_MR_MEM_GET 0x1042
#define SWI_MR_MEM_FREE 0x1043
#define SWI_MR_DRAWBITMAPA 0x1044	//TODO这个和下面有冲突？？？？？先改个名字吧，额
#define SWI_MR_GETCHARBITMAP 0x1045
#define SWI_MR_TIMERSTART 0x1046
#define SWI_MR_TIMERSTOP 0x1047
#define SWI_MR_GETTIME 0x1048
#define SWI_MR_GETDATETIME 0x1049
#define SWI_MR_GETUSERINFO 0x104a
#define SWI_MR_SLEEP 0x104b

#define SWI_MR_PLAT 0x1051
#define SWI_MR_PLATEX 0x1052

#define SWI_MR_FERRNO 0x1061
#define SWI_MR_OPEN 0x1062
#define SWI_MR_CLOSE 0x1063
#define SWI_MR_INFO 0x1064
#define SWI_MR_WRITE 0x1065
#define SWI_MR_READ 0x1066
#define SWI_MR_SEEK 0x1067
#define SWI_MR_GETLEN 0x1068
#define SWI_MR_REMOVE 0x1069
#define SWI_MR_RENAME 0x106a
#define SWI_MR_MKDIR 0x106b
#define SWI_MR_RMDIR 0x106c
#define SWI_MR_FINDSTART 0x106d
#define SWI_MR_FINDGETNEXT 0x106e
#define SWI_MR_FINDSTOP 0x106f

#define SWI_MR_EXIT 0x1081
#define SWI_MR_STARTSHAKE 0x1082
#define SWI_MR_STOPSHAKE 0x1083
#define SWI_MR_PLAYSOUND 0x1084
#define SWI_MR_STOPSOUND 0x1085

#define SWI_MR_SENDSMS 0x109
#define SWI_MR_CALL 0x109
#define SWI_MR_GETNETWORKID 0x109
#define SWI_MR_CONNECTWAP 0x109

#define SWI_MR_MENUCREATE 0x10a1
#define SWI_MR_MENUSETITEM 0x10a2
#define SWI_MR_MENUSHOW 0x10a3
#define SWI_MR_MENURELEASE 0x10a5
#define SWI_MR_MENUREFRESH 0x10a6

#define SWI_MR_DIALOGCREATE 0x10a9
#define SWI_MR_DIALOGRELEASE 0x10aa
#define SWI_MR_DIALOGREFRESH 0x10ab

#define SWI_MR_TEXTCREATE 0x10ae
#define SWI_MR_TEXTRELEASE 0x10af
#define SWI_MR_TEXTREFRESH 0x10b1

#define SWI_MR_EDITCREATE 0x10b4
#define SWI_MR_EDITRELEASE 0x10b5
#define SWI_MR_EDITGETTEXT 0x10b6

#define SWI_MR_WINCREATE 0x10b9
#define SWI_MR_WINRELEASE 0x10ba

#define SWI_MR_GETSCREENINFO 0x10bd

#define SWI_MR_INITNETWORK 0x10c1
#define SWI_MR_CLOSENETWORK 0x10c2
#define SWI_MR_GETHOSTBYNAME 0x10c3
#define SWI_MR_SOCKET 0x10c4
#define SWI_MR_CONNECT 0x10c5
#define SWI_MR_CLOSESOCKET 0x10c6
#define SWI_MR_RECV 0x10c7
#define SWI_MR_RECVFROM 0x10c8
#define SWI_MR_SEND 0x10c9
#define SWI_MR_SENDTO 0x10ca

#define SWI_MR_MD5_INIT 0x10d1
#define SWI_MR_MD5_APPEND 0x10d2
#define SWI_MR_MD5_FINISH 0x10d3
#define SWI_MR_LOAD_SMS_CFG 0x10d7
#define SWI_MR_SAVE_SMS_CFG 0x10d8

#define SWI_MR_DISPUPEX 0x10e1
#define SWI_MR_DRAWPOINT 0x10e2
#define SWI_MR_DRAWBITMAP 0x10e3
#define SWI_MR_DRAWBITMAPEX 0x10e4
#define SWI_MR_DRAWRECT 0x10e5
#define SWI_MR_DRAWTEXT 0x10e6
#define SWI_MR_BITMAPCHECK 0x10e7

#define SWI_MR_READFILE 0x10f1
#define SWI_MR_WSTRLEN 0x10f2
#define SWI_MR_REGISTERAPP 0x10f3
#define SWI_MR_DRAWTEXTEX 0x10f4
#define SWI_MR_EFFSETCON 0x10f5
#define SWI_MR_TESTCOM 0x10f6
#define SWI_MR_TESTCOM1 0x10f7
#define SWI_MR_C2U 0x10f8
#define SWI_MR_DIV 0x10f9
#define SWI_MR_MOD 0x10fa

#define SWI_MR_UPDCRC 0x110
#define SWI_MR_UNZIP 0x110
#define SWI_MR_PLATDRAWCHAR 0x110
#define SWI_MR_TRANSBITMAPDRAW 0x110
#define SWI_MR_DRAWREGION 0x110

#define DATAP_RESERVE 0

#define DATAP_C_INTERNAL_TABLE -1
#define DATAP_C_PORT_TABLE -2
#define DATAP_LG_MEM_MIN -3
#define DATAP_LG_MEM_TOP -4
#define DATAP_START_FILE_PARAMETER -5
#define DATAP_SMS_RETURN_FLAG -6
#define DATAP_SMS_RETURN_VAL -7
#define DATAP_EXIT_CB -8
#define DATAP_EXIT_CB_DATA -9
#define DATAP_ENTRY -10
#define DATAP_LG_MEM_FREE -11
#define DATAP_SCREENBUF -12
#define DATAP_SCREENWIDTH -13
#define DATAP_SCREENHEIGHT -14
#define DATAP_SCREENBIT -15
#define DATAP_BITMAP -16
#define DATAP_TILE -17
#define DATAP_MAP -18
#define DATAP_SOUND -19
#define DATAP_SPRITE -20
#define DATAP_PACK_FILENAME -21
#define DATAP_START_FILENAME -22
#define DATAP_OLD_PACK_FILENAME -23
#define DATAP_OLD_START_FILENAME -24
#define DATAP_RAM_FILE -25
#define DATAP_RAM_FILE_LEN -26
#define DATAP_SOUNDON -27
#define DATAP_SHAKEON -28
#define DATAP_LG_MEM_BASE -29
#define DATAP_LG_MEM_LEN -30
#define DATAP_LG_MEM_END -31
#define DATAP_LG_MEM_LEFT -32
#define DATAP_SMS_CFG_BUF -33

#endif