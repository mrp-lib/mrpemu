#ifndef __MR_GUI_H__
#define __MR_GUI_H__

#include "common/type.h"

typedef int32 (*T_mr_menuCreate)(const char *title, int16 num);
typedef int32 (*T_mr_menuSetItem)(int32 menu, const char *text, int32 index);
typedef int32 (*T_mr_menuShow)(int32 menu);
typedef int32 (*T_mr_menuSetFocus)(int32 menu, int32 index);
typedef int32 (*T_mr_menuRelease)(int32 menu);
typedef int32 (*T_mr_menuRefresh)(int32 menu);

typedef int32 (*T_mr_dialogCreate)(const char *title, const char *text, int32 type);
typedef int32 (*T_mr_dialogRelease)(int32 dialog);
typedef int32 (*T_mr_dialogRefresh)(int32 dialog, const char *title, const char *text, int32 type);

typedef int32 (*T_mr_textCreate)(const char *title, const char *text, int32 type);
typedef int32 (*T_mr_textRelease)(int32 text);
typedef int32 (*T_mr_textRefresh)(int32 handle, const char *title, const char *text);

typedef int32 (*T_mr_editCreate)(const char *title, const char *text, int32 type, int32 max_size);
typedef int32 (*T_mr_editRelease)(int32 edit);
typedef const char *(*T_mr_editGetText)(int32 edit);

typedef int32 (*T_mr_winCreate)(void);
typedef int32 (*T_mr_winRelease)(int32 win);

int32 mr_menuCreate(const char *title, int16 num);
int32 mr_menuSetItem(int32 menu, const char *text, int32 index);
int32 mr_menuShow(int32 menu);
int32 mr_menuSetFocus(int32 menu, int32 index);
int32 mr_menuRelease(int32 menu);
int32 mr_menuRefresh(int32 menu);

int32 mr_dialogCreate(const char *title, const char *text, int32 type);
int32 mr_dialogRelease(int32 dialog);
int32 mr_dialogRefresh(int32 dialog, const char *title, const char *text, int32 type);

int32 mr_textCreate(const char *title, const char *text, int32 type);
int32 mr_textRelease(int32 text);
int32 mr_textRefresh(int32 handle, const char *title, const char *text);

int32 mr_editCreate(const char *title, const char *text, int32 type, int32 max_size);
int32 mr_editRelease(int32 edit);
const char *mr_editGetText(int32 edit);

int32 mr_winCreate(void);
int32 mr_winRelease(int32 win);

#endif