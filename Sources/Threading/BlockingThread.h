#pragma once

#include <condition_variable>
#include <mutex>
#include "Thread.h"

/**
 * @brief	A thread wrapper with its thread running as long as its object exists. It can be blocked and unblocked and therefore reduced creation overhead and allows self-blocking.
 */
class BlockingThread : Thread
{
private:

	enum class State
	{
		BLOCKED,
		UNBLOCKING,
		UNBLOCKED,
		BLOCKING
	};

	std::mutex stateMutex; // Locked whenever the state is being accessed
	State state;

	std::condition_variable unblockCondition; // Notified to unblock
	std::condition_variable isBlockedCondition; // Notified when blocked

	bool isJoining;

	virtual void prepareJoin() override;

	virtual void threadRoutine() override;

	/**
	 * @brief	Resets the blocking thread to its initialization state.
	 */
	void reset();

protected:

	/**
	 * @brief	Starts the wrapped thread. Shall be called only once in the sub class constructor.
	 */
	void start();

	/**
	 * @brief	Queues this thread to join and blocks the current thread until this thread is joined.
	 *			If this thread is unblocked, it will automatically be blocked before being joined
	 */
	void join();

	/**
	 * @brief	Called when this thread is blocked.
	 */
	virtual void blocked();

	/**
	 * @brief	Called whenever this thread is queued to block. Additionally, it is execution before this thread's destruction if the thread is unblocked
	 */
	virtual void block();

	/**
	 * @brief	Called whenever this thread is queued to unblock.
	 */
	virtual void unblock();

	/**
	 * @brief	The thread routine of this thread that is executed as long as this thread is unblocked.
	 */
	virtual void unblockedRoutine() = 0;

	/**
	 * @brief	Returns whether this thread's BlockingThread::virtualUnblockedRoutine shall be executed
	 *
	 * @return	True if BlockingThread::virtualUnblockedRoutine shall be executed; False if not.
	 */
	bool shallExecute();

public:

	/**
	 * @brief	The constructor of BlockingThread.
	 */
	BlockingThread();

	/**
	 * @brief	The destructor of BlockingThread.
	 */
	~BlockingThread();

	/**
	 * @brief	Queues this thread to block.
	 */
	void Block();

	/**
	 * @brief	Queues this thread to unblock.
	 */
	void Unblock();

	/**
	 * @brief	Blocks the calling thread until this thread is fully blocked.
	 */
	void BlockUntilIsBlocked();
};