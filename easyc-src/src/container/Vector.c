#include <stdlib.h>
#include <Windows.h>
#include "Vector.h"
#include "Object.h"

static const INITIAL_CAPACITY = 16;
static const float DEFAULT_LOAD_FACTOR = 0.75f;

static void * get(Vector * const self, int idx);
static void set(Vector * const self, int idx, void *item);
static void fset(Vector * const self, int idx, void *item);
static void ffset(Vector * const self, int idx, void *item, void(*freeMethod)(void *));
static void add(Vector * const self, void *item);
static void fadd(Vector * const self, void *item);
static void ffadd(Vector *const self, void *item, void(*freeMethod)(void *));
static void insert(Vector * const self, int idx, void *item);
static void finsert(Vector * const self, int idx, void *item);
static void ffinsert(Vector * const self, int idx, void *item, void(*freeMethod)(void *));
static void remove(Vector * const self, int idx);
static int size(Vector * const self);
static void *toArray(Vector *const self);
static String *toString(Vector *const self);
static void _resize(Vector * const self, int target);

/*
* @Desc   : Vector构造器。构造一个无元素的Vector实例。
* @Return : 返回新的构建好的Vector
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
Vector *newVector(){
	Vector * vector = (Vector *)malloc(sizeof(Vector));
	ZeroMemory(vector, sizeof(Vector));
	vector->_mem = (Object **)malloc(sizeof(Object *)*INITIAL_CAPACITY);
	ZeroMemory(vector->_mem, sizeof(Object *)*INITIAL_CAPACITY);
	vector->_capacity = INITIAL_CAPACITY;
	vector->_loadFactor = DEFAULT_LOAD_FACTOR;
	vector->_size = 0;

	// load function
	vector->get = get;
	vector->set = set;
	vector->fset = fset;
	vector->ffset = ffset;
	vector->add = add;
	vector->fadd = fadd;
	vector->ffadd = ffadd;
	vector->insert = insert;
	vector->finsert = finsert;
	vector->ffinsert = ffinsert;
	vector->remove = remove;
	vector->size = size;
	vector->toArray = toArray;
	vector->toString = toString;
	vector->_resize = _resize;
}

/*
* @Desc   : Vector构造器.通过整数数组的首地址和长度, 构造一个Vector.
* @Param  : *ptr, 数组的首元素地址
* @Param  : length, 数组长度
* @Return : 返回新的构建好的Vector
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
Vector *newVectorByInts(int *ptr, int length){
	Vector *vector = newVector();
	for (int i = 0; i < length; i++, ptr++){
		vector->add(vector, *ptr);
	}
	return vector;
}

/*
* @Desc   : Vector构造器.构造一个长度为length,所有元素为整数number的Vector.
* @Param  : number, 填充Vector的数值.
* @Param  : length, 填充的长度.
* @Return : 返回新的构建好的Vector.
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
Vector *newVectorByNumber(int number, int length){
	Vector *vector = newVector();
	for (int i = 0; i < length; i++){
		vector->add(vector, number);
	}
	return vector;
}

/*
 * @Desc   : Vector构造器.通过一个Vector构造另外一个Vector, 新Vector的元素和原Vector的元素相同.
 * @Param  : ovect, 原始的Vector.
 * @Return : 返回新的构建好的Vector.
 * @Authro : Shuaiji Lu
 * @Date   : 2017.11.25
*/
Vector *newVectorByVector(Vector * const ovect){
	Vector *vector = newVector();
	for (int i = 0; i < ovect->size(ovect); i++){
		vector->_resize(vector, vector->_size+1);
		vector->_mem[i] = ovect->_mem[i]->copy(ovect->_mem[i]);
		vector->_size += 1;
	}
	return vector;
}

/*
* @Desc   : 释放Vector容器,  包括容器中的object以及关联内存.
* @Param  : *vector, 待释放的Vector容器
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
void freeVector(Vector * const vector){
	if (vector->_relative){
		freeVector(vector->_relative);
	}
	for (int i = 0; i < vector->_size; i++){
		freeObject(vector->_mem[i]);
	}
	free(vector->_mem);
	free(vector);
}

/*
* @Desc   : 释放所有的Vector.
* @Param  : vector, vector中的元素为待释放的vector实例.
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
void freeAllVector(Vector * const vector){
	for (int i = 0; i < vector->_size; i++){
		freeVector(vector->get(vector, i));
	}
}

/*
* @Desc   : 获得指定索引位置处的元素.
* @Param  : *self, 待操作的vector.
* @Param  : idx, 获得该索引处的元素.
* @Return : 索引处的元素.
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void * get(Vector * const self, int idx){
	if (idx >= self->_size){
		return NULL;
	}
	return self->_mem[idx]->item;
}


/*
* @Desc   : 给指定位置进行赋值, 并且设置主题内存不进行内存释放, 原先idx除的主题内存会进行释放.
* @Param  : *self, 待操作的vector.
* @Param  : idx, 待赋值的位置.
* @Param  : *item, 赋值到vector的元素.
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void set(Vector * const self, int idx, void *item){
	if (idx >= self->_size){
		return NULL;
	}
	Object * old = self->_mem[idx];
	if (old==NULL){								// 该空间为NULL, 直接生成Object即可
		self->_mem[idx] = simpleObject(item);
	}
	else{
		if (old->freeMethod == NULL){			// 若释放方式为空, 则直接设置item.
			self->_mem[idx]->item = item;
		}
		else{
			freeObject(old);					// 该空间不为NULL, 且释放方式不为NULL, 则先释放原来的内存，再生成新的object
			self->_mem[idx] = simpleObject(item);
		}
	}
}

/*
* @Desc   : 给指定位置进行赋值, 并且设置主题内存采用free自动释放, 原先idx除的主题内存会进行释放.
* @Param  : *self, 待操作的vector.
* @Param  : idx, 待赋值的位置.
* @Param  : *item, 赋值到vector的元素.
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void fset(Vector * const self, int idx, void *item){
	if (idx >= self->_size){
		return NULL;
	}
	Object * old = self->_mem[idx];
	if (old == NULL){							// 该空间为NULL, 直接生成Object即可
		self->_mem[idx] = newObject(item, free);
	}
	else{
		if (old->freeMethod == free){			// 若释放方式为freeMethod, 则直接设置item.
			self->_mem[idx]->item = item;
		}
		else{
			freeObject(old);					// 该空间不为NULL, 且释放方式不为NULL, 则先释放原来的内存，再生成新的object
			self->_mem[idx] = newObject(item, free);
		}
	}
}

/*
* @Desc   : 给指定位置进行赋值, 并且设置主题内存用freeMethod自动释放, 原先idx除的主题内存会进行释放.
* @Param  : *self, 待操作的vector.
* @Param  : idx, 待赋值的位置.
* @Param  : *item, 赋值到vector的元素.
* @Param  : *freeMethod, 主题内存的释放方式.
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void ffset(Vector * const self, int idx, void *item, void(*freeMethod)(void *)){
	if (idx >= self->_size){
		return NULL;
	}
	Object * old = self->_mem[idx];
	if (old == NULL){								// 该空间为NULL, 直接生成Object即可
		self->_mem[idx] = newObject(item, freeMethod);
	}
	else{
		if (old->freeMethod == freeMethod){			// 若释放方式为freeMethod, 则直接设置item.
			self->_mem[idx]->item = item;
		}
		else{
			freeObject(old);					// 该空间不为NULL, 且释放方式不为NULL, 则先释放原来的内存，再生成新的object
			self->_mem[idx] = newObject(item, freeMethod);
		}
	}
}

/*
* @Desc   : 在末尾添加一格新元素, 新主题内存不进行自动释放.
* @Param  : *self, 待操作的vector.
* @Param  : *item, 主题内存.
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void add(Vector * const self, void *item){
	self->_resize(self, self->_size + 1);
	self->_mem[self->_size] = simpleObject(item);
	self->_size += 1;
}

/*
* @Desc   : 在末尾添加一格新元素, 新主题内存采用free进行自动释放.
* @Param  : *self, 待操作的vector.
* @Param  : *item, 主题内存.
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void fadd(Vector * const self, void *item){
	self->_resize(self, self->_size + 1);
	self->_mem[self->_size] = newObject(item, free);
	self->_size += 1;
}

/*
* @Desc   : 在末尾添加一格新元素, 新主题内存采用freeMethod进行自动释放.
* @Param  : *self, 待操作的vector.
* @Param  : *item, 主题内存.
* @Param  : *freeMethod, 主题内存的释放方式.
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void ffadd(Vector *const self, void *item, void(*freeMethod)(void *)){
	self->_resize(self, self->_size + 1);
	self->_mem[self->_size] = newObject(item, freeMethod);
	self->_size += 1;
}

/*
* @Desc   : 删除指定索引位置的元素，将该索引后面的元素往前面移动一格, 被删除的元素会被自动释放掉.
* @Param  : *self, 待操作的vector.
* @Param  : idx, 待删除的索引位置.
* @Return : 无.
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void remove(Vector * const self, int idx){
	if (idx >= self->_size){
		return NULL;
	}
	freeObject(self->_mem[idx]);
	for (int i = idx; i < self->_size-1; i++){
		self->_mem[i] = self->_mem[i+1];
	}
	self->_mem[self->_size - 1] = NULL;
	self->_size -= 1;
}

/*
* @Desc   : 在指定的位置插入一个元素, 该位置后面的所有元素往后面移动一格.新主题内存不进行自动释放
* @Param  : *self, 待操作的vector.
* @Param  : idx, 在该索引位置插入.
* @Param  : *item, 插入的主题内存.
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void insert(Vector * const self, int idx, void *item){
	if (idx >= self->_size+1){		// 可以插在末尾
		return NULL;
	}
	self->_resize(self, self->_size + 1);
	for (int i = idx; i < self->_size; i++){
		self->_mem[i + 1] = self->_mem[i];
	}
	self->_mem[idx] = simpleObject(item);
	self->_size += 1;
}

/*
* @Desc   : 在指定的位置插入一个元素, 该位置后面的所有元素往后面移动一格.新主题内存采用free进行自动释放.
* @Param  : *self, 待操作的vector.
* @Param  : idx, 在该索引位置插入.
* @Param  : *item, 插入的主题内存.
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void finsert(Vector * const self, int idx, void *item){
	if (idx >= self->_size + 1){		// 可以插在末尾
		return NULL;
	}
	self->_resize(self, self->_size + 1);
	for (int i = idx; i < self->_size; i++){
		self->_mem[i + 1] = self->_mem[i];
	}
	self->_mem[idx] = newObject(item, free);
	self->_size += 1;
}

/*
* @Desc   : 在指定的位置插入一个元素, 该位置后面的所有元素往后面移动一格.新主题内存采用freeMethod进行自动释放
* @Param  : *self, 待操作的vector.
* @Param  : idx, 在该索引位置插入.
* @Param  : *item, 插入的主题内存.
* @Param  : *freeMethod, 主题内存的释放方式.
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void ffinsert(Vector * const self, int idx, void *item, void(*freeMethod)(void *)){
	if (idx >= self->_size + 1){		// 可以插在末尾
		return NULL;
	}
	self->_resize(self, self->_size + 1);
	for (int i = idx; i < self->_size; i++){
		self->_mem[i + 1] = self->_mem[i];
	}
	self->_mem[idx] = newObject(item, freeMethod);
	self->_size += 1;
}

/*
* @Desc   : 得到vector中的元素个数.
* @Param  : *self, 待操作的vector.
* @Return : vector中存在的元素个数.
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static int size(Vector * const self){
	return self->_size;
}

static void *toArray(Vector *const self){

}

static String *toString(Vector *const self){
	String *s = newString("");
	for (int i = 0; i < self->_size; i++){
		void *ptr = self->get(self, i);
		s->appendPtr(s, ptr)->append(s, " ");
	}
	if (self->_relative == NULL){
		self->_relative = newVector();
	}
	self->_relative->ffadd(self->_relative, s, freeString);
	return s;
}

/*
* @Desc   : vector扩容. 当vector中再增加一个元素达到capacity*loadFactor时, 将进行扩容操作.
* @Param  : *self, 待操作的vector.
* @Return : 无
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static void _resize(Vector * const self, int target){
	if ( target < self->_capacity * self->_loadFactor){
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