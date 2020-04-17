#include "mrp.h"

typedef struct
{
	char *bitbuf;
	char *filename; //字体文件名
	long f;			//文件指针
	long font_size; //字号
	long ansi_size; //ansi字符宽度
	int width;
	int height;
} FONT;

typedef struct
{
	char width[128];
	char height[128];

} FONT_ANSI; //所有ansi字符宽高

FONT_ANSI *font_ansi;

FONT *font_sky16;
char font_sky16_bitbuf[32];
int font_sky16_f;
int font_sky16_font_size = 16;
int font_sky16_ansi_size = 8;

//字体初始化，打开字体文件
int xl_font_sky16_init()
{
	logi("xl_font_sky16_init()");

	if (font_sky16_f)
		return 0;

	int id = 0;

	font_sky16 = malloc(sizeof(FONT));
	font_ansi = malloc(sizeof(FONT_ANSI));
	//font_sky16_filename = "system/gb16_mrpoid.uc2";

	font_sky16_f = mr_open("system/gb16_mrpoid.uc2", 0);
	//font_sky16_f= open(font_sky16_filename, O_RDONLY, 0777);
	//int f = open(LOGFILE,O_READ,0777);
	//lseek(f,0,2);
	//write(f,text,strlen(text));
	if (font_sky16_f <= 0)
	{
		logw("字体加载失败");
		return -1;
	}

	//gEmuEnv.androidDrawChar = FALSE;
	logi("字体加载成功 fd=%d", font_sky16_f);

	//font_sky16_bitbuf=malloc(32);
	while (id < 128)
	{
		font_ansi->width[id] = 8;
		font_ansi->height[id] = 16;
		id++;
	}
	font_sky16_font_size = 16;

	font_sky16_ansi_size = 8;

	return 0;
}

//关闭字体
int xl_font_sky16_close()
{
	logi("xl_font_sky16_close()");
	if (!font_sky16_f)
		return 0;
	return mr_close(font_sky16_f);
}

void dpoint(int x, int y, int color)
{
	if (x < 0 || x >= screen.width || y < 0 || y >= screen.height)
		return;
	*(mr_screenBuf + y * screen.width + x) = color;
}

//获取二进制缓存里指定像素的值
int xl_font_sky16_getfontpix(char *buf, int n)
{
	//计算在第几个字节，从0开始
	buf += n / 8;
	//计算在第几位n%8
	return (128 >> (n % 8)) & *buf;
}

//获取字体第n个点信息
int getfontpix(char *buf, int n)
{
	//计算在第几个字节，从0开始
	buf += n / 8;
	//计算在第几位n%8
	return (128 >> (n % 8)) & *buf;
}

//获得字符的位图
char *xl_font_sky16_getChar(uint16 id)
{
	logi("xl_font_sky16_getChar(%x)", id);
	mr_seek(font_sky16_f, id * 32, 0);
	mr_read(font_sky16_f, font_sky16_bitbuf, 32);
	return font_sky16_bitbuf;
}

//画一个字
char *xl_font_sky16_drawChar(uint16 id, int x, int y, uint16 color)
{
	//  printfs("drawChar 16 font");
	// font_sky16_f = mr_open(font_sky16_filename, 1);
	mr_seek(font_sky16_f, id * 32, 0);
	//memset(wordBuf, 0, 32);
	mr_read(font_sky16_f, font_sky16_bitbuf, 32);
	//	mr_close(font_sky16_f);
	//log_i("字体加载",(int)font_sky16_f);
	//log_i(font_sky16_filename,id);
	//int x2 = x+ font_sky16_font_size;
	int y2 = y + 16; //font_sky16_font_size;
	int n = 0;

	int ix = x;
	int iy;
	for (iy = y; iy < y2; iy++)
	{

		ix = x;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;

		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
		if (getfontpix(font_sky16_bitbuf, n))
			dpoint(ix, iy, color);
		n++;
		ix++;
	}
	return font_sky16_bitbuf;
}

//获得换行文本的高度
int xl_font_sky16_textWidthHeightLines(uint8 *pcText, uint16 showWidth, int32 *width, int32 *height, int32 *lines)
{
	return 0;
}

//获取一个文字的宽高
void xl_font_sky16_charWidthHeight(uint16 id, int32 *width, int32 *height)
{
	logi("xl_font_sky16_charWidthHeight(%x, %p, %p)", id, width, height);

	if (id < 128)
	{
		if (width)
		{
			*width = font_ansi->width[id];
		}
		if (height)
		{
			*height = font_ansi->height[id];
		}
	}
	else
	{
		if (width)
		{
			*width = 16;
		}
		if (height)
		{
			*height = 16;
		}
	}
}

//获取单行文字的宽高
void xl_font_sky16_textWidthHeight(char *text, int32 *width, int32 *height)
{
	int textIdx = 0;
	int id;
	int fontw = 0, fonth = 0;
	while (text[textIdx] != 0)
	{
		id = (text[textIdx] << 8) + (text[textIdx + 1]);
		xl_font_sky16_charWidthHeight(id, &fontw, &fonth);
		(*width) += fontw;
		(*height) += fonth;
		textIdx += 2;
	}
}