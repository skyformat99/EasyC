#include "RIO.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <fileapi.h>

static int readn(RIO *self, void *userbuf, unsigned int n);
static int readline(RIO *self, void *userbuf, unsigned int n);
static int writen(RIO *self, void *userbuf, unsigned int n);

void initRIO(RIO *rio, HANDLE hi, HANDLE ho){
	ZeroMemory(rio, sizeof(RIO));

	rio->_hi = hi;
	rio->_ho = ho;
	rio->_ptr = rio->_memory;
	
	// load function
	rio->readline = readline;
	rio->readn = readn;
	rio->writen = writen;
}

/*
�����Ļ����������
��ȡָ�����ȴ�С�����ݵ�buf�С�
�ȳ��Խ�������������Ȼ�󽫻����������ݿ�����buf�У���󷵻ض������ݵĸ�����
*/
static int basicRead(RIO *rp, char *buf, unsigned int n){
	if (rp->_unreadSize <= 0){		// ����������Ϊ�գ���Ҫ��ȡ����
		if (ReadFile(rp->_hi, rp->_memory, sizeof(rp->_memory), &rp->_unreadSize, NULL) == FALSE){
			// # ERROR #
			return -1;
		}
		if (rp->_unreadSize == 0){		// ���ݶ�ȡ���
			return 0;
		}
		else{
			rp->_ptr = rp->_memory;	// ��ȡ���µĻ��棬��������ͷ
		}
	}

	// ��������������
	int cnt = rp->_unreadSize < n ? rp->_unreadSize : n;	// ���������������С�ڴ�ȡ�������ݸ�������ȡ��������Ϊ�������Ĵ�С
	memcpy(buf, rp->_ptr, cnt);								// ȡ������
	rp->_ptr += cnt;										// �ƶ�
	rp->_unreadSize -= cnt;
	return cnt;
}

static int readn(RIO *self, void *userbuf, unsigned int n){

}
static int readline(RIO *self, void *userbuf, unsigned int maxlen){
	int n = 0;
	char *bufp = userbuf;
	for (n = 1; n < maxlen; n++){
		int rc = 0;
		char c = 0;
		if ((rc = basicRead(self, &c, 1)) == 1){	// �ӻ����������ȡ���ݣ�����������û�����ݻ��ȳ��Խ�����������������
			*bufp++ = c;
			if (c == '\n'){
				break;
			}
		}
		else if (rc == 0){
			if (n == 1){
				return 0;
			}
			else{
				break;
			}
		}
		else{
			return -1;
		}
	}
	return n;
}
static int writen(RIO *self, void *userbuf, unsigned int n){

}