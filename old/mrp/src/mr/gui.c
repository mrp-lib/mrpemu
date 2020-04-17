#include "mrp.h"

int32 mr_menuCreate(const char *title, int16 num)
{
	return MR_FAILED;
}

int32 mr_menuSetItem(int32 hd, const char *text, int32 index)
{
	return MR_FAILED;
}

int32 mr_menuShow(int32 menu)
{
	return MR_IGNORE;
}

int32 mr_menuSetFocus(int32 menu, int32 index)
{
	return MR_IGNORE;
}

int32 mr_menuRelease(int32 menu)
{
	return MR_IGNORE;
}

int32 mr_menuRefresh(int32 menu)
{
	return MR_IGNORE;
}

int32 mr_dialogCreate(const char *title, const char *text, int32 type)
{
	return MR_IGNORE;
}

int32 mr_dialogRelease(int32 dialog)
{
	return MR_IGNORE;
}

int32 mr_dialogRefresh(int32 dialog, const char *title, const char *text, int32 type)
{
	return MR_FAILED;
}

int32 mr_textCreate(const char *title, const char *text, int32 type)
{
	return MR_IGNORE;
}

int32 mr_textRelease(int32 text)
{
	return MR_IGNORE;
}

int32 mr_textRefresh(int32 handle, const char *title, const char *text)
{
	return MR_IGNORE;
}

int32 mr_editCreate(const char *title, const char *text, int32 type, int32 max_size)
{
	//TODO
	return MR_FAILED;
}

int32 mr_editRelease(int32 edit)
{
	//TODO
	return MR_SUCCESS;
}

const char *mr_editGetText(int32 edit)
{
	//TODO
	return "";
}

int32 mr_winCreate(void)
{
	return MR_IGNORE;
}

int32 mr_winRelease(int32 win)
{
	return MR_IGNORE;
}
