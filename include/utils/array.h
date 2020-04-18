#ifndef __UTILS_ARRAY_H__
#define __UTILS_ARRAY_H__

#include "common/type.h"

//数组信息
typedef struct
{
	uint32 size;   //数组容量
	uint32 length; //数组长度
	void **datas;  //数组的数据列表
} array_t;

//创建一个数组，指定一个容量
array_t *array(uint32 size);

//销毁数组
void array_free(array_t *array);

//加入元素到数组，并返回加入的元素
void *array_push(array_t *array, void *data);

//取出数组最后一个元素，如果没有返回null
void *array_pop(array_t *array);

//获取第index个元素，如果没有返回null
void *array_item(array_t *array, uint32 index);

//删除第index个元素，并返回所删除的元素
void *array_remove(array_t *array, uint32 index);

#endif