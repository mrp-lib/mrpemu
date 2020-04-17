#ifndef __MR_NETWORK_H__
#define __MR_NETWORK_H__

#include "common/type.h"

enum
{
	MR_IPPROTO_TCP,
	MR_IPPROTO_UDP
};

enum
{
	MR_SOCKET_BLOCK,   //阻塞方式（同步方式）
	MR_SOCKET_NONBLOCK //非阻塞方式（异步方式）
};

enum
{
	MR_SOCK_STREAM,
	MR_SOCK_DGRAM
};

enum
{
	MR_ENCODE_ASCII,
	MR_ENCODE_UNICODE
};

typedef struct
{
	int32 socket;
	int32 port;
} mr_bind_st;

typedef enum
{
	NETTYPE_WIFI = 0,
	NETTYPE_CMWAP = 1,
	NETTYPE_CMNET = 2,
	NETTYPE_UNKNOW = 3
} AND_NETTYPE;

typedef int32 (*MR_INIT_NETWORK_CB)(int32 result);
typedef int32 (*MR_GET_HOST_CB)(int32 ip);

typedef int32 (*T_mr_initNetwork)(MR_INIT_NETWORK_CB cb, const char *mode);
typedef int32 (*T_mr_closeNetwork)(void);
typedef int32 (*T_mr_getHostByName)(const char *name, MR_GET_HOST_CB cb);
typedef int32 (*T_mr_socket)(int32 type, int32 protocol);
typedef int32 (*T_mr_connect)(int32 s, int32 ip, uint16 port, int32 type);
typedef int32 (*T_mr_closeSocket)(int32 s);
typedef int32 (*T_mr_recv)(int32 s, char *buf, int len);
typedef int32 (*T_mr_recvfrom)(int32 s, char *buf, int len, int32 *ip, uint16 *port);
typedef int32 (*T_mr_send)(int32 s, const char *buf, int len);
typedef int32 (*T_mr_sendto)(int32 s, const char *buf, int len, int32 ip, uint16 port);

int32 mr_initNetwork(MR_INIT_NETWORK_CB cb, const char *mode);
int32 mr_closeNetwork(void);
int32 mr_getHostByName(const char *name, MR_GET_HOST_CB cb);
int32 mr_socket(int32 type, int32 protocol);
int32 mr_connect(int32 s, int32 ip, uint16 port, int32 type);
int32 mr_closeSocket(int32 s);
int32 mr_recv(int32 s, char *buf, int len);
int32 mr_recvfrom(int32 s, char *buf, int len, int32 *ip, uint16 *port);
int32 mr_send(int32 s, const char *buf, int len);
int32 mr_sendto(int32 s, const char *buf, int len, int32 ip, uint16 port);

#endif