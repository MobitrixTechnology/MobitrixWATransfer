#include "BaseLock.h"
#ifdef WIN32

Mutex::Mutex()
{
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);
}


Mutex::~Mutex()
{
	::CloseHandle(m_mutex);
}


void Mutex::Lock() const
{
	DWORD d = WaitForSingleObject(m_mutex, INFINITE);
}

void Mutex::Unlock() const
{
	::ReleaseMutex(m_mutex);
}


CLock::CLock(const IMyLock& m) : m_lock(m)
{
	m_lock.Lock();
}


CLock::~CLock()
{
	m_lock.Unlock();
}

#else
#include <stdlib.h>
Mutex::Mutex()
{
    m_mutex = new pthread_mutex_t;
    int flg = pthread_mutex_init(m_mutex, NULL);
    if (flg != 0)
    {
        abort();
    }

}

Mutex::~Mutex()
{
    int flg = pthread_mutex_destroy(m_mutex);
    if (flg != 0)
    {
        abort();
    }
    delete m_mutex;

}

void Mutex::Lock()
{
    pthread_mutex_lock(m_mutex);
}


void Mutex::Unlock()
{
    pthread_mutex_unlock(m_mutex);
}


#endif