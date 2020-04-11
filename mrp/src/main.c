#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "mrp.h"

int32 GET_HOST_CB(int32 ip)
{
	printf("host got: %d\n", ip);

	int32 fd = mr_socket(MR_SOCK_STREAM, MR_IPPROTO_TCP);

	println("socket:%d", fd);

	int32 cres = mr_connect(fd, ip, 1201, NETTYPE_CMNET);
	println("connect: %d", cres);

	mr_send(fd, "yizhi", 5);

	char buf[1000] = {0};
	mr_recv(fd, buf, 1000);
	printf("recved: %s\n", buf);

	mr_closeSocket(fd);

	return MR_SUCCESS;
}

int32 net_inited(int32 result)
{
	printf("net work inited\n");
	int32 resh = mr_getHostByName("www.baidu.com", &GET_HOST_CB);
	println("hostname res:%d", resh);
	return 0;
}

int main(int argc, char **argv)
{
	//初始化时间
	gettimeofday(&dsmStartTime, null);

	//初始化手机信息
	strcpy(userInfo.IMEI, "123456789abcdef");
	strcpy(userInfo.IMSI, "aadfsd");
	strcpy(userInfo.manufactory, "yizhi");
	strcpy(userInfo.type, "mrpemu");

	//手机移动网络ID初始化
	networkID = MR_NET_ID_MOBILE;

	//初始化屏幕信息
	screen.width = 240;
	screen.height = 320;
	screen.bit = 24;

	//初始化允许内存
	memSize = 1024 * 1024 * 5;

	//初始化目录信息
	dsm_init("/sdcard/", "mythroad/");

	//字体初始化
	xl_font_sky16_init();

	//启动
	mr_start_dsm("%hello.mrp");

	//测试网络
	mr_initNetwork(&net_inited, "cmnet");

	return 0;
}