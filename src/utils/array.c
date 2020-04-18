#include <stdlib.h>
#include <string.h>

#include "utils/array.h"
#include "common/log.h"

//指针类型的大小
#define size_p (sizeof(void *))

//当数组满的时候，自动追加的容量
#define ALLOC_SIZE 20

//创建一个数组
array_t *array(uint32 size)
{
	//创建数组结构
	array_t *arr = (array_t *)malloc(sizeof(array_t));
	if (arr == null)
		return null;
	memset(arr, 0, sizeof(array_t));

	//创建数据体
	void *datas = (void *)malloc(size_p * size); //这里的话指针长度在32位和64位中是不同的
	if (datas == null)
	{
		free(arr);
		return null;
	}

	//初始化
	arr->datas = datas;
	arr->size = size;

	//返回去
	return arr;
}

//销毁数组
void array_free(array_t *array)
{
	if (array != null)
	{
		//删除数据指针
		if (array->datas != null)
			free(array->datas);
		//删除数组
		free(array);
	}
}

//加入元素到数组，并返回位置
void *array_push(array_t *array, void *data)
{
	if (!array)
		return null;
	//看看数组长度是不是太小了
	if (array->size <= array->length)
	{
		uint32 size = array->size + ALLOC_SIZE;				  //计算追加后的大小
		void *new_data = (void *)malloc(size_p * size);		  //追加分配
		memcpy(new_data, array->datas, size_p * array->size); //数据拷贝
		free(array->datas);									  //释放原有内存
		array->datas = new_data;							  //使用新内存
		array->size = size;									  //更改容量
	}
	//追加元素
	*(array->datas + array->length) = data;
	array->length++;

	//返回
	return data;
}

//取出数组最后一个元素，如果没有返回null
void *array_pop(array_t *array)
{
	//空数组
	if (array->length == 0)
		return null;
	//取得最后一个元素
	void *data = *(array->datas + (array->length - 1));
	//数组长度-1
	array->length--;
	//返回
	return data;
}

//获取第index个元素，如果没有返回null
void *array_item(array_t *array, uint32 index)
{
	//越界了
	if (index >= array->length)
		return null;
	//返回数据
	return *(array->datas + index);
}

//删除第index个元素
void *array_remove(array_t *array, uint32 index)
{
	//越界了
	if (index >= array->length)
		return null;

	//拿到数据
	void *data = *(array->datas + index);

	//从后往前逐个覆盖
	for (uint32 i = index; i < array->size - 1; i++)
		*(array->datas + i) = *(array->datas + i + 1);

	array->length--;

	//返回
	return data;
}