#ifndef __STACK_H
#define __STACK_H

struct List;
struct Stack;
typedef struct Stack Stack;
typedef struct List List;

Stack * newStack();
void freeStack(Stack * const stack);

struct Stack{
	List * _list;

	// public(interface)
	void *(*pop)(Stack * const self);					// ��ջ
	void(*push)(Stack * const self, void *obj);		// ��ջ
	void *(*top)(Stack * const self);					// ջ��Ԫ��
	int(*size)(Stack * const self);					// ջ��С
};

#endif