#pragma once
#ifdef WIN32
#include <windows.h>

class IMyLock
{
public:
	virtual ~IMyLock() {}

	virtual void Lock() const = 0;
	virtual void Unlock() const = 0;
};

class Mutex : public IMyLock
{
public:
	Mutex();
	~Mutex();

	virtual void Lock() const;
	virtual void Unlock() const;

private:
	HANDLE m_mutex;
};
//Lock
class CLock
{
public:
	CLock(const IMyLock&);
	~CLock();

private:
	const IMyLock& m_lock;
};
#else
#include <pthread.h>
using namespace std;
class Mutex {
public:
	Mutex();
	~Mutex();
	;
public:
	void Lock();
	void Unlock();


private:
	pthread_mutex_t* m_mutex;
};

#endif
