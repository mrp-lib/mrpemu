#ifndef __MRP_MR_GUI_H__
#define __MRP_MR_GUI_H__

#include "mrp/common/helper.h"

enum
{
	MR_DIALOG_OK,		 //对话框有"确定"键。
	MR_DIALOG_OK_CANCEL, //对话框有"确定" "取消"键。
	MR_DIALOG_CANCEL	 //对话框有"返回"键。
};

enum
{
	MR_EDIT_ANY,	 //任何字符
	MR_EDIT_NUMERIC, // 数字
	MR_EDIT_PASSWORD //密码，用"*"显示
};

/*菜单，此结构分配在mrp虚拟机中，结构中的指针则在宿主机中分配*/
typedef struct mr_menu_st
{
	char *title;	 //标题
	char **items;	 //菜单项
	uint32 item_num; //菜单项数量
	uint32 current;	 //激活状态下的菜单项
} mr_menu_t;

/*对话框，内存分配方式同菜单*/
typedef struct mr_dialog_st
{
	char *title; //对话框标题
	char *text;	 //对话框内容
	int32 type;	 //对话框类型
} mr_dialog_t;

/*文本框框，内存分配方式同菜单*/
typedef struct mr_textbox_st
{
	char *title; //文本框标题
	char *text;	 //文本框内容
	int32 type;	 //文本框类型
} mr_textbox_t;

/*文本编辑框，内存分配方式同上*/
typedef struct mr_textedit_st
{
	char *title;	  //标题
	char *text;		  //初始内容
	int32 type;		  //类型
	int32 max_size;	  //最大长度
	vmpt buffer;	  //缓冲区，当mr_editGetText被调用时，将分配内存，并将结果放到这里（这里的内存是在虚拟机中分配的）。
	int32 buffer_len; //缓冲区长度
} mr_textedit_t;

void swi_mr_menuCreate(vm_info_t *vm);
void swi_mr_menuSetItem(vm_info_t *vm);
void swi_mr_menuShow(vm_info_t *vm);
void swi_mr_menuSetFocus(vm_info_t *vm);
void swi_mr_menuRelease(vm_info_t *vm);
void swi_mr_menuRefresh(vm_info_t *vm);

void swi_mr_dialogCreate(vm_info_t *vm);
void swi_mr_dialogRelease(vm_info_t *vm);
void swi_mr_dialogRefresh(vm_info_t *vm);

void swi_mr_textCreate(vm_info_t *vm);
void swi_mr_textRelease(vm_info_t *vm);
void swi_mr_textRefresh(vm_info_t *vm);

void swi_mr_editCreate(vm_info_t *vm);
void swi_mr_editRelease(vm_info_t *vm);
void swi_mr_editGetText(vm_info_t *vm);

void swi_mr_winCreate(vm_info_t *vm);
void swi_mr_winRelease(vm_info_t *vm);

#endif