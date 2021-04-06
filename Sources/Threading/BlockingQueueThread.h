#pragma once

#include "BlockingThread.h"
#include "Threading/ThreadSafeQueue.h"

/**
 * @brief	Wraps a thread using a queue.
 *			Elements can be added to the queue from any thread and this thread will then pop these elements
 *
 * @tparam	The type of elements contained in the queue. The elements that are added to the queue must be allocated dynamically using new.
 *
 */
template<typename T>
class BlockingQueueThread : public BlockingThread
{
private:

	ThreadSafeQueue<T*> queue;

	/**
	 * @brief	Clears the queue
	 */
	void clearQueue()
	{
		T* element;

		while (queue.TryPop(&element))
		{
			delete element;
		}
	}

protected:

	virtual void block() override
	{
		queue.CancelPop();
	}

	virtual void unblock() override
	{
		// Clear the queue
		clearQueue();
	}

	virtual void unblockedRoutine() override
	{
		bool continuePopping = true;

		// While this thread shouldn't block, handle the queue
		while (shallExecute() && continuePopping)
		{
			// Pop an element
			T* element = nullptr;

			if (queue.Pop(&element) && shallExecute())
			{
				continuePopping = elementPopped(element);
			}

			// Delete the element
			delete element;
		}
	}

	/**
	 * @brief	Function that is called whenever an element was popped from the queue
	 *
	 * @param[in]	element		Element that was popped
	 *
	 * @return	True if this thread shall continue popping elements; False otherwise
	 */
	virtual bool elementPopped(T* element) = 0;

public:

	/**
	 * @brief	The constructor of BlockingQueueThread.
	 */
	BlockingQueueThread()
	{
	}

	/**
	 * @brief	The destructor of BlockingQueueThread.
	 */
	~BlockingQueueThread()
	{
		// Clear the queue
		clearQueue();
	}

	/**
	 * @brief	Pushes an element to the queue
	 *
	 * @param[in]	element		Element to be pushed. This element must be allocated dynamically using new.
	 */
	void PushToQueue(T* element)
	{
		queue.Push(element);
	}
};