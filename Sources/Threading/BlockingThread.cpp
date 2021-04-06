#include "BlockingThread.h"

BlockingThread::BlockingThread()
{
	reset();
}

BlockingThread::~BlockingThread()
{
}

void BlockingThread::Block()
{
	std::unique_lock<std::mutex> uniqueLock(stateMutex);

	// Only block if unblocked
	if (state == State::UNBLOCKED || state == State::UNBLOCKING)
	{
		state = State::BLOCKING;
		block();
	}
}

void BlockingThread::Unblock()
{
	std::unique_lock<std::mutex> uniqueLock(stateMutex);

	// Only unblock if blocked
	if (state == State::BLOCKED || state == State::BLOCKING)
	{
		state = State::UNBLOCKING;
		unblock();

		unblockCondition.notify_one();
	}
}

void BlockingThread::BlockUntilIsBlocked()
{
	std::unique_lock<std::mutex> uniqueLock(stateMutex);

	while (state != State::BLOCKED && !isJoining)
	{
		isBlockedCondition.wait(uniqueLock);
	}
}

void BlockingThread::start()
{
	Thread::Start();
}

void BlockingThread::join()
{
	Thread::Join();
}

void BlockingThread::prepareJoin()
{
	{
		std::unique_lock<std::mutex> uniqueLock(stateMutex);

		isJoining = true;
		unblockCondition.notify_one();
	}

	// Block the thread (needs to be done before joining as it might be stuck in the unblocked thread routine
	Block();
}

void BlockingThread::blocked()
{
}

void BlockingThread::block()
{
}

void BlockingThread::unblock()
{
}

void BlockingThread::threadRoutine()
{
	while (!isJoining)
	{
		// Block if necessary
		{
			std::unique_lock<std::mutex> uniqueLock(stateMutex);

			while (state != State::UNBLOCKING && !isJoining)
			{
				state = State::BLOCKED;

				// Notify all threads waiting for this thread to be blocked
				isBlockedCondition.notify_all();

				unblockCondition.wait(uniqueLock);
			}
		}

		// Abort early if joining
		if (isJoining)
		{
			return;
		}

		stateMutex.lock();
		state = State::UNBLOCKED;
		stateMutex.unlock();

		// The thread is unblocked; Execute its routine
		while (shallExecute())
		{
			unblockedRoutine();
		}

		stateMutex.lock();
		state = State::BLOCKING;
		blocked();
		stateMutex.unlock();
	}
}

bool BlockingThread::shallExecute()
{
	return !isJoining && state == State::UNBLOCKED;
}

void BlockingThread::reset()
{
	std::unique_lock<std::mutex> uniqueLock(stateMutex);

	state = State::BLOCKED;
	isJoining = false;
}