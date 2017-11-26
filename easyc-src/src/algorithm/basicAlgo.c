#include "basicAlgo.h"
#include "Vector.h"
/*
* @Desc   : ��vector����Сֵ��������
* @Param  : vector, ԭʼ��Vector
* @Param  : *idx, ������Сֵ������
* @Return : ����vector�е���Сֵ
* @Authro : ¬˧��
* @Date   : 2017.11.26
*/
int minInVector(Vector *vector, int *idx){
	int val = vector->get(vector, 0);
	*idx = 0;
	for (int i = 1; i < vector->size(vector); i++){
		int v = vector->get(vector, i);
		if (v < val){
			val = v;
			*idx = i;
		}
	}
	return val;
}