#ifndef __MRP_COMMON_HELPER_H__
#define __MRP_COMMON_HELPER_H__

#include "common/type.h"

typedef struct vm_info_st vm_info_t;
typedef struct mr_menu_st mr_menu_t;
typedef struct mr_dialog_st mr_dialog_t;
typedef struct mr_textbox_st mr_textbox_t;
typedef struct mr_textedit_st mr_textedit_t;

typedef void (*on_bufferRefresh_ft)(vm_info_t *vm, uint16 *bmp, int16 x, int16 y, uint16 w, uint16 h);
typedef int32 (*on_menu_ft)(vm_info_t *vm, uint32 type, mr_menu_t *menu);		  //菜单消息， type=0:销毁菜单，type=1:绘制菜单，type=2:刷新菜单
typedef int32 (*on_dialog_ft)(vm_info_t *vm, uint32 type, mr_dialog_t *dialog);	  //对话框消息，type=0:销毁对话框，type=1:绘制对话框，type=2:刷新对话框
typedef int32 (*on_textbox_ft)(vm_info_t *vm, uint32 type, mr_textbox_t *text);	  //文本框消息，type=0:销毁文本框，type=1:绘制文本框，type=2:刷新文本框
typedef int32 (*on_textedit_ft)(vm_info_t *vm, uint32 type, mr_textedit_t *edit); //文本编辑框消息，type=0:销毁文本编辑框，type=1:绘制文本编辑框
typedef char *(*on_editGetText_ft)(vm_info_t *vm, mr_textedit_t *edit);			  //文本编辑框内容获取消息

/*
虚拟机的回调函数列表（事件）
*/
typedef struct
{
	on_bufferRefresh_ft onBufferRefresh;
	on_menu_ft onMenu;
	on_dialog_ft onDialog;
	on_textbox_ft onTextBox;
	on_textedit_ft onTextEdit;
	on_editGetText_ft onEditGetText;
} vm_callback_t;

#endif