#ifndef __OBJECT_H
#define __OBJECT_H

struct Object;
typedef struct Object Object;

Object *newObject(void *item, void(*freeMethod)(void *item));
Object *simpleObject(void *item);
void freeObject(Object *obj);

/*
* @Desc   : ����һ�ֺ��������ڴ��ַ������, Ҳ������������ͬ����.�����������е��ڴ濽���Ͷ�����Զ��ͷ�.
* @Author : Shuaiji Lu
* @Date   : 2017/11/27
*/
struct Object{
	void *item;
	int refCount;

	Object*(*copy)(Object *self);

	void*(*itemCopy)(void *item);
	void(*freeMethod)(void *item);
};

#endif