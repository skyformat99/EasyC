#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "Object.h"

static void * defaultItemCopy(void *item);
static Object* defaultCopy(Object *self);

/*
* @Desc   : Object����������Ҫָ��Object��item��ָ�ڴ���ͷŷ���, ������Object�ͷ�ʱ, �Զ��ͷ�item��ָ����ڴ�.
* @Param  : *item, Object�е������ڴ�ĵ�ַ.
* @Param  : *freeMethod, �����ڴ���ͷ�.
* @Return : �����µĹ����õ�Object.
* @Authro : Shuaiji Lu
* @Date   : 2017/11/27
*/
Object *newObject(void *item, void(*freeMethod)(void *item)){
	Object *obj = (Object *)malloc(sizeof(Object));
	ZeroMemory(obj, sizeof(Object));
	obj->item = item;
	obj->refCount = 1;
	// load function
	obj->freeMethod = freeMethod;
	obj->itemCopy = defaultItemCopy;
	obj->copy = defaultCopy;
	return obj;
}

/*
* @Desc   : Object������������һ����Object, ���������ڴ治��Ҫ�ͷ�(�������һ����item��û��ָ������ڴ�, ������ָ���ڴ�ֲ��ڳ�������)
* @Param  : *item��Object�����ڴ�ĵ�ַ��
* @Return : �����µĹ����õ�Object
* @Authro : Shuaiji Lu
* @Date   : 2017/11/27
*/
Object *simpleObject(void *item){
	return newObject(item, NULL);
}

/*
* @Desc   : �ͷ�Object�����������������ü���, ���ڵ�Object���������������, ������һ�������ͷſռ�ʱ���ƶ�Object���ͷš���freeMethod��ΪNULLʱ����������ڴ��ͷſռ䡣
* @Param  : *chars.��ʼ�ַ���.
* @Return : �����µĹ����õ�String
* @Authro : Shuaiji Lu
* @Date   : 2017/11/27
*/
void freeObject(Object *obj){
	obj->refCount -= 1;
	if (obj->refCount == 0){
		if (obj->freeMethod){
			obj->freeMethod(obj->item);
		}
		free(obj);
	}
}

static void * defaultItemCopy(void *item){
	return item;
}

static Object* defaultCopy(Object *self){
	return newObject(self->itemCopy(self->item), self->freeMethod);
}