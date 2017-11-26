#ifndef __MATRIX_H
#define __MATRIX_H

struct Matrix;
typedef struct Matrix Matrix;

// ���������
Matrix *newMatrix();
Matrix *newMatrixByNumber(int number, int rows, int cols);
void freeMatrix(Matrix * const mat);

/*
* @Desc   : �̶����д�С�Ķ�ά����
* @Author : Shuaiji Lu
* @Date   : 2017.11.26
*/
struct Matrix{
	int _rowLength;
	int _colLength;
	void ***_mem;

	void *(*get)(Matrix * const self, int m, int n);
	void *(*set)(Matrix * const self, int m, int n, void *val);

	int(*rowSize)(Matrix * const self);
	int(*colSize)(Matrix * const self);
	int(*totalSize)(Matrix * const self);
};

#endif