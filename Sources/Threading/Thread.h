#pragma once

#include <thread>

/**
 * @brief	An abstract, platform-independent thread wrapper
 */
class Thread
{
private:

	std::thread internThread;

	bool joining;
	bool running;

	/**
	 * @brief	Sets the name of this thread
	 *
	 * @param[in]	threadName	The name to name this thread
	 */
	void setName(const char* threadName);

	/**
	 * @brief	The internal thread routine of this thread.
	 */
	void internalThreadRoutine();

protected:

	/**
	 * @brief	Prepares this thread to join (e.g. unblocking blocking functions)
	 */
	virtual void prepareJoin();

	/**
	 * @brief	Called when this thread is joined.
	 */
	virtual void joined();

	/**
	 * @brief	Thread routine of this thread.
	 */
	virtual void threadRoutine() = 0;

	/**
	 * @brief	Returns whether this thread is currently joining.
	 *
	 * @return	True if this thread is joining; False if not.
	 */
	bool isJoining();

	/**
	 * @brief	Returns whether this function is called by the internal thread.
	 *
	 * @return	True if this function call is done by the internal thread; False if not.
	 */
	bool isRunningOnInternalThread();

public:

	/**
	 * @brief	The constructor of Thread.
	 */
	Thread();

	/**
	 * @brief	The destructor of Thread.
	 */
	virtual ~Thread();

	/**
	 * @brief	Prepares this thread to join. The calling thread however does not join this thread. This is done via the CThread::Join() function.
	 */
	void PrepareJoin();

	/**
	 * @brief	Queues this thread to join and blocks the current thread until this thread is joined. If CThread::PrepareJoin() was not called after the last CThread::Start() call, it is being called implicitely.
	 */
	void Join();

	/**
	 * @brief	Starts the wrapped thread.
	 */
	void Start();

	/**
	 * @brief	Returns the name of this thread.
	 *
	 * @return	The name of this thread.
	 */
	virtual const char* GetName() = 0;

	/**
	 * @brief	Returns whether this thread is currently running.
	 *			A thread leaves the running state as soon as will not execute its thread routine anymore.
	 *			Note that even if a thread is not running anymore, it may still be joinable.
	 *
	 * @return	True if this thread is running; False if not.
	 */
	bool IsRunning();

	/**
	 * @brief	Returns whether this thread is currently joinable.
	 *			A thread is joinable if it was started using Start() but not yet joined using Join().
	 *
	 * @return	True if this thread is joinable; False if not.
	 */
	bool IsJoinable();
};