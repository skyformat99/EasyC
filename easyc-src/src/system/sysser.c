#include <stdio.h>
#include <stdarg.h>
#include <String.h>
#include "sysser.h"
#include "Vector.h"

/*
 * @Desc  : ��װfun���������������Ŀɱ������װΪVector���ٴ�������װ�ĺ���fun����Ҫע�����Vector���ڸú�������ǰ�����ٵ���
 * @Param : * fun, ����װ�ĺ���
 * @Param : length, �ɱ�������ȣ�λ�ڿɱ������ǰ����
 * @Return: ���ر���װ�ĺ���fun�ķ���ֵ
 * @Author: Shuaiji Lu
 * @Date  : 2017/11/26
*/
void * funWrapperInts(void *(*fun)(Vector *vector), int length, ...){
	Vector *vector = newVector();
	va_list head;
	va_start(head, length);
	for (int i = 0; i < length; i++){
		vector->add(vector, va_arg(head, int));
	}
	va_end(head);
	void * obj = fun(vector);
	freeVector(vector);
	return obj;
}

void * funWrapperPtrs(void *(*fun)(Vector *vector), int length, ...){
	Vector *vector = newVector();
	va_list head;
	va_start(head, length);
	for (int i = 0; i < length; i++){
		vector->add(vector, va_arg(head, void *));
	}
	va_end(head);
	void * obj = fun(vector);
	freeVector(vector);
	return obj;
}