#include <stdlib.h>
#include <string.h>
#include "mrp.h"
#include "utils/coding.h"

#define check_handle(handle) \
	if (handle <= 0)         \
	{                        \
		mr_ret(MR_FAILED);   \
		return;              \
	}
;

char *parse_text(char *text)
{
	uint32 len = UCS2_strlen(text);
	char *buf = malloc(len + 2); //注意，对于Unicode字符串，需要+2
	memcpy(buf, text, len);

	UCS2ByteRev(buf);
	UCS2ToGBString((const uint16 *)buf, buf, len);

	return buf;
}

void swi_mr_menuCreate(vm_info_t *vm)
{
	//原型： (const char* title, int16 num) -> int32
	char *title = parse_text(vmpt_real(char, vmreg(0)));
	int16 num = vmreg(1);
	logsysc("mr_menuCreate(title=%s, num=%d)", title, num);

	//创建菜单
	vmpt _menu = mem_malloc(vm, sizeof(mr_menu_t));
	check_handle(_menu);

	mr_menu_t *menu = vmpt_real(mr_menu_t, _menu);
	memset(menu, 0, sizeof(mr_menu_t));
	//分配菜单项
	menu->items = (char **)malloc(sizeof(char *) * num);
	if (menu->items == null)
	{
		mem_free(vm, _menu, sizeof(mr_menu_t));
		mr_ret(MR_FAILED);
		return;
	}
	memset(menu->items, 0, sizeof(char *) * num);
	//赋值
	menu->title = title;
	menu->item_num = num;
	menu->current = 0;

	//返回
	mr_ret(_menu);
}

void swi_mr_menuSetItem(vm_info_t *vm)
{
	//原型： (int32 menu, const char *text, int32 index) -> int32
	int32 _menu = vmreg(0);
	char *text = parse_text(vmpt_real(char, vmreg(1)));
	int32 index = vmreg(2);
	logsysc("mr_menuSetItem(menu=%d, text=%s, item=%d)", _menu, text, index);

	check_handle(_menu);

	mr_menu_t *menu = vmpt_real(mr_menu_t, _menu);
	if (index >= menu->item_num)
	{
		mr_ret(MR_FAILED);
		return;
	}
	if (menu->items[index])
		free(menu->items[index]);
	menu->items[index] = text;

	mr_ret(MR_SUCCESS);
}

void swi_mr_menuShow(vm_info_t *vm)
{
	//原型： (int32 menu) -> int32
	int32 _menu = vmreg(0);
	logsysc("mr_menuShow(menu=%d)", _menu);

	check_handle(_menu);

	if (vm->callbacks.onMenu)
		vm->callbacks.onMenu(vm, 1, vmpt_real(mr_menu_t, _menu));

	mr_ret(MR_SUCCESS);
}

void swi_mr_menuSetFocus(vm_info_t *vm)
{
	//原型： (int32 menu, int32 index) -> int32
	int32 _menu = vmreg(0);
	int32 index = vmreg(1);
	logsysc("mr_menuSetFocus(menu=%d, item=%d)", _menu, index);

	check_handle(_menu);

	mr_menu_t *menu = vmpt_real(mr_menu_t, _menu);
	if (index >= menu->item_num)
	{
		mr_ret(MR_FAILED);
		return;
	}

	menu->current = index;

	mr_ret(MR_SUCCESS);
}

void swi_mr_menuRelease(vm_info_t *vm)
{
	//原型： (int32 menu) -> int32
	int32 _menu = vmreg(0);
	logsysc("mr_menuRelease(menu=%d)", _menu);

	check_handle(_menu);

	mr_menu_t *menu = vmpt_real(mr_menu_t, _menu);

	if (vm->callbacks.onMenu)
		vm->callbacks.onMenu(vm, 0, menu);

	free(menu->title);
	for (uint32 i = 0; i < menu->item_num; i++)
	{
		if (menu->items[i])
			free(menu->items[i]);
	}

	mem_free(vm, _menu, sizeof(mr_menu_t));

	mr_ret(MR_SUCCESS);
}

void swi_mr_menuRefresh(vm_info_t *vm)
{
	//原型： (int32 menu) -> int32
	int32 _menu = vmreg(0);
	logsysc("mr_menuRefresh(menu=%d)", _menu);

	check_handle(_menu);

	if (vm->callbacks.onMenu)
		vm->callbacks.onMenu(vm, 2, vmpt_real(mr_menu_t, _menu));

	mr_ret(MR_SUCCESS);
}

void swi_mr_dialogCreate(vm_info_t *vm)
{
	//原型： (const char * title, const char * text, int32 type) -> int32
	char *title = parse_text(vmpt_real(char, vmreg(0)));
	char *text = parse_text(vmpt_real(char, vmreg(1)));
	int32 type = vmreg(2);
	logsysc("mr_dialogCreate(title=%s, text=%s, type=%d)", title, text, type);

	vmpt _dialog = mem_malloc(vm, sizeof(mr_dialog_t));
	check_handle(_dialog);

	if (vm->callbacks.onDialog != null)
	{
		mr_dialog_t *dialog = vmpt_real(mr_dialog_t, _dialog);
		dialog->title = title;
		dialog->text = text;
		dialog->type = type;

		vm->callbacks.onDialog(vm, 1, dialog);
	}
	mr_ret(_dialog);
}

void swi_mr_dialogRelease(vm_info_t *vm)
{
	//原型： (int32 dialog) -> int32
	int32 _dialog = vmreg(0);
	logsysc("mr_dialogRelease(dialog=%d)", _dialog);

	check_handle(_dialog);
	mr_dialog_t *dialog = vmpt_real(mr_dialog_t, _dialog);

	if (vm->callbacks.onDialog)
		vm->callbacks.onDialog(vm, 0, dialog);

	if (dialog->title != null)
		free(dialog->title);
	if (dialog->text != null)
		free(dialog->text);
	mem_free(vm, _dialog, sizeof(mr_dialog_t));

	mr_ret(MR_SUCCESS);
}

void swi_mr_dialogRefresh(vm_info_t *vm)
{
	//原型： (int32 dialog, const char * title, const char * text, int32 type) -> int32
	int32 _dialog = vmreg(0);
	char *title = parse_text(vmpt_real(char, vmreg(1)));
	char *text = parse_text(vmpt_real(char, vmreg(2)));
	int32 type = vmreg(3);
	logsysc("mr_dialogRefresh(dialog=%d, title=%s, text=%s, type=%d)", _dialog, title, text, type);

	check_handle(_dialog);
	if (vm->callbacks.onDialog != null)
	{
		mr_dialog_t *dialog = vmpt_real(mr_dialog_t, _dialog);
		if (dialog->title != null)
			free(dialog->title);
		dialog->title = title;
		if (dialog->text != null)
			free(dialog->text);
		dialog->text = text;
		dialog->type = type;

		vm->callbacks.onDialog(vm, 2, dialog);
	}

	mr_ret(MR_SUCCESS);
}

void swi_mr_textCreate(vm_info_t *vm)
{
	//原型： (const char * title, const char * text, int32 type) -> int32
	char *title = parse_text(vmpt_real(char, vmreg(0)));
	char *text = parse_text(vmpt_real(char, vmreg(1)));
	int32 type = vmreg(2);
	logsysc("mr_textCreate(title=%s, text=%s, type=%d)", title, text, type);

	vmpt _textbox = mem_malloc(vm, sizeof(mr_textbox_t));
	check_handle(_textbox);

	if (vm->callbacks.onTextBox != null)
	{
		mr_textbox_t *textbox = vmpt_real(mr_textbox_t, _textbox);
		textbox->title = title;
		textbox->text = text;
		textbox->type = type;

		vm->callbacks.onTextBox(vm, 1, textbox);
	}
	mr_ret(_textbox);
}

void swi_mr_textRelease(vm_info_t *vm)
{
	//原型： (int32 handle) -> int32
	int32 _textbox = vmreg(0);
	logsysc("mr_textRelease(handle=%d)", _textbox);

	check_handle(_textbox);
	mr_textbox_t *textbox = vmpt_real(mr_textbox_t, _textbox);

	if (vm->callbacks.onTextBox)
		vm->callbacks.onTextBox(vm, 0, textbox);

	if (textbox->title != null)
		free(textbox->title);
	if (textbox->text != null)
		free(textbox->text);
	mem_free(vm, _textbox, sizeof(mr_textbox_t));

	mr_ret(MR_SUCCESS);
}

void swi_mr_textRefresh(vm_info_t *vm)
{
	//原型： (int32 handle, const char * title, const char * text) -> int32
	int32 _textbox = vmreg(0);
	char *title = parse_text(vmpt_real(char, vmreg(1)));
	char *text = parse_text(vmpt_real(char, vmreg(2)));
	logsysc("mr_textRefresh(handle=%d, title=%s, text=%s)", _textbox, title, text);

	check_handle(_textbox);
	if (vm->callbacks.onTextBox != null)
	{
		mr_textbox_t *textbox = vmpt_real(mr_textbox_t, _textbox);
		if (textbox->title != null)
			free(textbox->title);
		textbox->title = title;
		if (textbox->text != null)
			free(textbox->text);
		textbox->text = text;

		vm->callbacks.onTextBox(vm, 2, textbox);
	}

	mr_ret(MR_SUCCESS);
}

void swi_mr_editCreate(vm_info_t *vm)
{
	//原型： (const char * title, const char * text, int32 type, int32 max_size) -> int32
	char *title = parse_text(vmpt_real(char, vmreg(0)));
	char *text = parse_text(vmpt_real(char, vmreg(1)));
	int32 type = vmreg(2);
	int32 max_size = vmreg(3);
	logsysc("mr_editCreate(title=%s, text=%s, type=%d, max_size=%d)", title, text, type, max_size);

	vmpt _edit = mem_malloc(vm, sizeof(mr_textedit_t));
	check_handle(_edit);
	mr_textedit_t *edit = vmpt_real(mr_textedit_t, _edit);
	edit->buffer_len = edit->buffer = 0;

	if (vm->callbacks.onTextEdit != null)
	{
		edit->title = title;
		edit->text = text;
		edit->type = type;
		edit->max_size = max_size;

		vm->callbacks.onTextEdit(vm, 1, edit);
	}
	mr_ret(_edit);
}

void swi_mr_editRelease(vm_info_t *vm)
{
	//原型： (int32 edit) -> int32
	int32 _edit = vmreg(0);
	logsysc("mr_editRelease(edit=%d)", _edit);

	check_handle(_edit);
	mr_textedit_t *edit = vmpt_real(mr_textedit_t, _edit);

	if (vm->callbacks.onTextEdit)
		vm->callbacks.onTextEdit(vm, 0, edit);

	if (edit->title != null)
		free(edit->title);
	if (edit->text != null)
		free(edit->text);
	if (edit->buffer_len)
	{
		mem_free(vm, edit->buffer, edit->buffer_len);
		edit->buffer_len = edit->buffer = 0;
	}
	mem_free(vm, _edit, sizeof(mr_textedit_t));

	mr_ret(MR_SUCCESS);
}

void swi_mr_editGetText(vm_info_t *vm)
{
	//原型： (int32 edit) -> const char *
	int32 _edit = vmreg(0);
	logsysc("mr_editGetText(edit=%d)", _edit);

	if (_edit <= 0 || (vm->callbacks.onEditGetText == null))
	{
		mr_ret(0); //返回空
		return;
	}

	//获取文本
	mr_textedit_t *edit = vmpt_real(mr_textedit_t, _edit);
	char *text = vm->callbacks.onEditGetText(vm, edit);
	if (null == text)
	{
		mr_ret(0);
		return;
	}

	//缓冲区初始化
	if (edit->buffer_len > 0)
		mem_free(vm, edit->buffer, edit->buffer_len);
	edit->buffer_len = UCS2_strlen(text) + 2;
	edit->buffer = mem_malloc(vm, edit->buffer_len);
	//拷贝到缓冲
	memcpy(vmpt_real(char, edit->buffer), text, edit->buffer_len);

	//返回结果
	mr_ret(edit->buffer);
}

void swi_mr_winCreate(vm_info_t *vm)
{
	//原型： (void) -> int32
	logsysc("mr_winCreate()");

	mr_ret(MR_IGNORE);
}

void swi_mr_winRelease(vm_info_t *vm)
{
	//原型： (int32 win) -> int32
	int32 win = vmreg(0);
	logsysc("mr_winRelease(win=%d)", win);
	
	mr_ret(MR_IGNORE);
}