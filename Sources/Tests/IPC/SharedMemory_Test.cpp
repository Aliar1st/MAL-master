#include <gtest/gtest.h>

#include <functional>
#include <thread>
#include "IPC/SharedMemory.h"

namespace IPC_Tests
{
	class SharedMemory_Test : public ::testing::Test
	{
	protected:

		constexpr static const char SHARED_MEMORY_NAME[] = "TEST_SHARED_MEMORY_NAME";
		constexpr static const char BUFFER[] = "Hello World";

		SharedMemory sharedMemory;

	public:

		virtual void SetUp() override
		{
		}

		virtual void TearDown() override
		{
			// Make sure the shared memory is destroyed
			sharedMemory.Destroy();
		}
	};

	// #####
	// TESTS
	// #####
	TEST_F(SharedMemory_Test, InitialValues)
	{
		ASSERT_FALSE(sharedMemory.IsOpened());
		ASSERT_EQ(sharedMemory.GetDataPointer(), nullptr);
		ASSERT_EQ(sharedMemory.GetName(), nullptr);
	}

	TEST_F(SharedMemory_Test, Create)
	{
		ASSERT_EQ(sharedMemory.Create(SHARED_MEMORY_NAME, sizeof(BUFFER)), SHARED_MEMORY_RETURN_SUCCESS);

		ASSERT_TRUE(sharedMemory.IsOpened());
		ASSERT_NE(sharedMemory.GetDataPointer(), nullptr);
		ASSERT_STREQ(sharedMemory.GetName(), SHARED_MEMORY_NAME);

		ASSERT_EQ(sharedMemory.Destroy(), SHARED_MEMORY_RETURN_SUCCESS);
	}

	TEST_F(SharedMemory_Test, Open)
	{
		ASSERT_EQ(sharedMemory.Create(SHARED_MEMORY_NAME, sizeof(BUFFER)), SHARED_MEMORY_RETURN_SUCCESS);

		SharedMemory sharedMemory2;

		ASSERT_EQ(sharedMemory2.Open(SHARED_MEMORY_NAME, sizeof(BUFFER)), SHARED_MEMORY_RETURN_SUCCESS);
		ASSERT_EQ(sharedMemory2.Close(), SHARED_MEMORY_RETURN_SUCCESS);

		ASSERT_EQ(sharedMemory.Destroy(), SHARED_MEMORY_RETURN_SUCCESS);
	}

	TEST_F(SharedMemory_Test, ReadAndWrite)
	{
		ASSERT_EQ(sharedMemory.Create(SHARED_MEMORY_NAME, sizeof(BUFFER)), SHARED_MEMORY_RETURN_SUCCESS);

		SharedMemory sharedMemory2;

		ASSERT_EQ(sharedMemory2.Open(SHARED_MEMORY_NAME, sizeof(BUFFER)), SHARED_MEMORY_RETURN_SUCCESS);

		char* dataPointer = static_cast<char*>(sharedMemory.GetDataPointer());

		memcpy(dataPointer, BUFFER, sizeof(BUFFER));

		dataPointer = static_cast<char*>(sharedMemory2.GetDataPointer());

		ASSERT_STREQ(dataPointer, BUFFER);

		ASSERT_EQ(sharedMemory2.Close(), SHARED_MEMORY_RETURN_SUCCESS);

		ASSERT_EQ(sharedMemory.Destroy(), SHARED_MEMORY_RETURN_SUCCESS);
	}
}