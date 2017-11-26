#ifndef __PROCESS_H
#define __PROCESS_H


// �ṹ����
struct Process;
struct RIO;
typedef struct Process Process;
typedef struct RIO RIO;
typedef HashMap;
typedef void * HANDLE;

Process * newProcess();
void freeProcess(Process * const p);

struct Process{
	HashMap * _hashMap;
	int _isRun;
	RIO *_rio;
	HANDLE *_pHandle;		// ���̾��

	Process *(*set)(Process * const self, char *key, char *value);
	char *(*get)(Process * const self, char *key);
	int(*start)(Process * const self, char *path);							// ���̿�ʼ������������δ��ɡ�
	int(*startBlock)(Process * const self, char *path);						// ���̿�ʼֱ����������δ��ɡ�
	int(*readline)(Process * const self, void *userbuf, unsigned int n);	// ��ȡһ�����ݣ������������Ϊn������һ��ֱ������
	int(*readn)(Process * const self, void *userbuf, unsigned int n);		// ��ȡָ���ֽڴ�С�����ݣ���������������δ��ɡ�
};

#endif