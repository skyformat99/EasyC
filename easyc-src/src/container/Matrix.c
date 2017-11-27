#include <stdio.h>
#include <Windows.h>

#include "Vector.h"
#include "Object.h"
#include "Matrix.h"

static void *get(Matrix * const self, int m, int n);
static void *set(Matrix * const self, int m, int n, void *val);

static int rowSize(Matrix * const self);
static int colSize(Matrix * const self);
static int totalSize(Matrix * const self);

/*
* @Desc   : Matrix������������Matrix�ռ䣬װ�غ�����
* @Return : �����µĹ����õ�Matrix
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
static Matrix *newMatrix(){
	Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
	ZeroMemory(mat, sizeof(Matrix));
	mat->_relative = newVector();

	// load function
	mat->get = get;
	mat->set = set;
	mat->rowSize = rowSize;
	mat->colSize = colSize;
	mat->totalSize = totalSize;
	return mat;
}

/*
* @Desc   : Matrix������������һ��ָ����С�ľ��󣬲���ÿ�������Ԫ��Ϊָ����intֵ��
* @Param  : number, ���Matrix��ÿ��Ԫ�ص����ݡ�
* @Param  : rows, Matrix������
* @Param  : cols, Matrix������
* @Return : �����µĹ����õ�Vector
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
Matrix *newMatrixByNumber(int number, int rows, int cols){
	Matrix * mat = newMatrix();
	mat->_mem = (Object ***)malloc(sizeof(Object **) * rows);
	for (int i = 0; i < rows; i++){
		mat->_mem[i] = (Object **)malloc(sizeof(Object *) * cols);
		for (int j = 0; j < cols; j++){
			mat->_mem[i][j] = simpleObject(number);
		}
	}
	mat->_colLength = cols;
	mat->_rowLength = rows;
	return mat;
}

/*
* @Desc   : �ͷ�Matrix�����ռ�
* @Param  : *mat, ��Ҫ�ͷŵ�Matrixʵ��
* @Return : void, �޷���ֵ
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
void freeMatrix(Matrix * const mat){
	for (int i = 0; i < mat->_rowLength; i++){
		for (int j = 0; j < mat->_colLength; j++){
			freeObject(mat->_mem[i][j]);
		}
		free(mat->_mem[i]);
	}
	freeVector(mat->_relative);
	free(mat);
}

/*
* @Desc   : ��þ�����ָ��λ�õ�Ԫ��
* @Param  : *self, ��������Matrix
* @Param  : m, ָ������ȡԪ�ص���
* @Param  : n, ָ������ȡԪ�ص���
* @Return : �����µĹ����õ�Vector
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
void *get(Matrix * const self, int m, int n){
	if (m >= self->_rowLength || n >= self->_colLength){
		return NULL;
	}
	return self->_mem[m][n]->item;
}

/*
* @Desc   : ����Matrix��ָ��λ�õ�ֵ
* @Param  : *self, ��������Matrix
* @Param  : m, ָ����Ҫ���õ�Ԫ�ص���
* @Param  : n, ָ����Ҫ���õ�Ԫ�ص���
* @Return : ��λ����ǰ��Ԫ��
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
void *set(Matrix * const self, int m, int n, void *val){
	if (m >= self->_rowLength || n>=self->_colLength){
		return NULL;
	}
	void * old = self->_mem[m][n]->item;
	self->_mem[m][n]->item = val;
	return old;
}

/*
* @Desc   : ��ȡMatrix������
* @Param  : *self, ��������Matrix
* @Return : ����Matrix������
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
int rowSize(Matrix * const self){
	return self->_rowLength;
}

/*
* @Desc   : ��ȡMatrix������
* @Param  : *self, ��������Matrix
* @Return : ����Matrix������
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
int colSize(Matrix * const self){
	return self->_colLength;
}


/*
* @Desc   : ��ȡMatrix����Ԫ����
* @Param  : *self, ��������Matrix
* @Return : ����Matrix����Ԫ����
* @Authro : Shuaiji Lu
* @Date   : 2017/11/26
*/
int totalSize(Matrix * const self){
	return self->_colLength*self->_rowLength;
}
