#ifndef __VECTOR_H
#define __VECTOR_H

// �ṹ����
struct Vector;
struct Object;
struct String;
typedef struct Vector Vector;
typedef struct Object Object;
typedef struct String String;

// �������������
Vector *newVector();
Vector *newVectorByInts(int *ptr, int length);
Vector *newVectorByNumber(int number, int length);
Vector *newVectorByVector(Vector * const ovect);
void freeVector(Vector * const vector);
void freeAllVector(Vector * const vector);

// �ṹ����
struct Vector{
	int _size;
	int _capacity;
	float _loadFactor;
	Vector * _relative;
	Object ** _mem;

	// public(interface)
	void * (* get)(Vector * const self, int idx);							// ��ȡָ������������
	void (*set)(Vector * const self, int idx, void *item);					// ����ָ������������, ������ָ����ڴ治���ͷ�
	void (*fset)(Vector * const self, int idx, void *item);					// ����ָ������������, ������ָ�ڴ���ͷŷ�ʽΪfree
	void (*ffset)(Vector * const self, int idx, void *item,					// ����ָ������������, ������ָ�ڴ���ͷŷ�ʽΪfreeMethod
		void(*freeMethod)(void *));
	void(*add)(Vector * const self, void *item);							// ��ĩβ�������, ������ָ����ڴ治���ͷ�
	void(*fadd)(Vector * const self, void *item);							// ��ĩβ�������, ��ʹ��free���ͷ�������ָ����ڴ�
	void(*ffadd)(Vector *const self, void *item, void(*freeMethod)(void *));// ��ĩβ�������, ��ʹ��freeMethod���ͷ�������ָ����ڴ�
	void(*insert)(Vector * const self, int idx, void *item);				// ��ָ������λ�ò�������, ������ָ����ڴ治���ͷ�
	void(*finsert)(Vector * const self, int idx, void *item);				// ��ָ������λ�ò�������, ��ʹ��free���ͷ�������ָ����ڴ�
	void(*ffinsert)(Vector * const self, int idx, void *item,				// ��ָ������λ�ò�������, ��ʹ��freeMethod���ͷ�������ָ����ڴ�
		void(*freeMethod)(void *));				
	void (*remove)(Vector * const self, int idx);							// ɾ��ָ������������
	int(*size)(Vector * const self);										// ���ض�̬�����С
	void(*clean)(Vector * const);											// ��ն�̬���飬��δʵ�֡�
	int(*contain)(Vector * const, void * obj, int(*cmp)(void*, void *));	// �ж϶����Ƿ����, ��δʵ�֡�
	void *(*toArray)(Vector *const self);									// ת��Ϊ����, ��δʵ�֡�
	String *(*toString)(Vector *const self);								// ��Vector�е�objectת��ΪString

	// private
	void(*_resize)(Vector * const self, int target);
};

#endif