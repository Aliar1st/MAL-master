#include <gtest/gtest.h>

#include <condition_variable>
#include <mutex>
#include "Threading/Thread.h"

namespace Threading
{
	class ThreadClass : public Thread
	{
	private:

		std::mutex* mutex;
		bool* conditionSatisfied;
		std::condition_variable* conditionVariable;

	protected:

		virtual void threadRoutine() override
		{
			// Satisfy the condition
			std::unique_lock<std::mutex> uniqueLock(*mutex);

			*conditionSatisfied = true;
			conditionVariable->notify_one();
		}

		virtual void prepareJoin() override
		{
			prepareJoinCalled = true;
		}

		virtual void joined() override
		{
			joinedCalled = true;
		}

	public:

		bool prepareJoinCalled;
		bool joinedCalled;

		ThreadClass(std::mutex* mutex, bool* conditionSatisfied, std::condition_variable* conditionVariable)
			: mutex(mutex),
			  conditionSatisfied(conditionSatisfied),
			  conditionVariable(conditionVariable)
		{
			prepareJoinCalled = false;
			joinedCalled = false;
		}

		virtual const char* GetName() override
		{
			return "Thread_TestClass";
		}
	};

	class Thread_Test : public ::testing::Test
	{
	protected:

		std::mutex mutex;
		bool conditionSatisfied;
		std::condition_variable conditionVariable;

		ThreadClass thread;

	public:

		Thread_Test()
			: thread(&mutex, &conditionSatisfied, &conditionVariable)
		{
			conditionSatisfied = false;
		}

		virtual void SetUp() override
		{
		}

		virtual void TearDown() override
		{
			// Make sure the timer is joined
			thread.Join();
		}
	};

	// #####
	// TESTS
	// #####
	TEST_F(Thread_Test, InitialValues)
	{
		ASSERT_FALSE(thread.IsRunning());

		ASSERT_FALSE(thread.IsJoinable());

		// Test if joining a non-running thread does anything
		thread.Join();
		ASSERT_FALSE(thread.prepareJoinCalled);
		ASSERT_FALSE(thread.joinedCalled);
	}

	TEST_F(Thread_Test, Run)
	{
		thread.Start();

		ASSERT_TRUE(thread.IsJoinable());

		// Wait until the thread satisfies the condition
		{
			std::unique_lock<std::mutex> uniqueLock(mutex);

			while (!conditionSatisfied)
			{
				conditionVariable.wait(uniqueLock);
			}
		}

		// Busy wait until the thread stops running
		while (thread.IsRunning())
		{
		}
		ASSERT_TRUE(thread.IsJoinable());

		thread.Join();

		ASSERT_FALSE(thread.IsRunning());
		ASSERT_FALSE(thread.IsJoinable());
	}

	TEST_F(Thread_Test, Joining)
	{
		thread.Start();

		thread.PrepareJoin();
		ASSERT_TRUE(thread.prepareJoinCalled);
		ASSERT_FALSE(thread.joinedCalled);

		thread.prepareJoinCalled = false;

		thread.Join();
		ASSERT_FALSE(thread.prepareJoinCalled);
		ASSERT_TRUE(thread.joinedCalled);
	}
}