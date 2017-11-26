#include <stdlib.h>
#include <Windows.h>
#include "Vector.h"

static const INITIAL_CAPACITY = 16;
static const float DEFAULT_LOAD_FACTOR = 0.75f;

static void * get(Vector * const self, int idx);
static void * set(Vector * const self, int idx, void *obj);
static void add(Vector * const self, void *obj);
static void * remove(Vector * const self, int idx);
static void insert(Vector * const self, int idx, void *obj);
static int size(Vector * const self);
static void resize(Vector * const self);

/*
* @Desc   : Vector������������һ����Ԫ�ص�Vectorʵ����
* @Return : �����µĹ����õ�Vector
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
Vector *newVector(){
	Vector * vector = (Vector *)malloc(sizeof(Vector));
	ZeroMemory(vector, sizeof(Vector));
	vector->_mem = (void **)malloc(sizeof(void *)*INITIAL_CAPACITY);
	for (int i = 0; i < INITIAL_CAPACITY; i++){
		vector->_mem[i] = NULL;
	}
	vector->_capacity = INITIAL_CAPACITY;
	vector->_loadFactor = DEFAULT_LOAD_FACTOR;
	vector->_size = 0;

	// load function
	vector->get = get;
	vector->set = set;
	vector->add = add;
	vector->insert = insert;
	vector->remove = remove;
	vector->size = size;
	vector->_resize = resize;
}

/*
* @Desc   : Vector������.ͨ������������׵�ַ�ͳ���, ����һ��Vector.
* @Param  : *ptr, �������Ԫ�ص�ַ
* @Param  : length, ���鳤��
* @Return : �����µĹ����õ�Vector
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
Vector *newVectorByInts(int *ptr, int length){
	Vector *vector = newVector();
	for (int i = 0; i < length; i++, ptr++){
		vector->add(vector, *ptr);
	}
	return vector;
}

/*
* @Desc   : Vector������.����һ������Ϊlength,����Ԫ��Ϊ����number��Vector.
* @Param  : number, ���Vector����ֵ.
* @Param  : length, ���ĳ���.
* @Return : �����µĹ����õ�Vector.
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
Vector *newVectorByNumber(int number, int length){
	Vector *vector = newVector();
	for (int i = 0; i < length; i++){
		vector->add(vector, number);
	}
	return vector;
}

/*
 * @Desc   : Vector������.ͨ��һ��Vector��������һ��Vector, ��Vector��Ԫ�غ�ԭVector��Ԫ����ͬ.
 * @Param  : ovect, ԭʼ��Vector.
 * @Return : �����µĹ����õ�Vector.
 * @Authro : ¬˧��
 * @Date   : 2017.11.25
*/
Vector *newVectorByVector(Vector * const ovect){
	Vector *vector = newVector();
	for (int i = 0; i < ovect->size(ovect); i++){
		vector->add(vector, ovect->get(ovect, i));
	}
	return vector;
}

/*
* @Desc   : �ͷ�Vector����, ֻ�ͷ����������ͷ����е�ָ����ָ����ڴ�.
* @Param  : *vector, ���ͷŵ�Vector����
* @Return : ��
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
void freeVector(Vector * const vector){
	free(vector->_mem);
	vector->_mem = NULL;
	free(vector);
}

/*
* @Desc   : �ͷ����е�Vector, ֵ�ͷ���ЩVector����, �����ͷ�����ָ����ָ����ڴ�.
* @Param  : vector, vector�е�Ԫ��Ϊ���ͷŵ�vectorʵ��.
* @Return : ��
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
void freeAllVector(Vector * const vector){
	for (int i = 0; i < vector->_size; i++){
		freeVector(vector->get(vector, i));
	}
}

/*
* @Desc   : ���ָ������λ�ô���Ԫ��.
* @Param  : *self, ��������vector.
* @Param  : idx, ��ø���������Ԫ��.
* @Return : ��������Ԫ��.
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
static void * get(Vector * const self, int idx){
	if (idx >= self->_size){
		return NULL;
	}
	return self->_mem[idx];
}


/*
* @Desc   : ��ָ��λ�ý��и�ֵ, ���ظ�λ��ԭ�ȵ�Ԫ��.
* @Param  : *self, ��������vector.
* @Param  : idx, ����ֵ��λ��.
* @Param  : *obj, ��ֵ��vector��Ԫ��.
* @Return : �����µĹ����õ�Vector
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
static void * set(Vector * const self, int idx, void *obj){
	if (idx >= self->_size){
		return NULL;
	}
	void * old = self->_mem[idx];
	self->_mem[idx] = obj;
	return old;
}

/*
* @Desc   : ��ĩβ���һ����Ԫ��.
* @Param  : *self, ��������vector.
* @Param  : length, ���ĳ���.
* @Return : ��
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
static void add(Vector * const self, void *obj){
	self->_resize(self);
	self->_mem[self->_size] = obj;
	self->_size += 1;
}

/*
* @Desc   : ɾ��ָ������λ�õ�Ԫ�أ��������������Ԫ����ǰ���ƶ�һ��, ���ҷ��ر�ɾ����Ԫ��.
* @Param  : *self, ��������vector.
* @Param  : idx, ��ɾ��������λ��.
* @Return : ���ر�ɾ����Ԫ��.
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
static void * remove(Vector * const self, int idx){
	if (idx >= self->_size){
		return NULL;
	}
	void *old = self->_mem[idx];
	for (int i = idx; i < self->_size-1; i++){
		self->_mem[i] = self->_mem[i+1];
	}
	self->_mem[self->_size - 1] = NULL;
	self->_size -= 1;
	return old;
}

/*
* @Desc   : ��ָ����λ�ò���һ��Ԫ��, ��λ�ú��������Ԫ���������ƶ�һ��.
* @Param  : *self, ��������vector.
* @Param  : idx, �ڸ�����λ�ò���.
* @Return : ��
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
static void insert(Vector * const self, int idx, void *obj){
	if (idx >= self->_size+1){		// ���Բ���ĩβ
		return NULL;
	}
	self->_resize(self);
	for (int i = idx; i < self->_size; i++){
		self->_mem[i + 1] = self->_mem[i];
	}
	self->_mem[idx] = obj;
	self->_size += 1;
}

/*
* @Desc   : �õ�vector�е�Ԫ�ظ���.
* @Param  : *self, ��������vector.
* @Return : vector�д��ڵ�Ԫ�ظ���.
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
static int size(Vector * const self){
	return self->_size;
}

/*
* @Desc   : vector����. ��vector��������һ��Ԫ�شﵽcapacity*loadFactorʱ, ���������ݲ���.
* @Param  : *self, ��������vector.
* @Return : ��
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
static void resize(Vector * const self){
	if ( (self->_size+1) < self->_capacity * self->_loadFactor){
		return;
	}
	int newcap = 2 * self->_capacity;
	void ** newmem = (void **)malloc(sizeof(void *) * newcap);
	for (int i = 0; i < newcap; i++){
		if (i < self->_capacity){
			newmem[i] = self->_mem[i];
		}
		else{
			newmem[i] = NULL;
		}
	}
	free(self->_mem);
	self->_mem = newmem;
	self->_capacity = newcap;
}