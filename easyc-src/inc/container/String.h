#ifndef __STRING_H
#define __STRING_H

struct String;
struct Vector;
typedef struct String String;
typedef struct Vector Vector;

String *newString(char * chars);
void freeString(String *s);
char *toChars(String *self);
int toInt(String *self);									// ��δʵ�֡�
int toFloat(String *self);									// ��δʵ�֡�

/*
* @Desc   : ��̬�ַ���, ���Զ�̬����չ�ַ���, Ҳ�ṩת��Ϊchar*�Ľӿ�.
* @Author : Shuaiji Lu
* @Date   : 2017/11/27
*/
struct String{
	char *_mem;				// �ַ���������, ������β��
	int _capacity;
	int _loadFactor;
	int _size;				// ��������β�����ַ�����
	Vector *_relative;

	// public (interface)
	String *(*append)(String *self, char *chars);
	String *(*appendString)(String *self, String *s);
	String *(*appendPtr)(String *self, void *ptr);
	String *(*appendInt)(String *self, int intnum);
	String *(*appendFloat)(String *self, float floatnum);
	String *(*substring)(String *self, int begin, int length);		// ��δʵ�֡�
	int(*size)(String *self);
	int(*toInt)(String *self);										// ��δʵ�֡�
	int(*toFloat)(String *self);									// ��δʵ�֡�
	char *(*toChars)(String *self);

	// private
	void(*_resize)(String *self, int target);
};

#endif