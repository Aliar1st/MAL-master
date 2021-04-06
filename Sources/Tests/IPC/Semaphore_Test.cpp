#include <gtest/gtest.h>

#include <functional>
#include <thread>
#include "IPC/Semaphore.h"

namespace IPC_Tests
{
	class Semaphore_Test : public ::testing::Test
	{
	protected:

		constexpr static const char SEMAPHORE_NAME[] = "TEST_SEMAPHORE_NAME";

		Semaphore semaphore;

		void useUnnamed(std::function<void()> lambda)
		{
			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore.Create(NULL, 0));
			lambda();
			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore.Destroy());
		}

		void useNamed(std::function<void()> lambda)
		{
			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore.Create(SEMAPHORE_NAME, 0));
			lambda();
			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore.Destroy());
		}

	public:

		virtual void SetUp() override
		{
		}

		virtual void TearDown() override
		{
			// Make sure the semaphore is destroyed
			semaphore.Destroy();
		}

		void upUnnamedSemaphore(Semaphore* semaphore)
		{
			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore->Up());
		}

		void upNamedSemaphore()
		{
			Semaphore semaphore;

			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore.Open(SEMAPHORE_NAME));
			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore.Up());
			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore.Close());
		}
	};

	// #####
	// TESTS
	// #####
	TEST_F(Semaphore_Test, InitialValues)
	{
		ASSERT_FALSE(semaphore.IsOpened());
	}

	TEST_F(Semaphore_Test, ManipulateUnopened)
	{
		ASSERT_EQ(SEMAPHORE_RETURN_ERROR_NOT_OPENED, semaphore.Close());
		ASSERT_EQ(SEMAPHORE_RETURN_ERROR_NOT_OPENED, semaphore.Destroy());

		ASSERT_EQ(SEMAPHORE_RETURN_ERROR_NOT_OPENED, semaphore.Down());
		ASSERT_EQ(SEMAPHORE_RETURN_ERROR_NOT_OPENED, semaphore.DownExtended(0));

		ASSERT_EQ(SEMAPHORE_RETURN_ERROR_NOT_OPENED, semaphore.Up());
	}

	TEST_F(Semaphore_Test, CreateUnnamed)
	{
		useUnnamed(
			[]()
		{
		});
	}

	TEST_F(Semaphore_Test, CreateNamed)
	{
		useNamed(
			[]()
		{
		});
	}

	TEST_F(Semaphore_Test, CreateTwice)
	{
		auto lambda = [&]()
		{
			ASSERT_EQ(SEMAPHORE_RETURN_ERROR_ALREADY_OPENED, semaphore.Create(NULL, 0));
			ASSERT_EQ(SEMAPHORE_RETURN_ERROR_ALREADY_OPENED, semaphore.Open(SEMAPHORE_NAME));
		};

		useUnnamed(lambda);
		useNamed(lambda);
	}

	TEST_F(Semaphore_Test, OpenNonExistent)
	{
		ASSERT_EQ(SEMAPHORE_RETURN_ERROR_NOT_FOUND, semaphore.Open(SEMAPHORE_NAME));
	}

	TEST_F(Semaphore_Test, UpAndDown)
	{
		auto lambda = [&]()
		{
			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore.Up());
			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore.Down());
		};

		useUnnamed(lambda);
		useNamed(lambda);
	}

	TEST_F(Semaphore_Test, UpAndDownMultithreaded)
	{
		useUnnamed(
			[&]()
		{
			std::thread thread(&Semaphore_Test::upUnnamedSemaphore, this, &semaphore);

			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore.Down());

			thread.join();
		});

		useNamed(
			[&]()
		{
			std::thread thread(&Semaphore_Test::upNamedSemaphore, this);

			ASSERT_EQ(SEMAPHORE_RETURN_SUCCESS, semaphore.Down());

			thread.join();
		});
	}

	TEST_F(Semaphore_Test, DownExtended)
	{
		auto lambda = [&]()
		{
			ASSERT_EQ(semaphore.DownExtended(10), SEMAPHORE_RETURN_ERROR_OPERATION_TIMEOUT);
		};

		useUnnamed(lambda);
		useNamed(lambda);
	}

	TEST_F(Semaphore_Test, IsError)
	{
		ASSERT_FALSE(semaphore.IsError(SEMAPHORE_RETURN_SUCCESS));
		ASSERT_FALSE(semaphore.IsError(SEMAPHORE_RETURN_SUCCESS_ALREADY_EXISTED));
		ASSERT_TRUE(semaphore.IsError(SEMAPHORE_RETURN_ERROR_ALREADY_OPENED));
		ASSERT_TRUE(semaphore.IsError(SEMAPHORE_RETURN_ERROR_NOT_OPENED));
		ASSERT_TRUE(semaphore.IsError(SEMAPHORE_RETURN_ERROR_COULD_NOT_CREATE_SEMAPHORE));
		ASSERT_TRUE(semaphore.IsError(SEMAPHORE_RETURN_ERROR_COULD_NOT_OPEN_SEMAPHORE));
		ASSERT_TRUE(semaphore.IsError(SEMAPHORE_RETURN_ERROR_NOT_FOUND));
		ASSERT_TRUE(semaphore.IsError(SEMAPHORE_RETURN_ERROR_COULD_NOT_INCREASE_SEMAPHORE));
		ASSERT_TRUE(semaphore.IsError(SEMAPHORE_RETURN_ERROR_COULD_NOT_DECREASE_SEMAPHORE));
		ASSERT_TRUE(semaphore.IsError(SEMAPHORE_RETURN_ERROR_OPERATION_ABORTED));
		ASSERT_TRUE(semaphore.IsError(SEMAPHORE_RETURN_ERROR_OPERATION_TIMEOUT));
	}
}