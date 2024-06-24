#pragma once
#include <condition_variable>
#include <mutex>
/*
��д��(д�����Ȳ���)��
1.��û��д������ʱ�򣬶�����߿���ͬʱ��ȡ��
2.����д����ʱ����������Ҫ�ȴ�
3.����д����ʱ��д����Ҳ��ȴ�

*/
class CReadWriteLock
{
public:
	CReadWriteLock();
	virtual ~CReadWriteLock();
	void ReadLock();	//��ȡ����
	void ReadUnLock();	//�ͷŶ���
	void WriteLock();	//��ȡд��
	void WriteUnlock();	//�ͷ�д��

private:
	std::mutex m_mutex;	//������

	int m_readerCounts;	//��������
	int m_writerCounts; //д������  Ϊ0����1
	int m_writerWaitingCounts;	//���ڵȴ���д������

	std::condition_variable m_conditionRead;
	std::condition_variable m_conditionWrite;

};

