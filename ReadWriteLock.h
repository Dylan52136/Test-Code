#pragma once
#include <condition_variable>
#include <mutex>
/*
读写锁(写者优先策略)：
1.当没有写操作的时候，多个读者可以同时获取锁
2.当有写操作时，读操作需要等待
3.当有写操作时，写操作也需等待

*/
class CReadWriteLock
{
public:
	CReadWriteLock();
	virtual ~CReadWriteLock();
	void ReadLock();	//获取读锁
	void ReadUnLock();	//释放读锁
	void WriteLock();	//获取写锁
	void WriteUnlock();	//释放写锁

private:
	std::mutex m_mutex;	//互斥锁

	int m_readerCounts;	//读者数量
	int m_writerCounts; //写者数量  为0或者1
	int m_writerWaitingCounts;	//正在等待的写者数量

	std::condition_variable m_conditionRead;
	std::condition_variable m_conditionWrite;

};

