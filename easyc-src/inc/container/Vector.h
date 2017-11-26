#ifndef __VECTOR_H
#define __VECTOR_H

// �ṹ����
struct Vector;
typedef struct Vector Vector;

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
	void ** _mem;

	// public(interface)
	void * (* get)(Vector * const self, int idx);								// ��ȡָ������������
	void * (* set)(Vector * const self, int idx, void *obj);					// ����ָ������������
	void(*add)(Vector * const self, void *obj);								// ��ĩβ�������
	void * (*remove)(Vector * const self, int idx);							// ɾ��ָ������������
	void(*insert)(Vector * const self, int idx, void *obj);					// ��ָ������λ�ò�������
	int(*size)(Vector * const self);											// ���ض�̬�����С
	void(*clean)(Vector * const);											// ��ն�̬���飬��δʵ�֡�
	int(*contain)(Vector * const, void * obj, int(*cmp)(void*, void *));	// �ж϶����Ƿ����, ��δʵ�֡�


	// private
	void(*_resize)(Vector * const self);
};

#endif