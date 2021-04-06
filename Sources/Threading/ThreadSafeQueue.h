#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

/**
 * @brief	A thread safe queue
 *
 * @tparam	The type of elements contained in the queue.
 */
template<typename T>
class ThreadSafeQueue
{
private:

	std::queue<T> queue;

	bool canceled = false;

	// Condition variable
	std::mutex mutex;
	std::condition_variable conditionVariable;

public:

	/**
	 * @brief	The constructor of ThreadSafeQueue.
	 */
	ThreadSafeQueue()
	{
	}

	/**
	 * @brief	The destructor of ThreadSafeQueue.
	 */
	~ThreadSafeQueue()
	{
		CancelPop();
	}

	/**
	 * @brief	Cancels any current and future Pop() operations.
	 */
	void CancelPop()
	{
		std::unique_lock<std::mutex> uniqueLock(mutex);

		canceled = true;
		conditionVariable.notify_all();
	}

	/**
	 * @brief	Reset the queue to not cancel any Pop() operation anymore. Counterpart to CancelPop()
	 */
	void ResetPop()
	{
		std::unique_lock<std::mutex> uniqueLock(mutex);

		canceled = false;
	}

	/**
	 * @brief	Pushes an elment to the queue.
	 *
	 * @param	element	The element to add to the queue.
	 */
	void Push(T element)
	{
		std::unique_lock<std::mutex> uniqueLock(mutex);

		queue.push(element);
		conditionVariable.notify_one();
	}

	/**
	 * @brief	Pops an element from the queue. Blocks if the queue is empty.
	 *
	 * @param[out]	element		After this function returns, contains the popped element. If this operation was canceled, the value is unchanged.
	 *
	 * @return	True if the operation wasn't canceled; False if it was.
	 */
	bool Pop(T* element)
	{
		std::unique_lock<std::mutex> uniqueLock(mutex);

		while (queue.empty())
		{
			conditionVariable.wait(uniqueLock);

			// Check if the pop operation was canceled
			if (canceled)
			{
				return false;
			}
		}

		*element = queue.front();

		queue.pop();

		return true;
	}

	/**
	 * @brief	Tries to pop an element from the queue. The function returns immediately.
	 *
	 * @param[out]	element		After this function returns, contains the popped element. If the queue was empty, the value is unchanged
	 *
	 * @return	True if an element could be popped (if the queue wasn't empty); False if not.
	 */
	bool TryPop(T* element)
	{
		std::unique_lock<std::mutex> uniqueLock(mutex);

		bool anyData = !queue.empty();

		// Check if there is data to get
		if (anyData)
		{
			*element = queue.front();

			queue.pop();
		}

		return anyData;
	}
};