#include "ReadWriteLock.h"

CReadWriteLock::CReadWriteLock():
	m_readerCounts(0),
	m_writerCounts(0),
	m_writerWaitingCounts(0)
{

}

CReadWriteLock::~CReadWriteLock()
{
}

void CReadWriteLock::ReadLock()
{
	std::unique_lock<std::mutex> lock(m_mutex);	//����������
	while (m_writerCounts > 0 || m_writerWaitingCounts > 0)	//��д�߻�Ծ������д���ڵȴ�
	{
		m_conditionRead.wait(lock);	//��Ҫ�ȴ�֪ͨ
	}
	m_readerCounts++;//д����������
}

void CReadWriteLock::ReadUnLock()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_readerCounts--;	//д����������
	if (0 == m_readerCounts && m_writerWaitingCounts > 0)	//���û�л�Ծ�Ķ��߲�����д���ڵȴ�
	{
		m_conditionWrite.notify_one();	//֪ͨһλд��
	}
}

void CReadWriteLock::WriteLock()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_writerWaitingCounts++; //��ʼ��ȡ��,���ڵȴ���д������
	while (m_readerCounts > 0 || m_writerCounts > 0)
	{
		m_conditionWrite.wait(lock);
	}
	m_writerWaitingCounts--;	//�ɹ���ȡ��,���ڵȴ���д�߼���
	m_writerCounts++;	//��Ծ��д������
}

void CReadWriteLock::WriteUnlock()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_writerCounts--;	//��Ծ��д�߼���
	m_conditionRead.notify_all();	//֪ͨ���ж���
	m_conditionWrite.notify_one();	//֪ͨһλд��
}
