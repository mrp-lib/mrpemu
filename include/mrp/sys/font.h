#ifndef __MRP_FRONT_H__
#define __MRP_FRONT_H__

#include "common/type.h"
#include "mrp/common/helper.h"

typedef struct font_info_st
{
	uint8 *buf;			 //文件数据
	uint32 len;			 //数据长度
	uint32 size;		 //字体大小
	uint32 chlen;		 //单个字符占的位数
	uint8 bitmapBuf[64]; //点阵缓存，取得点阵信息后缓存在这里
} font_info_t;

/*
初始化字体，提供外部访问，可以在程序启动之前初始化字体
初始化之后字体信息会被存放在虚拟机中，在虚拟机销毁时一并销毁
参数：
	vm		虚拟机
*/
int32 font_init(vm_info_t *vm);

/*
释放字体
*/
void font_free(vm_info_t *vm);

/*
获取文字的位图信息，这个位图信息的地址是固定的，也就是font_info_t.bitmapBuffer的地址
参数：
	vm		虚拟机
	ch		Unicode字符
*/
uint8 *font_get_bitmap(vm_info_t *vm, uint16 ch);

/*
获取字符宽度，字符的高度和字体大小一致
参数：
	vm		虚拟机
	ch		Unicode字符
*/
int32 font_ch_width(vm_info_t *vm, uint16 ch);

/*
获取文本宽度，高度和字体高度一致
参数：
	vm		虚拟机
	str		Unicode字符串
*/
int32 font_text_width(vm_info_t *vm, uint16 *str);

/*
绘制字符
参数：
	x 	横坐标
	y	纵坐标
	ch	Unicode字符
*/
int32 font_ch_draw(vm_info_t *vm, int32 x, int32 y, uint16 ch, uint16 color);

#endif