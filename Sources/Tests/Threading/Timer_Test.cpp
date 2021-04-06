#include <gtest/gtest.h>

#include <condition_variable>
#include <mutex>
#include "Threading/Timer.h"

namespace Threading
{
	class Timer_Test : public ::testing::Test
	{
	protected:

		constexpr static const time_interval_t TIMER_INTERVAL = 1;

		Timer timer;

	public:

		virtual void SetUp() override
		{
		}

		virtual void TearDown() override
		{
			// Make sure the timer is joined
			timer.Join();
		}
	};

	// #####
	// TESTS
	// #####
	TEST_F(Timer_Test, InitialValues)
	{
		ASSERT_FALSE(timer.IsRunning());

		ASSERT_FALSE(timer.IsJoinable());
	}

	TEST_F(Timer_Test, RunOnce)
	{
		int counter = 0;

		auto timerFunction = [&](bool timePassed, void* data)
		{
			if (timePassed)
			{
				counter++;
			}
		};

		timer.InitializeData(TIMER_INTERVAL, false, timerFunction, NULL);
		timer.Start();

		// Busy wait until the thread stops running
		while (timer.IsRunning())
		{
		}

		timer.Join();
		ASSERT_EQ(1, counter);
	}

	TEST_F(Timer_Test, RunLoop)
	{
		std::mutex counterMutex;
		int counter = 0;
		std::condition_variable counterConditionVariable;

		auto timerFunction = [&](bool timePassed, void* data)
		{
			if (timePassed)
			{
				std::unique_lock<std::mutex> counterUniqueLock(counterMutex);

				counter++;
				if (counter == 2)
				{
					counterConditionVariable.notify_one();
				}
			}
		};

		timer.InitializeData(TIMER_INTERVAL, true, timerFunction, NULL);
		timer.Start();

		{
			std::unique_lock<std::mutex> counterUniqueLock(counterMutex);

			while (counter < 2)
			{
				counterConditionVariable.wait(counterUniqueLock);
			}
		}

		timer.Join();
	}

	TEST_F(Timer_Test, ThreadData)
	{
		int intData = 86457;
		int intDataModified = intData;

		auto timerFunction = [&](bool timePassed, void* data)
		{
			int* threadIntData = static_cast<int*>(data);

			ASSERT_EQ(intData, *threadIntData);
			(*threadIntData)++;
		};

		timer.InitializeData(TIMER_INTERVAL, false, timerFunction, &intDataModified);
		timer.Start();
		timer.Join();

		ASSERT_EQ(intData + 1, intDataModified);
	}
}