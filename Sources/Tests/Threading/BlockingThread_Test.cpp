#include <gtest/gtest.h>

#include "Threading/BlockingThread.h"

namespace Threading
{
	class BlockingThreadClass : public BlockingThread
	{
	protected:

		virtual void blocked() override
		{
			blockedCallCounter++;
		}

		virtual void block() override
		{
			blockCallCounter++;
		}

		virtual void unblock() override
		{
			unblockCallCounter++;
		}

		virtual void unblockedRoutine() override
		{
			unblockedRoutineCallCounter++;
		}

	public:

		unsigned int blockedCallCounter;
		unsigned int blockCallCounter;
		unsigned int unblockCallCounter;
		unsigned int unblockedRoutineCallCounter;

		BlockingThreadClass()
		{
			blockedCallCounter = 0;
			blockCallCounter = 0;
			unblockCallCounter = 0;
			unblockedRoutineCallCounter = 0;

			start();
		}

		~BlockingThreadClass()
		{
			join();
		}

		virtual const char* GetName() override
		{
			return "BlockingThread_TestClass";
		}
	};

	class BlockingThread_Test : public ::testing::Test
	{
	protected:

		BlockingThreadClass thread;

	public:

		BlockingThread_Test()
		{
		}

		virtual void SetUp() override
		{
		}

		virtual void TearDown() override
		{
		}
	};

	// #####
	// TESTS
	// #####
	TEST_F(BlockingThread_Test, InitialValues)
	{
		ASSERT_EQ(thread.blockedCallCounter, 0);
		ASSERT_EQ(thread.blockCallCounter, 0);
		ASSERT_EQ(thread.unblockCallCounter, 0);
		ASSERT_EQ(thread.unblockedRoutineCallCounter, 0);

		// Test if blocking a blocked thread does anything
		thread.Block();

		ASSERT_EQ(thread.blockedCallCounter, 0);
		ASSERT_EQ(thread.blockCallCounter, 0);
		ASSERT_EQ(thread.unblockCallCounter, 0);
		ASSERT_EQ(thread.unblockedRoutineCallCounter, 0);
	}

	TEST_F(BlockingThread_Test, Unblock)
	{
		thread.Unblock();

		ASSERT_EQ(thread.unblockCallCounter, 1);

		// Busy wait until the thread performs its routine
		while (!thread.unblockedRoutineCallCounter)
		{
		}

		// Block the thread again and wait
		thread.Block();
		thread.BlockUntilIsBlocked();

		ASSERT_EQ(thread.blockCallCounter, 1);
		ASSERT_EQ(thread.blockedCallCounter, 1);
	}

	TEST_F(BlockingThread_Test, UnblockTwice)
	{
		/**
		 * First unblock
		 */
		thread.Unblock();

		ASSERT_EQ(thread.unblockCallCounter, 1);

		// Busy wait until the thread performs its routine
		while (!thread.unblockedRoutineCallCounter)
		{
		}

		// Block the thread again and wait
		thread.Block();
		thread.BlockUntilIsBlocked();

		ASSERT_EQ(thread.blockCallCounter, 1);
		ASSERT_EQ(thread.blockedCallCounter, 1);

		/**
		 * Second unblock
		 */
		thread.unblockedRoutineCallCounter = 0;
		thread.Unblock();

		ASSERT_EQ(thread.unblockCallCounter, 2);

		// Busy wait until the thread performs its routine
		while (!thread.unblockedRoutineCallCounter)
		{
		}

		// Block the thread again and wait
		thread.Block();
		thread.BlockUntilIsBlocked();

		ASSERT_EQ(thread.blockCallCounter, 2);
		ASSERT_EQ(thread.blockedCallCounter, 2);
	}
}