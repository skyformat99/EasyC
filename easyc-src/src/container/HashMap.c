#include <stdlib.h>
#include <string.h>

#include "HashMap.h"

static const INITIAL_BUCKETS_LENGTH = 16;
static const DEFAULT_MAX_AVERAGE_DEEP = 3;

static void * get(HashMap * const self, void *key);
static void * put(HashMap * const self, void *key, void * value);
static int size(HashMap * const self);
static Vector *keys(HashMap * const self);
static Vector *entries(HashMap * const self);
static void resize(HashMap * const self);
static unsigned long defaultHash(void *obj);		// key的默认hash处理函数

/*
* @Desc   : HashMap构造器.构造一个空的hashMap.
* @Param  : *hashFunction, hash函数, 作用于key上进行.为NULL时认为key为字符串, 并采用easyc默认的hash函数.
* @Return : 返回新的构建好的HashMap
* @Authro : 卢帅吉
* @Date   : 2017.11.26
*/
HashMap *newHashMap( 
	unsigned long (* hashFunction)(void *obj)		// 配置hash函数
	){
	HashMap *map = (HashMap *)malloc(sizeof(HashMap));

	map->_buckets = (KVNode **)malloc(sizeof(KVNode *)*INITIAL_BUCKETS_LENGTH);
	for (int i = 0; i < INITIAL_BUCKETS_LENGTH; i++){
		map->_buckets[i] = NULL;
	}
	map->_size = 0;
	map->_bucketsLength = INITIAL_BUCKETS_LENGTH;
	map->_maxAverageDeep = DEFAULT_MAX_AVERAGE_DEEP;

	// load function
	map->get = get;
	map->put = put;
	map->size = size;
	map->keys = keys;
	map->entries = entries;
	map->_resize = resize;

	map->_hash = hashFunction == NULL ? defaultHash : hashFunction;

	return map;
}

/*
* @Desc   : 释放hashMap.仅仅释放容器, 不会释放掉容器中所引用的内存.
* @Param  : *map, 待释放的map.
* @Return : 无
* @Authro : 卢帅吉
* @Date   : 2017.11.26
*/
void freeHashMap(HashMap * const map){
	for (int i = 0; i < map->_bucketsLength; i++){
		KVNode *node = map->_buckets[i];
		while (node!=NULL && node->next != NULL){			// 释放桶中链表
			KVNode *nextnode = node->next;
			free(node);
			node = nextnode;
		}
		free(node);
		map->_buckets[i] = NULL;
	}
	free(map->_buckets);
	map->_buckets = NULL;
	free(map);
}
/*
* @Desc   : 释放hashMap.会释放掉容器中所引用到的内存.其中包括通过keys, entries返回的Vector空间.
* @Param  : *map, 待释放的map.
* @Return : 无
* @Authro : 卢帅吉
* @Date   : 2017.11.26
*/
void removeHashMap(HashMap * const map){

}

/*
* @Desc   : 获取hashmap中key所对应的val.
* @Param  : *self, 待操作的hashMap.
* @Param  : *key, 获取该key对应的val.
* @Return : 返回key对应的val.
* @Authro : 卢帅吉
* @Date   : 2017.11.26
*/
static void * get(HashMap * const self, void *key){
	int idx = self->_hash(key) % self->_bucketsLength;

	for (KVNode *node = self->_buckets[idx]; node != NULL; node = node->next){
		if (strcmp(node->key, key)==0){
			return node->value;
		}
	}

	return NULL;
}

/*
* @Desc   : 设置hashmap中key所对应的val.若不存在该key则添加key, 若key已经存在则覆盖key所对应的val.
* @Param  : *self, 待操作的hashMap.
* @Param  : *key, 获取该key对应的val.
* @Param  : *value, 设置key的值为value.
* @Return : 返回key之前所对应的val.
* @Authro : 卢帅吉
* @Date   : 2017.11.26
*/
static void * put(HashMap * const self, void *key, void * value){
	int idx = 0;
	KVNode *bucketop = NULL;

	self->_resize(self);			// 是否进行扩充

	idx = self->_hash(key) % self->_bucketsLength;
	bucketop = self->_buckets[idx];

	for (KVNode *node = self->_buckets[idx]; node != NULL; node = node->next){
		if (strcmp(node->key, key) == 0){	// 覆盖
			void *old = node->value;
			node->value = value;
			return old;
		}
	}

	KVNode * newnode = (KVNode *)malloc(sizeof(KVNode));
	newnode->key = key;
	newnode->value = value;
	newnode->next = bucketop;
	self->_buckets[idx] = newnode;
	self->_size += 1;
	return NULL;
}

/*
* @Desc   : 返回HashMap中的kv对个数.
* @Param  : *self, 待操作的hashMap.
* @Return : 无
* @Authro : 卢帅吉
* @Date   : 2017.11.26
*/
static int size(HashMap * const self){
	return self->_size;
}

/*
* @Desc   : 以Vector的形式返回HashMap中所有的key.
* @Param  : *self, 待操作的hashMap.
* @Return : 无
* @Authro : 卢帅吉
* @Date   : 2017.11.26
*/
static Vector *keys(HashMap * const self){
	Vector *vector = newVector();
	for (int i = 0; i < self->_bucketsLength; i++){
		for (KVNode *node = self->_buckets[i]; node != NULL; node = node->next){
			vector->add(vector, node->key);
		}
	}
	return vector;
}

/*
* @Desc   : 以Vector的形式返回HashMap中所有的kv对.
* @Param  : *self, 待操作的hashMap.
* @Return : 无
* @Authro : 卢帅吉
* @Date   : 2017.11.26
*/
static Vector *entries(HashMap * const self){
	Vector *vector = newVector();
	for (int i = 0; i < self->_bucketsLength; i++){
		for (KVNode *node = self->_buckets[i]; node != NULL; node = node->next){
			Entry *entry = (Entry *)malloc(sizeof(Entry));
			entry->key = node->key;
			entry->value = node->value;
			vector->add(vector, entry);
		}
	}
	return vector;
}

/*
* @Desc   : 对桶进行扩容操作.当添加一个kv对时, 桶的平均深度大于maxAverageDeep时, 将会对桶进行扩容操作.
* @Param  : *self, 待操作的hashMap.
* @Return : 无
* @Authro : 卢帅吉
* @Date   : 2017.11.26
*/
static void resize(HashMap * const self){
	float averageDeep = (float)(self->_size + 1) / self->_bucketsLength;
	if (averageDeep < self->_maxAverageDeep){
		return;
	}
	// 新的桶和桶大小
	int newBucketsLength = self->_bucketsLength * 2;
	KVNode **newBuckets = (KVNode **)malloc(sizeof(KVNode *)*newBucketsLength);
	for (int i = 0; i < newBucketsLength; i++){
		newBuckets = NULL;
	}
	// 旧桶节点移动到新桶
	for (int i = 0; i < self->_bucketsLength; i++){
		KVNode *tmpnode = NULL;
		for (KVNode *node = self->_buckets[i]; node != NULL; node = tmpnode->next){
			int idx = self->_hash(node->key) % newBucketsLength;
			KVNode * bucketop = newBuckets[idx];
			tmpnode = node->next;
			node->next = bucketop;
			newBuckets[idx] = node;
		}
	}
	free(self->_buckets);
	self->_buckets = newBuckets;
	self->_bucketsLength = newBucketsLength;
}

/*
* @Desc   : 对字符串的默认hash处理.
* @Param  : *obj, 应为字符串首地址.
* @Return : 返回对应的hash值.
* @Authro : 卢帅吉
* @Date   : 2017.11.26
*/
static unsigned long defaultHash(void *obj){
	char *s = (char *)obj;
	unsigned long val = 0;
	for (int i = 0; s[i] != '\0'; i++){
		val += s[i];
	}
	return val;
}