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
	std::unique_lock<std::mutex> lock(m_mutex);	//锁定互斥锁
	while (m_writerCounts > 0 || m_writerWaitingCounts > 0)	//有写者活跃或者有写者在等待
	{
		m_conditionRead.wait(lock);	//需要等待通知
	}
	m_readerCounts++;//写者数量增加
}

void CReadWriteLock::ReadUnLock()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_readerCounts--;	//写者数量减少
	if (0 == m_readerCounts && m_writerWaitingCounts > 0)	//如果没有活跃的读者并且有写者在等待
	{
		m_conditionWrite.notify_one();	//通知一位写者
	}
}

void CReadWriteLock::WriteLock()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_writerWaitingCounts++; //开始获取锁,正在等待的写者增加
	while (m_readerCounts > 0 || m_writerCounts > 0)
	{
		m_conditionWrite.wait(lock);
	}
	m_writerWaitingCounts--;	//成功获取锁,正在等待的写者减少
	m_writerCounts++;	//活跃的写者增加
}

void CReadWriteLock::WriteUnlock()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_writerCounts--;	//活跃的写者减少
	m_conditionRead.notify_all();	//通知所有读者
	m_conditionWrite.notify_one();	//通知一位写者
}
