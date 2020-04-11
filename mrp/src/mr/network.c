#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#include "mrp.h"

//对mr的socket进行转换，因为值的定义是不同的。
#define SOCKET_TYPE_FROM_MR(type) ((type == MR_SOCK_STREAM) ? SOCK_STREAM : SOCK_DGRAM)

int32 mr_initNetwork(MR_INIT_NETWORK_CB cb, const char *mode)
{
	//默认是cmnet
	if (mode == NULL)
		mode = "cmnet";

	//如果有回调函数表示异步通知
	if (cb != NULL)
	{
		cb(MR_SUCCESS);
		return MR_WAITING;
	}
	return MR_SUCCESS;
}

int32 mr_closeNetwork(void)
{
	return MR_SUCCESS;
}

int32 mr_getHostByName(const char *ptr, MR_GET_HOST_CB cb)
{
	struct hostent *hptr = gethostbyname(ptr);

	//没有主机名
	if (hptr == NULL)
		return MR_FAILED;

	//解析失败
	if (hptr->h_addrtype != AF_INET && hptr->h_addrtype != AF_INET6)
		return MR_FAILED;

	// char **pptr = hptr->h_addr_list;
	// while (*pptr != NULL)
	// {
	// 	char str[64] = {0};
	// 	inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str));
	// 	printf("IP: %s\n", str);
	// 	pptr++;
	// }

	//得到IP
	int32 result;
	memcpy(&result, hptr->h_addr, 4);
	int32 ip = ntohl(result);

	//返回结果
	if (cb != NULL)
		cb(ip);
	return ip;
}

int32 mr_socket(int32 type, int32 protocol)
{
	int32 sock_type = SOCKET_TYPE_FROM_MR(type);
	int32 mtk_protocol_type = 0;

	//创建socket
	int32 fd = socket(AF_INET, sock_type, 0);
	if (fd < 0)
		return MR_FAILED;

	return fd;
}

int32 mr_connect(int32 s, int32 ip, uint16 port, int32 type)
{
	struct sockaddr_in addr;

	//初始化地址
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);

	//连接到socket
	int32 ret = connect(s, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	if (ret < 0)
		return MR_FAILED;

	// TODO
	// 可以的话还应该处理一下阻塞模式

	return MR_SUCCESS;
}

int32 mr_closeSocket(int32 s)
{
	return close(s);
	// return MR_SUCCESS;
}

int32 mr_recv(int32 s, char *buf, int len)
{
	int32 read = recv(s, (void *)buf, len, 0);
	if (read > 0)
	{
		return read;
	}
	else if (read < 0 && errno == EWOULDBLOCK)
	{
		return 0;
	}
	else
	{
		return MR_FAILED;
	}
}

int32 mr_recvfrom(int32 s, char *buf, int len, int32 *ip, uint16 *port)
{
	struct sockaddr_in fromaddr;
	socklen_t addr_len;
	int32 read = recvfrom(s, buf, len, 0, (struct sockaddr *)&fromaddr, &addr_len);
	if (read >= 0)
	{
		return read;
	}
	else if (read < 0 && errno == EWOULDBLOCK)
	{
		return 0;
	}
	else
	{
		return MR_FAILED;
	}
}

int32 mr_send(int32 s, const char *buf, int len)
{
	int32 write = send(s, (void *)buf, len, 0);
	if (write >= 0)
	{
		return write;
	}
	else if (write < 0 && errno == EWOULDBLOCK)
	{
		return 0;
	}
	else
	{
		return MR_FAILED;
	}
}

int32 mr_sendto(int32 s, const char *buf, int len, int32 ip, uint16 port)
{
	struct sockaddr_in toaddr;
	memset(&toaddr, 0, sizeof(toaddr));
	toaddr.sin_family = AF_INET;
	toaddr.sin_port = htons(port);
	toaddr.sin_addr.s_addr = htonl(ip);

	int32 write = sendto(s, buf, len, 0, (struct sockaddr *)&toaddr, sizeof(struct sockaddr));
	if (write >= 0)
	{
		return write;
	}
	else if (write < 0 && errno == EWOULDBLOCK)
	{
		return 0;
	}
	else
		return MR_FAILED;
}