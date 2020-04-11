#include "mrp.h"

#define MR_SCREEN_CACHE_POINT(x, y) (mr_screenBuf + y * screen.width + x)

uint16 *mr_screenBuf;

void mr_drawBitmap(uint16 *bmp, int16 x, int16 y, uint16 w, uint16 h)
{
	logi("mr_drawBitmap(%p, %d, %d, %d, %d)", bmp, x, y, w, h);
	//TODO
	// emu_bitmapToscreen(bmp, x, y, w, h);
}

const char *mr_getCharBitmap(uint16 ch, uint16 fontSize, int *width, int *height)
{
	logi("mr_getCharBitmap(%d, %d, %p, %p)", ch, fontSize, width, height);
	xl_font_sky16_charWidthHeight(ch, width, height);
	return xl_font_sky16_getChar(ch);
}

int32 _DispUpEx(int16 x, int16 y, uint16 w, uint16 h)
{
	logi("_DispUpEx(%d,%d,%d,%d)", x, y, w, h);

	if (1 == vm_state)
		mr_drawBitmap(mr_screenBuf, x, y, w, h);

	return 0;
}

void _DrawPoint(int16 x, int16 y, uint16 nativecolor)
{
	if (x < 0 || y < 0 || x >= screen.width || y >= screen.height)
		return;
	*MR_SCREEN_CACHE_POINT(x, y) = nativecolor;
}

void _DrawBitmap(uint16 *p, int16 x, int16 y, uint16 w, uint16 h, uint16 rop, uint16 transcoler, int16 sx, int16 sy, int16 mw)
{
	uint16 *dstp, *srcp;
	int MaxY = MIN(screen.height, y + h);
	int MaxX = MIN(screen.width, x + w);
	int MinY = MAX(0, y);
	int MinX = MAX(0, x);
	uint16 dx, dy;

	if (rop > MR_SPRITE_TRANSPARENT)
	{
		uint16 BitmapRop = rop & MR_SPRITE_INDEX_MASK;
		uint16 BitmapMode = (rop >> MR_TILE_SHIFT) & 0x3;
		uint16 BitmapFlip = (rop >> MR_TILE_SHIFT) & 0x4;
		switch (BitmapRop)
		{
		case BM_TRANSPARENT:
			for (dy = MinY; dy < MaxY; dy++)
			{
				//dstp = mr_screenBuf + dy * MR_SCREEN_MAX_W + MinX;
				dstp = MR_SCREEN_CACHE_POINT(MinX, dy);
				srcp = p + (dy - y) * w + (MinX - x);
				for (dx = MinX; dx < MaxX; dx++)
				{
					if (*srcp != transcoler)
						*dstp = *srcp;
					dstp++;
					srcp++;
				}
			}
			break;
		case BM_COPY:
			switch (BitmapMode)
			{
			case MR_ROTATE_0:
				if (MaxX > MinX)
				{
					for (dy = MinY; dy < MaxY; dy++)
					{
						//dstp = mr_screenBuf + dy * MR_SCREEN_MAX_W + MinX;
						dstp = MR_SCREEN_CACHE_POINT(MinX, dy);
						srcp = BitmapFlip ? p + (h - 1 - (dy - y)) * w + (MinX - x) : p + (dy - y) * w + (MinX - x);
						memcpy(dstp, srcp, (MaxX - MinX) << 1);
					}
				}
				break;
			case MR_ROTATE_90:
				for (dy = MinY; dy < MaxY; dy++)
				{
					dstp = MR_SCREEN_CACHE_POINT(MinX, dy);
					srcp = BitmapFlip ? p + (h - 1 - (MinX - x)) * w + (w - 1 - (dy - y)) : p + (MinX - x) * w + (w - 1 - (dy - y));
					for (dx = MinX; dx < MaxX; dx++)
					{
						*dstp = *srcp;
						dstp++;
						srcp = BitmapFlip ? srcp - w : srcp + w;
					}
				}
				break;
			case MR_ROTATE_180:
				for (dy = MinY; dy < MaxY; dy++)
				{
					dstp = MR_SCREEN_CACHE_POINT(MinX, dy);
					srcp = BitmapFlip ? p + (dy - y) * w + (w - 1 - (MinX - x)) : p + (h - 1 - (dy - y)) * w + (w - 1 - (MinX - x));
					for (dx = MinX; dx < MaxX; dx++)
					{
						*dstp = *srcp;
						dstp++;
						srcp--;
					}
				}
				break;
			case MR_ROTATE_270:
				for (dy = MinY; dy < MaxY; dy++)
				{
					dstp = MR_SCREEN_CACHE_POINT(MinX, dy);
					srcp = BitmapFlip ? p + (MinX - x) * w + (dy - y) : p + (h - 1 - (MinX - x)) * w + (dy - y);
					for (dx = MinX; dx < MaxX; dx++)
					{
						*dstp = *srcp;
						dstp++;
						srcp = BitmapFlip ? srcp + w : srcp - w;
					}
				}
				break;
			}
		}
	}
	else
	{
		switch (rop)
		{
		case BM_TRANSPARENT:
			for (dy = MinY; dy < MaxY; dy++)
			{
				dstp = MR_SCREEN_CACHE_POINT(MinX, dy);
				srcp = p + (dy - y + sy) * mw + (MinX - x + sx);
				for (dx = MinX; dx < MaxX; dx++)
				{
					if (*srcp != transcoler)
						*dstp = *srcp;
					//mr_platDrawCharReal(1,0,0,0);
					dstp++;
					srcp++;
				}
			}
			break;
		case BM_COPY:
			if (MaxX > MinX)
			{
				for (dy = MinY; dy < MaxY; dy++)
				{
					dstp = MR_SCREEN_CACHE_POINT(MinX, dy);
					srcp = p + (dy - y + sy) * mw + (MinX - x + sx);
					memcpy(dstp, srcp, (MaxX - MinX) << 1);
				}
			}
			break;
		case BM_GRAY:
		case BM_OR:
		case BM_XOR:
		case BM_NOT:
		case BM_MERGENOT:
		case BM_ANDNOT:
		case BM_AND:
		case BM_REVERSE:
			for (dy = MinY; dy < MaxY; dy++)
			{
				dstp = MR_SCREEN_CACHE_POINT(MinX, dy);
				srcp = p + (dy - y + sy) * mw + (MinX - x + sx);
				for (dx = MinX; dx < MaxX; dx++)
				{
					switch (rop)
					{
					case BM_GRAY:
						if (*srcp != transcoler)
						{
							uint32 color_old = *srcp;
							uint32 r, g, b;
							// #ifdef MR_SCREEN_CACHE_BITMAP
							//r = ((color_old & 0x7c00) >> 10);
							//g = ((color_old & 0x3e0) >> 5);
							//b = ((color_old & 0x1f));
							// #else
							r = ((color_old & 0xf800) >> 11);
							g = ((color_old & 0x7e0) >> 6);
							b = ((color_old & 0x1f));
							// #endif
							r = (r * 60 + g * 118 + b * 22) / 25;
							*dstp = MAKERGB(r, r, r);
						}
						break;
					case BM_REVERSE:
						if (*srcp != transcoler)
						{
							*dstp = ~*srcp;
						}
						break;
					case BM_OR:
						*dstp = (*srcp) | (*dstp);
						break;
					case BM_XOR:
						*dstp = (*srcp) ^ (*dstp);
						break;
					case BM_NOT:
						*dstp = ~(*srcp);
						break;
					case BM_MERGENOT:
						*dstp = (~*srcp) | (*dstp);
						break;
					case BM_ANDNOT:
						*dstp = (~*srcp) & (*dstp);
						break;
					case BM_AND:
						*dstp = (*srcp) & (*dstp);
						break;
					}
					dstp++;
					srcp++;
				}
			}
			break;
		}
	}
}

void _DrawBitmapEx(mr_bitmapDrawSt *srcbmp, mr_bitmapDrawSt *dstbmp, uint16 w, uint16 h, mr_transMatrixSt *pTrans, uint16 transcoler)
{
	int32 A = pTrans->A;
	int32 B = pTrans->B;
	int32 C = pTrans->C;
	int32 D = pTrans->D;
	//uint16 rop = pTrans->rop;
	uint16 *dstp, *srcp;
	int16 CenterX = dstbmp->x + w / 2;
	int16 CenterY = dstbmp->y + h / 2;
	int32 dx, dy;
	int32 I = A * D - B * C;
	int16 MaxY = (ABS(C) * w + ABS(D) * h) >> 9;
	int16 MinY = 0 - MaxY;

	MaxY = MIN(MaxY, dstbmp->h - CenterY);
	MinY = MAX(MinY, 0 - CenterY);

	for (dy = MinY; dy < MaxY; dy++)
	{
		int16 MaxX = (int16)MIN(D == 0 ? 999 : (MAX((((w * I) >> 9) + B * dy) / D, (B * dy - ((w * I) >> 9)) / D)),
								C == 0 ? 999 : (MAX((A * dy + ((h * I) >> 9)) / C, (A * dy - ((h * I) >> 9)) / C)));
		int16 MinX = (int16)MAX(D == 0 ? -999 : (MIN((B * dy - ((w * I) >> 9)) / D, (((w * I) >> 9) + B * dy) / D)),
								C == 0 ? -999 : (MIN((A * dy - ((h * I) >> 9)) / C, (A * dy + ((h * I) >> 9)) / C)));
		MaxX = MIN(MaxX, dstbmp->w - CenterX);
		MinX = MAX(MinX, 0 - CenterX);
		dstp = dstbmp->p + (dy + CenterY) * dstbmp->w + (MinX + CenterX);
		switch (pTrans->rop)
		{
		case BM_TRANSPARENT:
			for (dx = MinX; dx < MaxX; dx++)
			{
				int32 offsety = ((A * dy - C * dx) << 8) / I + h / 2;
				int32 offsetx = ((D * dx - B * dy) << 8) / I + w / 2;
				if (((offsety < h) && (offsety >= 0)) && ((offsetx < w) && (offsetx >= 0)))
				{
					srcp = srcbmp->p + (offsety + srcbmp->y) * srcbmp->w + (offsetx + srcbmp->x);
					if (*srcp != transcoler)
						*dstp = *srcp;
				}
				dstp++;
			}
			break;
		case BM_COPY:
			for (dx = MinX; dx < MaxX; dx++)
			{
				int32 offsety = ((A * dy - C * dx) << 8) / I + h / 2;
				int32 offsetx = ((D * dx - B * dy) << 8) / I + w / 2;
				if (((offsety < h) && (offsety >= 0)) && ((offsetx < w) && (offsetx >= 0)))
				{
					srcp = srcbmp->p + (offsety + srcbmp->y) * srcbmp->w + (offsetx + srcbmp->x);
					*dstp = *srcp;
				}
				dstp++;
			}
			break;
		}
	}
}

void DrawRect(int16 x, int16 y, int16 w, int16 h, uint8 r, uint8 g, uint8 b)
{
	uint16 *dstp, *srcp;
	int MaxY = MIN(screen.height, y + h);
	int MaxX = MIN(screen.width, x + w);
	int MinY = MAX(0, y);
	int MinX = MAX(0, x);
	uint16 dx, dy;
	uint16 nativecolor;

	nativecolor = MAKERGB(r, g, b);

	if ((MaxY > MinY) && (MaxX > MinX))
	{
		dstp = MR_SCREEN_CACHE_POINT(MinX, MinY);
		srcp = dstp;
		for (dx = MinX; dx < MaxX; dx++)
		{
			*dstp = nativecolor;
			dstp++;
		}

		if (((uint32)srcp & 0x00000003) != 0)
		{
			srcp++;
			for (dy = MinY + 1; dy < MaxY; dy++)
			{
				dstp = MR_SCREEN_CACHE_POINT(MinX, dy);
				*dstp = nativecolor;
				dstp++;
				memcpy(dstp, srcp, (MaxX - MinX - 1) << 1);
			}
		}
		else
		{
			for (dy = MinY + 1; dy < MaxY; dy++)
			{
				dstp = MR_SCREEN_CACHE_POINT(MinX, dy);
				memcpy(dstp, srcp, (MaxX - MinX) << 1);
			}
		}
	}
}

int32 _DrawText(char *pcText, int16 x, int16 y, uint8 r, uint8 g, uint8 b, int is_unicode, uint16 font)
{
	int32 len;
	uint8 *p;
	uint16 ch;
	uint16 color = MAKERGB(r, g, b);
	int fw, fh, sx;

	logi("_DrawText(%s,%d,%d,%d,%d)", pcText, x, y, is_unicode, font);

	if (0 == is_unicode)
	{
		pcText = (char *)mr_c2u(pcText, NULL, &len);
	}

	sx = x;
	p = (uint8 *)pcText;
	while (*p || *(p + 1))
	{
		ch = (uint16)((*p << 8) + *(p + 1));

		mr_getCharBitmap(ch, font, &fw, &fh);
		mr_platDrawChar(ch, sx, y, color);

		sx += fw;
		p += 2;

		if (sx > screen.width)
			break;
	}

	if (0 == is_unicode)
		mr_free(pcText, len);

	return MR_SUCCESS;
}

int32 _DrawTextEx(char *pcText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst, int flag, uint16 font)
{
	int isUnicode, autoNewline;
	int32 len;
	uint8 *p;
	uint16 ch;
	uint16 color = MAKERGB(colorst.r, colorst.g, colorst.b);
	int fw, fh, sx, sy;
	int l, t, r, b;

	logi("_DrawTextEx(%s, %d, %d, %d, %d)", pcText, x, y, flag, font);

	isUnicode = flag & DRAW_TEXT_EX_IS_UNICODE;
	autoNewline = flag & DRAW_TEXT_EX_IS_AUTO_NEWLINE;

	if (!isUnicode)
	{
		pcText = (char *)mr_c2u(pcText, NULL, &len);
	}

	l = MAX(0, rect.x);
	t = MAX(0, rect.y);
	r = MIN(screen.width - 1, x + rect.w - 1);
	b = MIN(screen.height - 1, y + rect.h - 1);
	sx = x;
	sy = y;
	p = (uint8 *)pcText;
	while (*p || *(p + 1))
	{
		ch = (uint16)((*p << 8) + *(p + 1));

		mr_getCharBitmap(ch, font, &fw, &fh);

		if (sx < l || sx + fw > r || sy < t || sy > b)
			goto next;

		mr_platDrawChar(ch, sx, sy, color);

	next:
		p += 2;
		if (autoNewline && (sx + fw > r))
		{
			sx = l;
			sy += fh;
			if (sy > b)
				break;
		}
		else
		{
			sx += fw;
			if (sx > r)
				break;
		}
	}

	if (0 == isUnicode)
		mr_free(pcText, len);

	return MR_SUCCESS;
}

void mr_platDrawCharReal(uint16 ch, int32 x, int32 y, uint16 color)
{
	xl_font_sky16_drawChar(ch, x, y, color);
}

void mr_platDrawChar(uint16 ch, int32 x, int32 y, int32 color)
{
	logi("mr_platDrawChar(%x,%d,%d,%04x)", ch, x, y, color);
	mr_platDrawCharReal(ch, x, y, color);
}

int _BitmapCheck(uint16 *p, int16 x, int16 y, uint16 w, uint16 h, uint16 transcoler, uint16 color_check)
{
	int count;
	int16 x_d, y_d;
	int16 x_min, y_min;
	int16 x_max, y_max;
	uint16 *p_pic;
	uint16 *p_scr;

	logi("_BitmapCheck(%#p,%d,%d,%d,%d)", p, x, y, w, h);

	y_max = MIN(h + y, screen.height);
	x_max = MIN(w + x, screen.width);
	y_min = MAX(y, 0);
	x_min = MAX(x, 0);

	count = 0;
	for (y_d = y_min; y_d < y_max; y_d++)
	{
		p_scr = mr_screenBuf + 2 * (screen.width * y_d + x_min);
		p_pic = p + w * (y_d - y) + x_min - x;
		for (x_d = x_min; x_d < x_max; x_d++)
		{
			if (*p_pic != transcoler && *p_scr != color_check)
			{
				count++;
			}
			p_scr++;
			p_pic++;
		}
	}

	return count;
}

int32 _mr_EffSetCon(int16 x, int16 y, int16 w, int16 h, int16 perr, int16 perg, int16 perb)
{
	int16 x_d, y_d;
	int16 x_min, y_min;
	int16 x_max, y_max;
	uint16 pixel_new;
	uint16 pixel;
	uint16 *p;

	logi("_me_EffSetCon(%d, %d, %d, %d, %d, %d, %d)", x, y, w, h, perr, perg, perb);

	y_max = MIN(h + y, screen.height);
	x_max = MIN(w + x, screen.width);
	y_min = MAX(y, 0);
	x_min = MAX(x, 0);

	for (y_d = y_min; y_d < y_max; y_d++)
	{
		p = mr_screenBuf + screen.width * y_d + x_min;
		for (x_d = x_min; x_d < x_max; x_d++)
		{
			pixel = *p;
			pixel_new = (perr * (pixel & 0xF800) >> 8) & 0xF800;
			pixel_new |= (perg * (pixel & 0x07E0) >> 8) & 0x07E0;
			pixel_new |= (perb * (pixel & 0x001F) >> 8) & 0x001F;
			*p = pixel_new;
			p++;
		}
	}
	return 0;
}

int32 mr_transbitmapDraw(mr_transBitmap *hTransBmp, uint16 *dstBuf, int32 dest_max_w, int32 dest_max_h, int32 sx, int32 sy, int32 width, int32 height, int32 dx, int32 dy)
{
	uint16 *dest;
	int32 dest_maxW, dest_maxH;
	mr_transBitmap *pTransBitmap;
	int32 i, j, k;
	int32 startPixel;
	int32 len, actualLen;
	int32 hasNext = 0, nextPixel;
	uint16 *destLine, *destPixel;
	uint16 *srcLine, *srcPixel;
	int16 *mm;
	int32 pos1_e, pos2_e;

	pTransBitmap = hTransBmp;
	if (!pTransBitmap)
	{
		return MR_FAILED;
	}

	dest = dstBuf;

	dest_maxW = dest_max_w;
	dest_maxH = dest_max_h;

	hasNext = 0;

	/**先做负值坐标切换**/
	if (dx < 0)
	{
		sx += -dx;
		width += dx;
		dx = 0;
	}

	if (dy < 0)
	{
		sy += -dy;
		height += dy;
		dy = 0;
	}

	len = dest_maxW - dx;
	width = width < len ? width : len;

	mm = pTransBitmap->pMatrix + sy * 2 * pTransBitmap->markCount;
	srcLine = (uint16 *)pTransBitmap->pData + pTransBitmap->maxWidth * sy;
	destLine = dest + dest_maxW * dy + dx;

	pos1_e = sx + width;

	if (height > dest_maxH - dy)
		height = dest_maxH - dy;

	for (i = sy; i < sy + height; i++)
	{
		for (j = 0; j < pTransBitmap->markCount; j++)
		{
			startPixel = mm[j * 2];

			if (startPixel < 0) /*表示这个标记位无效,接下去的标记位也必然无效*/
			{
				break;
			}

			len = mm[j * 2 + 1];

			if (j == pTransBitmap->markCount - 1) /*到达最后一个标记栏，这里有是否包含后续点的分析*/
			{
				hasNext = (int32)(len & 0x4000); /*是否有未标识像素*/
				len &= 0x3FFF;

				if (hasNext)
				{
					nextPixel = startPixel + len;
				}
			}

			pos2_e = startPixel + len;

			if (pos2_e < startPixel || pos1_e < sx)
				continue; /*没有相交的部分*/

			startPixel = startPixel > sx ? startPixel : sx;
			pos2_e = pos2_e < pos1_e ? pos2_e : pos1_e;

			actualLen = pos2_e - startPixel;

			srcPixel = srcLine + startPixel;
			destPixel = destLine + (startPixel - sx);

			if (actualLen > 0)
			{
				if ((((int32)destPixel) & 3) != 0 &&
					(((int32)srcPixel) & 3) != 0)
				{
					*destPixel++ = *srcPixel++;
					actualLen--;
				}

				if (actualLen > 0)
					memcpy(destPixel, srcPixel, actualLen * 2);
			}
		}

		/*存在剩余的像素不在标识区*/
		if (hasNext)
		{
			hasNext = 0;
			startPixel = nextPixel;
			startPixel = startPixel > sx ? startPixel : sx;

			srcPixel = srcLine + startPixel;
			destPixel = destLine + (startPixel - sx);

			for (k = startPixel; k < sx + width; k++)
			{
				if (*srcPixel != pTransBitmap->transColor)
					*destPixel = *srcPixel;
				destPixel++;
				srcPixel++;
			}
		}

		srcLine += pTransBitmap->maxWidth;
		destLine += dest_maxW;
		mm += pTransBitmap->markCount * 2;
	}
	return 0;
}

/*
 *  x，y -- 坐标系： 屏幕， translate之前的坐标
 *
 *  width/height -- 将要渲染的图片或者region的尺寸， 没有Clip之前。
 */
static void calc_anchor(mr_jgraphics_context_t *gContext, int x, int y, int width, int height, int anchor, int *outx, int *outy)
{
	int anchor_h, anchor_v;

	x += gContext->mutableValues.translateX;
	y += gContext->mutableValues.translateY;

	if (anchor == 0)
	{
		anchor_h = LEFT_Graphics;
		anchor_v = TOP_Graphics;
	}
	else
	{
		anchor_h = AP_H_MASK_Graphics & anchor;
		anchor_v = AP_V_MASK_Graphics & anchor;
	}

	if (anchor_h == LEFT_Graphics && anchor_v == TOP_Graphics)
	{
		// quick case
		*outx = x, *outy = y;
		return;
	}
	else
	{
		switch (anchor_h)
		{
		case RIGHT_Graphics:
			*outx = x - width;
			break;
		case LEFT_Graphics:
			*outx = x;
			break;
		case HCENTER_Graphics:
			*outx = x - width / 2;
			break;
		default:
			return;
		}

		switch (anchor_v)
		{

		case BOTTOM_Graphics:
			*outy = y - height;
			break;
		case TOP_Graphics:
			*outy = y;
			break;
		case BASELINE_Graphics:
			*outy = y - height / 2 - 3; // XXX - 3 pixels
			break;
		case VCENTER_Graphics:
			*outy = y - height / 2;
			break;
		default:
			return;
		}
	}
}

// x_src, y_src是相对于图片的坐标。
void mr_drawRegion(mr_jgraphics_context_t *gContext, mr_jImageSt *src, int sx, int sy, int w, int h, int transform, int x, int y, int anchor)
{
	register uint16 transcolor;
	register int k, n;
	register uint16 *dstp, *srcp; //内层循环像素的指针。
	register int dx, dy;

	int dx_o;
	uint16 *dstp_o;
	int screenWidth = gContext->__REAL_SJC_SCREEN_WIDTH__;
	uint16 *value;

	int MinX, MaxX, MinY, MaxY; /* 经过clip后的区域， 相对于目标缓冲。 */

	if (!gContext || !src)
	{
		return;
	}

	if (src->trans != 0)
	{
		transcolor = (src->trans == 1) ? (*(uint16 *)src->data) : (src->transcolor);
	}

	// 先把锚点算好。
	if (anchor == 20 || anchor == 0)
	{
		x += gContext->mutableValues.translateX;
		y += gContext->mutableValues.translateY;
	}
	else
	{
		if (!(transform >> 2))
		{
			// 没有90度翻转的情况
			calc_anchor(gContext, x, y, w, h, anchor, &x, &y);
		}
		else
		{
			calc_anchor(gContext, x, y, h, w, anchor, &x, &y);
		}
	}

	// 开始计算Clip
	MinX = MAX(x, gContext->mutableValues.clipX);
	MinY = MAX(y, gContext->mutableValues.clipY);

	if (!(transform >> 2))
	{
		// 宽和高互调一下。
		MaxX = MIN(x + w, gContext->mutableValues.clipXRight);
		MaxY = MIN(y + h, gContext->mutableValues.clipYBottom);
	}
	else
	{
		MaxX = MIN(x + h, gContext->mutableValues.clipXRight);
		MaxY = MIN(y + w, gContext->mutableValues.clipYBottom);
	}

	dy = MaxY - MinY;
	dx_o = dx = MaxX - MinX;

	if (dy <= 0 || dx <= 0)
	{
		return;
	}

	switch (transform)
	{
	case TRANS_NONE_Sprite:

		if (src->trans == 0)
		{

			value = src->data + (sx + MinY - y) * src->width + (MinX - x + sx);

			srcp = value;
			dstp = (uint16 *)(gContext->__SJC_SCREEN_BUFFER__ + MinY * gContext->__SJC_SCREEN_WIDTH__ + MinX);

			n = (int16)src->width;

			while (dy-- > 0)
			{
				//copy一行
				memcpy(dstp, srcp, dx * 2);
				dstp += screenWidth;
				srcp += n;
			}

			return;
		}

		break;

	case TRANS_MIRROR_ROT180_Sprite:
		value = src->data + (h - 1 - (MinY - y - sy)) * src->width + (MinX - x + sx);

		k = 1;
		n = 0 - (int16)src->width;

		if (src->trans == 0)
		{
			srcp = value;
			dstp = (uint16 *)(gContext->__SJC_SCREEN_BUFFER__ + MinY * gContext->__SJC_SCREEN_WIDTH__ + MinX);

			while (dy-- > 0)
			{
				//copy一行
				memcpy(dstp, srcp, dx * 2);
				dstp += screenWidth;
				srcp += n;
			}
			return;
		}

		break;

	case TRANS_ROT180_Sprite:
		value = src->data + (h - 1 + sy) * src->width + w - (MinY - y) * src->width - 1 + sx - MinX + x;

		k = -1;
		n = 0 - (int16)src->width;

		break;

	case TRANS_MIRROR_Sprite: //FLIP_HORI
		value = src->data + (MinY - y + sy) * src->width + sx + w - 1 + x - MinX;
		k = -1;
		n = (int16)src->width;
		break;

	case TRANS_ROT90_Sprite:
		value = src->data + (h + sy - 1) * src->width + (MinY - y) + sx - (MinX - x) * src->width;
		k = 0 - (int16)src->width;
		n = 1;
		break;

	case TRANS_MIRROR_ROT90_Sprite:
		value = src->data + sx + (h + sy - 1) * src->width + w - MinY + y - 1 - MinX * src->width + x * src->width;
		k = 0 - (int16)src->width;
		n = -1;
		break;

	case TRANS_ROT270_Sprite:
		value = src->data + sx + sy * src->width + w - (MinY - y) - 1 + MinX * src->width - x * src->width;
		k = (int16)src->width;
		n = -1;
		break;

	case TRANS_MIRROR_ROT270_Sprite:
		value = src->data + (sy)*src->width - y + sx + MinY + MinX * src->width - x * src->width;
		k = (int16)src->width;
		n = 1;
		break;

	default:
		return;
	}

	dstp = dstp_o = (uint16 *)(gContext->__SJC_SCREEN_BUFFER__ + MinY * gContext->__SJC_SCREEN_WIDTH__ + MinX);

	if (src->trans == 0)
	{
		while (dy-- > 0)
		{
			srcp = value;
			while (dx-- > 0)
			{
				*dstp = *srcp;
				srcp += k;
				dstp++;
			}
			dx = dx_o;
			dstp = dstp_o += screenWidth;
			value += n;
		}
	}
	else
	{
		while (dy-- > 0)
		{
			srcp = value;
			while (dx-- > 0)
			{
				if (*srcp != transcolor)
				{
					*dstp = *srcp;
				}

				srcp += k;
				dstp++;
			}
			dx = dx_o;
			dstp = dstp_o += screenWidth;
			value += n;
		}
	}
}