#ifndef __HASHMAP_H
#define __HASHMAP_H

#include "Vector.h"

// �����ṹ�弰�����
struct KVNode;
struct HashMap;
struct Entry;
typedef struct KVNode KVNode;
typedef struct HashMap HashMap;
typedef struct Entry Entry;

// ���������
HashMap *newHashMap(unsigned long(*hashFunction)(void *obj));
void freeHashMap(HashMap * const map);
void removeHashMap(HashMap * const map);

// ����ṹ���Ա
struct KVNode{
	KVNode * next;
	char * key;
	void * value;
};

struct Entry{
	char *key;
	char *value;
};

struct HashMap{
	int _size;				// ʵ�ʵĴ�С
	int _bucketsLength;		// buckets�����С
	float _maxAverageDeep;	// ���ƽ�����
	KVNode ** _buckets;		// Ͱ

	// public(interface)
	void * (*get)(HashMap * const self, void *key);
	void * (*put)(HashMap * const self, void *key, void * value);
	Vector * (*keys)(HashMap * const self);
	Vector * (*entries)(HashMap * const self);
	int(*size)(HashMap * const self);
	char * (*toString)(HashMap * const self);				// ������ת��Ϊ�ַ�����������ԡ�δʵ�֡���

	// private
	void (* _resize)(HashMap * const self);

	// ��Ҫ���õĺ�����û�����������Ĭ������
	unsigned long (* _hash)(void *obj);				// value��hash
};


#endif