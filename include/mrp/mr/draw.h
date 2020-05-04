#ifndef __MRP_MR_DRAW_H__
#define __MRP_MR_DRAW_H__

#include "mrp/common/helper.h"

#define MR_SPRITE_INDEX_MASK (0x03FF) // mask of bits used for tile index
#define MR_SPRITE_TRANSPARENT (0x0400)
#define MR_TILE_SHIFT (11)
#define MR_ROTATE_0 (0)
#define MR_ROTATE_90 (1)
#define MR_ROTATE_180 (2)
#define MR_ROTATE_270 (3)

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

typedef struct mr_screenRect_st
{
	uint16 x;
	uint16 y;
	uint16 w;
	uint16 h;
} mr_screenRect_t;

typedef struct mr_colour_st
{
	uint8 r;
	uint8 g;
	uint8 b;
} mr_colour_t;

void swi_mr_drawRect(vm_info_t *vm);
void swi_mr_drawText(vm_info_t *vm);
void swi_mr_drawTextEx(vm_info_t *vm);
void swi_mr_refreshBuffer(vm_info_t *vm);
void swi_mr_drawPoint(vm_info_t *vm);
void swi_mr_getCharBitmap(vm_info_t *vm);
void swi_mr_platDrawChar(vm_info_t *vm);

void swi_mr_drawBitmap(vm_info_t *vm);

#endif