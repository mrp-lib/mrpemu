#ifndef __MR_DRAW_H__
#define __MR_DRAW_H__

#include "common/type.h"

#define MAKERGB(r, g, b) (uint16)(((uint32)(r >> 3) << 11) + ((uint32)(g >> 2) << 5) + ((uint32)(b >> 3)))

#define MR_SPRITE_INDEX_MASK (0x03FF) // mask of bits used for tile index
#define MR_SPRITE_TRANSPARENT (0x0400)

#define MR_TILE_SHIFT (11)

#define MR_ROTATE_0 (0)
#define MR_ROTATE_90 (1)
#define MR_ROTATE_180 (2)
#define MR_ROTATE_270 (3)

#define DRAW_TEXT_EX_IS_UNICODE 1
#define DRAW_TEXT_EX_IS_AUTO_NEWLINE 2

#define TOP_Graphics 16
#define BASELINE_Graphics 64
#define BOTTOM_Graphics 32
#define VCENTER_Graphics 0x2

#define LEFT_Graphics 0x4
#define HCENTER_Graphics 0x1
#define RIGHT_Graphics 0x8

#define AP_V_MASK_Graphics (TOP_Graphics | BASELINE_Graphics | BOTTOM_Graphics | VCENTER_Graphics)
#define AP_H_MASK_Graphics (LEFT_Graphics | HCENTER_Graphics | RIGHT_Graphics)

#define TRANS_NONE_Sprite 0
#define TRANS_MIRROR_ROT180_Sprite 1
#define TRANS_MIRROR_Sprite 2
#define TRANS_ROT180_Sprite 3

#define TRANS_MIRROR_ROT270_Sprite 4
#define TRANS_ROT90_Sprite 5
#define TRANS_ROT270_Sprite 6
#define TRANS_MIRROR_ROT90_Sprite 7

enum
{
	BM_OR,			//SRC .OR. DST*   半透明效果
	BM_XOR,			//SRC .XOR. DST*
	BM_COPY,		//DST = SRC*      覆盖
	BM_NOT,			//DST = (!SRC)*
	BM_MERGENOT,	//DST .OR. (!SRC)
	BM_ANDNOT,		//DST .AND. (!SRC)
	BM_TRANSPARENT, //透明色不显示，图片的第一个象素（左上角的象素）是透明色
	BM_AND,
	BM_GRAY,
	BM_REVERSE
};

typedef struct _tagTransBitmap
{
	void *pData;
	int32 width;
	int32 height;
	int32 maxWidth;
	uint16 transColor;

	int16 *pMatrix;
	int32 markCount;
	uint8 *pZion;
} mr_transBitmap;

typedef struct _mr_jgraphics_context_t
{
	struct
	{

		/*
         *  Clip 变量： 坐标值均为经过translate处理过的。
         *  相对于 ScreenBuffer的坐标。
         */
		int clipX;
		int clipY;
		int clipWidth;
		int clipHeight;
		int clipXRight;	 /* = clipX + clipWidth， 为了速度而设置的冗余值 */
		int clipYBottom; /* = clipY + clipBottom，为了速度而设置的冗余值 */

		int translateX;
		int translateY;

		int font;

		uint8 color_R, color_G, color_B;
		uint16 color_565;
	} mutableValues;

	uint16 *__SJC_SCREEN_BUFFER__;		//假的屏幕缓冲(缓存)
	int __SJC_SCREEN_WIDTH__;			//假的屏幕宽(缓存宽)
	int __SJC_SCREEN_HEIGHT__;			//假的屏幕高(缓存高)
	int flag;							//标识是否是屏幕buffer，0-否，1-是
	uint16 *__REAL_SJC_SCREEN_BUFFER__; //真正的屏幕冲缓
	int __REAL_SJC_SCREEN_WIDTH__;		//真正的屏幕宽
	int __REAL_SJC_SCREEN_HEIGHT__;		//真正的屏幕高
} mr_jgraphics_context_t;

typedef struct
{
	uint16 *data;
	uint16 width;
	uint16 height;
	uint8 trans; //1:使用透明色;0:不使用
	uint16 transcolor;
} mr_jImageSt;

typedef struct
{
	uint16 *p;
	uint16 w;
	uint16 h;
	uint16 x;
	uint16 y;
} mr_bitmapDrawSt;

typedef struct
{
	int16 A; // A, B, C, and D are fixed point values with an 8-bit integer part
	int16 B; // and an 8-bit fractional part.
	int16 C;
	int16 D;
	uint16 rop;
} mr_transMatrixSt;

typedef struct
{
	uint16 x;
	uint16 y;
	uint16 w;
	uint16 h;
} mr_screenRectSt;

typedef struct
{
	uint8 r;
	uint8 g;
	uint8 b;
} mr_colourSt;

typedef void (*T_mr_drawBitmap)(uint16 *bmp, int16 x, int16 y, uint16 w, uint16 h);
typedef const char *(*T_mr_getCharBitmap)(uint16 ch, uint16 fontSize, int *width, int *height);

typedef int32 (*T__DispUpEx)(int16 x, int16 y, uint16 w, uint16 h);
typedef void (*T__DrawPoint)(int16 x, int16 y, uint16 nativecolor);
typedef void (*T__DrawBitmap)(uint16 *p, int16 x, int16 y, uint16 w, uint16 h, uint16 rop, uint16 transcoler, int16 sx, int16 sy, int16 mw);
typedef void (*T__DrawBitmapEx)(mr_bitmapDrawSt *srcbmp, mr_bitmapDrawSt *dstbmp, uint16 w, uint16 h, mr_transMatrixSt *pTrans, uint16 transcoler);
typedef void (*T_DrawRect)(int16 x, int16 y, int16 w, int16 h, uint8 r, uint8 g, uint8 b);
typedef int32 (*T__DrawText)(char *pcText, int16 x, int16 y, uint8 r, uint8 g, uint8 b, int is_unicode, uint16 font);
typedef int32 (*T__DrawTextEx)(char *pcText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst, int flag, uint16 font);
typedef int32 (*T__mr_EffSetCon)(int16 x, int16 y, int16 w, int16 h, int16 perr, int16 perg, int16 perb);

extern uint16 *mr_screenBuf;

void mr_drawBitmap(uint16 *bmp, int16 x, int16 y, uint16 w, uint16 h);

const char *mr_getCharBitmap(uint16 ch, uint16 fontSize, int *width, int *height);

int32 _DispUpEx(int16 x, int16 y, uint16 w, uint16 h);
void _DrawPoint(int16 x, int16 y, uint16 nativecolor);
void _DrawBitmap(uint16 *p, int16 x, int16 y, uint16 w, uint16 h, uint16 rop, uint16 transcoler, int16 sx, int16 sy, int16 mw);
void _DrawBitmapEx(mr_bitmapDrawSt *srcbmp, mr_bitmapDrawSt *dstbmp, uint16 w, uint16 h, mr_transMatrixSt *pTrans, uint16 transcoler);
void DrawRect(int16 x, int16 y, int16 w, int16 h, uint8 r, uint8 g, uint8 b);
int32 _DrawText(char *pcText, int16 x, int16 y, uint8 r, uint8 g, uint8 b, int is_unicode, uint16 font);
int32 _DrawTextEx(char *pcText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst, int flag, uint16 font);
typedef int (*T__BitmapCheck)(uint16 *p, int16 x, int16 y, uint16 w, uint16 h, uint16 transcoler, uint16 color_check);
typedef void (*T_mr_platDrawChar)(uint16 ch, int32 x, int32 y, int32 color);
int32 _mr_EffSetCon(int16 x, int16 y, int16 w, int16 h, int16 perr, int16 perg, int16 perb);
int _BitmapCheck(uint16 *p, int16 x, int16 y, uint16 w, uint16 h, uint16 transcoler, uint16 color_check);
void mr_platDrawChar(uint16 ch, int32 x, int32 y, int32 color);

int32 mr_transbitmapDraw(mr_transBitmap *hTransBmp, uint16 *dstBuf, int32 dest_max_w, int32 dest_max_h, int32 sx, int32 sy, int32 width, int32 height, int32 dx, int32 dy);
void mr_drawRegion(mr_jgraphics_context_t *gContext, mr_jImageSt *src, int sx, int sy, int w, int h, int transform, int x, int y, int anchor);

#endif