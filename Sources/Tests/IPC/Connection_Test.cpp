#include "Connection_Test.h"

void IPC_Tests::Connection_Test::readWriteSecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*, ConnectionAsynchronousOperation*)> secondaryThreadWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*, ConnectionAsynchronousOperation*)> secondaryThreadRead, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd)
{
	Connection* secondaryThreadConnection = secondaryThreadStart(mainThreadConnection);

	ASSERT_TRUE(secondaryThreadConnection->IsOpened());

	// Write
	secondaryThreadWrite(secondaryThreadConnection, mainThreadConnection, mainThreadAsyncOp);
	secondaryThreadPostWrite(secondaryThreadConnection, mainThreadConnection);

	// Read
	secondaryThreadRead(secondaryThreadConnection, mainThreadConnection, mainThreadAsyncOp);
	secondaryThreadPostRead(secondaryThreadConnection, mainThreadConnection);

	ASSERT_EQ(CONNECTION_RETURN_SUCCESS, secondaryThreadConnection->Close());

	secondaryThreadEnd(secondaryThreadConnection, mainThreadConnection);
}

void IPC_Tests::Connection_Test::readTimeoutSecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd)
{
	auto secondaryThreadWrite = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		waitForAsyncOpStarted();
		ASSERT_EQ(CONNECTION_RETURN_ERROR_OPERATION_ABORTED, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, NULL));
	};
	auto secondaryThreadRead = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
	};

	readWriteSecondaryThread(mainThreadConnection, mainThreadAsyncOp, secondaryThreadStart, secondaryThreadWrite, secondaryThreadPostWrite, secondaryThreadRead, secondaryThreadPostRead, secondaryThreadEnd);
}

void IPC_Tests::Connection_Test::readCancelSecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd)
{
	auto secondaryThreadWrite = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		waitForAsyncOpStarted();
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->CancelOperation(mainThreadAsyncOp));
	};
	auto secondaryThreadRead = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
	};

	readWriteSecondaryThread(mainThreadConnection, mainThreadAsyncOp, secondaryThreadStart, secondaryThreadWrite, secondaryThreadPostWrite, secondaryThreadRead, secondaryThreadPostRead, secondaryThreadEnd);
}

void IPC_Tests::Connection_Test::readWriteArraySecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd)
{
	int buffer_array_send[2] = {21, 32};
	int buffer_array_recv[2];
	transfer_size_t elementsTransfered;

	auto secondaryThreadWrite = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, secondaryThreadConnection->WriteArray(buffer_array_send, 2, &elementsTransfered, NULL));
		ASSERT_EQ(static_cast<transfer_size_t>(2), elementsTransfered);
	};
	auto secondaryThreadRead = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, secondaryThreadConnection->ReadArray(buffer_array_recv, 2, &elementsTransfered, NULL));
		ASSERT_EQ(static_cast<transfer_size_t>(2), elementsTransfered);
	};

	readWriteSecondaryThread(mainThreadConnection, mainThreadAsyncOp, secondaryThreadStart, secondaryThreadWrite, secondaryThreadPostWrite, secondaryThreadRead, secondaryThreadPostRead, secondaryThreadEnd);

	for (transfer_size_t i = 0; i < elementsTransfered; i++)
	{
		ASSERT_EQ(buffer_array_send[i], buffer_array_recv[i]);
	}
}

void IPC_Tests::Connection_Test::readWriteSingleSecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd)
{
	int buffer_send = 45;
	int buffer_recv;

	auto secondaryThreadWrite = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, secondaryThreadConnection->WriteSingle(buffer_send, NULL));
	};
	auto secondaryThreadRead = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, secondaryThreadConnection->ReadSingle(&buffer_recv, NULL));
	};

	readWriteSecondaryThread(mainThreadConnection, mainThreadAsyncOp, secondaryThreadStart, secondaryThreadWrite, secondaryThreadPostWrite, secondaryThreadRead, secondaryThreadPostRead, secondaryThreadEnd);

	ASSERT_EQ(buffer_send, buffer_recv);
}

void IPC_Tests::Connection_Test::readWriteStringSecondaryThread(Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp, const std::function<Connection* (Connection*)> secondaryThreadStart, const std::function<void(Connection*, Connection*)> secondaryThreadPostWrite, const std::function<void(Connection*, Connection*)> secondaryThreadPostRead, const std::function<void(Connection*, Connection*)> secondaryThreadEnd)
{
	char buffer_string_send[5] = "test";
	char buffer_string_recv[20];
	transfer_size_t elementsTransfered;

	auto secondaryThreadWrite = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, secondaryThreadConnection->WriteString(buffer_string_send, &elementsTransfered, NULL));
		ASSERT_EQ(static_cast<transfer_size_t>(5), elementsTransfered);
	};
	auto secondaryThreadRead = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, secondaryThreadConnection->ReadArray(buffer_string_recv, 20, &elementsTransfered, NULL));
		ASSERT_EQ(static_cast<transfer_size_t>(5), elementsTransfered);
	};

	readWriteSecondaryThread(mainThreadConnection, mainThreadAsyncOp, secondaryThreadStart, secondaryThreadWrite, secondaryThreadPostWrite, secondaryThreadRead, secondaryThreadPostRead, secondaryThreadEnd);

	ASSERT_STREQ(buffer_string_send, buffer_string_recv);
}

void IPC_Tests::Connection_Test::waitForAsyncOpStarted()
{
	std::unique_lock<std::mutex> asyncOpStartedUniqueLock(mainThreadAsyncOpStartedMutex);

	while (!mainThreadAsyncOpStarted)
	{
		mainThreadAsyncOpStartedCondition.wait(asyncOpStartedUniqueLock);
	}
}

void IPC_Tests::Connection_Test::signalAsyncOpStarted()
{
	mainThreadAsyncOpStartedMutex.lock();
	mainThreadAsyncOpStarted = true;
	mainThreadAsyncOpStartedCondition.notify_one();
	mainThreadAsyncOpStartedMutex.unlock();
}

// #####
// TESTS
// #####
void IPC_Tests::Connection_Test::InitialValues()
{
	ASSERT_FALSE(getConnection()->IsOpened());

	// Check if an asynchronous operation is in a valid state
	ConnectionAsynchronousOperation* mainThreadAsyncOp = getConnection()->CreateAsynchronousOperation();

	ASSERT_TRUE(mainThreadAsyncOp->IsValid());
	ASSERT_TRUE(getConnection()->IsOperationCompleted(mainThreadAsyncOp));
	ASSERT_EQ(CONNECTION_RETURN_SUCCESS, getConnection()->WaitForOperation(mainThreadAsyncOp, NULL));
	ASSERT_EQ(CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED, getConnection()->CancelOperation(mainThreadAsyncOp));
}

void IPC_Tests::Connection_Test::ManipulateUnopened()
{
	int buffer_array[2] = {0, 0};
	int buffer_single;
	char buffer_string[] = "test";

	ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, getConnection()->Close());

	ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, getConnection()->ReadArray(buffer_array, sizeof(buffer_array), NULL, NULL));
	ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, getConnection()->ReadArrayExtended(buffer_array, sizeof(buffer_array), NULL, 0));

	ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, getConnection()->ReadSingle(&buffer_single, NULL));
	ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, getConnection()->ReadSingleExtended(&buffer_single, 0));

	ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, getConnection()->WriteArray(buffer_array, sizeof(buffer_array), NULL, NULL));
	ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, getConnection()->WriteArrayExtended(buffer_array, sizeof(buffer_array), NULL, 0));

	ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, getConnection()->WriteSingle(buffer_single, NULL));
	ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, getConnection()->WriteSingleExtended(buffer_single, 0));

	ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, getConnection()->WriteString(buffer_string, NULL, NULL));
	ASSERT_EQ(CONNECTION_RETURN_ERROR_NOT_OPENED, getConnection()->WriteStringExtended(buffer_string, NULL, 0));
}

void IPC_Tests::Connection_Test::ReadTimeout()
{
	int buffer;

	auto secondaryThreadFunction = &Connection_Test::readTimeoutSecondaryThread;
	auto mainThreadStart = std::bind(&Connection_Test::mainThreadStart, this);
	auto mainThreadPreRead = std::bind(&Connection_Test::mainThreadPreRead, this, std::placeholders::_1);
	auto mainThreadRead = [&](Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->ReadSingle(&buffer, mainThreadAsyncOp));

		signalAsyncOpStarted();

		ASSERT_EQ(CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT, mainThreadConnection->WaitForOperationExtended(mainThreadAsyncOp, NULL, 10));
	};
	auto mainThreadPostRead = [&](Connection* mainThreadConnection)
	{
	};
	auto mainThreadWrite = [&](Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
	};
	auto mainThreadPostWrite = [&](Connection* mainThreadConnection)
	{
	};
	auto mainThreadEnd = std::bind(&Connection_Test::mainThreadEnd, this, std::placeholders::_1);
	auto secondaryThreadStart = std::bind(&Connection_Test::secondaryThreadStart, this, std::placeholders::_1);
	auto secondaryThreadPostWrite = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection)
	{
	};
	auto secondaryThreadPostRead = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection)
	{
	};
	auto secondaryThreadEnd = std::bind(&Connection_Test::secondaryThreadEnd, this, std::placeholders::_1, std::placeholders::_2);

	readWrite(secondaryThreadFunction, mainThreadStart, mainThreadPreRead, mainThreadRead, mainThreadPostRead, mainThreadWrite, mainThreadPostWrite, mainThreadEnd, secondaryThreadStart, secondaryThreadPostWrite, secondaryThreadPostRead, secondaryThreadEnd);
}

void IPC_Tests::Connection_Test::ReadCancel()
{
	int buffer;

	auto secondaryThreadFunction = &Connection_Test::readCancelSecondaryThread;
	auto mainThreadStart = std::bind(&Connection_Test::mainThreadStart, this);
	auto mainThreadPreRead = std::bind(&Connection_Test::mainThreadPreRead, this, std::placeholders::_1);
	auto mainThreadRead = [&](Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->ReadSingle(&buffer, mainThreadAsyncOp));

		signalAsyncOpStarted();

		ASSERT_EQ(CONNECTION_RETURN_ERROR_OPERATION_ABORTED, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, NULL));
	};
	auto mainThreadPostRead = [&](Connection* mainThreadConnection)
	{
	};
	auto mainThreadWrite = [&](Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOpp)
	{
	};
	auto mainThreadPostWrite = [&](Connection* mainThreadConnection)
	{
	};
	auto mainThreadEnd = std::bind(&Connection_Test::mainThreadEnd, this, std::placeholders::_1);
	auto secondaryThreadStart = std::bind(&Connection_Test::secondaryThreadStart, this, std::placeholders::_1);
	auto secondaryThreadPostWrite = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection)
	{
	};
	auto secondaryThreadPostRead = [&](Connection* secondaryThreadConnection, Connection* mainThreadConnection)
	{
	};
	auto secondaryThreadEnd = std::bind(&Connection_Test::secondaryThreadEnd, this, std::placeholders::_1, std::placeholders::_2);

	readWrite(secondaryThreadFunction, mainThreadStart, mainThreadPreRead, mainThreadRead, mainThreadPostRead, mainThreadWrite, mainThreadPostWrite, mainThreadEnd, secondaryThreadStart, secondaryThreadPostWrite, secondaryThreadPostRead, secondaryThreadEnd);
}

void IPC_Tests::Connection_Test::ReadWriteArray()
{
	int buffer_array[2];
	transfer_size_t bytesTransferred;

	auto secondaryThreadFunction = &Connection_Test::readWriteArraySecondaryThread;
	auto mainThreadStart = std::bind(&Connection_Test::mainThreadStart, this);
	auto mainThreadPreRead = std::bind(&Connection_Test::mainThreadPreRead, this, std::placeholders::_1);
	auto mainThreadRead = [&](Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->ReadArray(buffer_array, 2, NULL, mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(static_cast<transfer_size_t>(sizeof(buffer_array)), bytesTransferred);

		bytesTransferred = 0;
		ASSERT_TRUE(mainThreadConnection->IsOperationCompleted(mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED, mainThreadConnection->CancelOperation(mainThreadAsyncOp));
		ASSERT_EQ(static_cast<transfer_size_t>(sizeof(buffer_array)), bytesTransferred);
	};
	auto mainThreadPostRead = std::bind(&Connection_Test::mainThreadPostRead, this, std::placeholders::_1);
	auto mainThreadWrite = [&](Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WriteArray(buffer_array, 2, NULL, mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(static_cast<transfer_size_t>(sizeof(buffer_array)), bytesTransferred);

		bytesTransferred = 0;
		ASSERT_TRUE(mainThreadConnection->IsOperationCompleted(mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED, mainThreadConnection->CancelOperation(mainThreadAsyncOp));
		ASSERT_EQ(static_cast<transfer_size_t>(sizeof(buffer_array)), bytesTransferred);
	};
	auto mainThreadPostWrite = std::bind(&Connection_Test::mainThreadPostWrite, this, std::placeholders::_1);
	auto mainThreadEnd = std::bind(&Connection_Test::mainThreadEnd, this, std::placeholders::_1);
	auto secondaryThreadStart = std::bind(&Connection_Test::secondaryThreadStart, this, std::placeholders::_1);
	auto secondaryThreadPostWrite = std::bind(&Connection_Test::secondaryThreadPostWrite, this, std::placeholders::_1, std::placeholders::_2);
	auto secondaryThreadPostRead = std::bind(&Connection_Test::secondaryThreadPostRead, this, std::placeholders::_1, std::placeholders::_2);
	auto secondaryThreadEnd = std::bind(&Connection_Test::secondaryThreadEnd, this, std::placeholders::_1, std::placeholders::_2);

	readWrite(secondaryThreadFunction, mainThreadStart, mainThreadPreRead, mainThreadRead, mainThreadPostRead, mainThreadWrite, mainThreadPostWrite, mainThreadEnd, secondaryThreadStart, secondaryThreadPostWrite, secondaryThreadPostRead, secondaryThreadEnd);
}

void IPC_Tests::Connection_Test::ReadWriteSingle()
{
	int buffer;
	transfer_size_t bytesTransferred;

	auto secondaryThreadFunction = &Connection_Test::readWriteSingleSecondaryThread;
	auto mainThreadStart = std::bind(&Connection_Test::mainThreadStart, this);
	auto mainThreadPreRead = std::bind(&Connection_Test::mainThreadPreRead, this, std::placeholders::_1);
	auto mainThreadRead = [&](Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->ReadSingle(&buffer, mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(static_cast<transfer_size_t>(sizeof(buffer)), bytesTransferred);

		bytesTransferred = 0;
		ASSERT_TRUE(mainThreadConnection->IsOperationCompleted(mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED, mainThreadConnection->CancelOperation(mainThreadAsyncOp));
		ASSERT_EQ(static_cast<transfer_size_t>(sizeof(buffer)), bytesTransferred);
	};
	auto mainThreadPostRead = std::bind(&Connection_Test::mainThreadPostRead, this, std::placeholders::_1);
	auto mainThreadWrite = [&](Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WriteSingle(buffer, mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(static_cast<transfer_size_t>(sizeof(buffer)), bytesTransferred);

		bytesTransferred = 0;
		ASSERT_TRUE(mainThreadConnection->IsOperationCompleted(mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED, mainThreadConnection->CancelOperation(mainThreadAsyncOp));
		ASSERT_EQ(static_cast<transfer_size_t>(sizeof(buffer)), bytesTransferred);
	};
	auto mainThreadPostWrite = std::bind(&Connection_Test::mainThreadPostWrite, this, std::placeholders::_1);
	auto mainThreadEnd = std::bind(&Connection_Test::mainThreadEnd, this, std::placeholders::_1);
	auto secondaryThreadStart = std::bind(&Connection_Test::secondaryThreadStart, this, std::placeholders::_1);
	auto secondaryThreadPostWrite = std::bind(&Connection_Test::secondaryThreadPostWrite, this, std::placeholders::_1, std::placeholders::_2);
	auto secondaryThreadPostRead = std::bind(&Connection_Test::secondaryThreadPostRead, this, std::placeholders::_1, std::placeholders::_2);
	auto secondaryThreadEnd = std::bind(&Connection_Test::secondaryThreadEnd, this, std::placeholders::_1, std::placeholders::_2);

	readWrite(secondaryThreadFunction, mainThreadStart, mainThreadPreRead, mainThreadRead, mainThreadPostRead, mainThreadWrite, mainThreadPostWrite, mainThreadEnd, secondaryThreadStart, secondaryThreadPostWrite, secondaryThreadPostRead, secondaryThreadEnd);
}

void IPC_Tests::Connection_Test::ReadWriteString()
{
	char buffer_string[20];
	transfer_size_t bytesTransferred;

	auto secondaryThreadFunction = &Connection_Test::readWriteStringSecondaryThread;
	auto mainThreadStart = std::bind(&Connection_Test::mainThreadStart, this);
	auto mainThreadPreRead = std::bind(&Connection_Test::mainThreadPreRead, this, std::placeholders::_1);
	auto mainThreadRead = [&](Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->ReadArray(buffer_string, 20, NULL, mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(static_cast<transfer_size_t>(5), bytesTransferred);

		bytesTransferred = 0;
		ASSERT_TRUE(mainThreadConnection->IsOperationCompleted(mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED, mainThreadConnection->CancelOperation(mainThreadAsyncOp));
		ASSERT_EQ(static_cast<transfer_size_t>(5), bytesTransferred);
	};
	auto mainThreadPostRead = std::bind(&Connection_Test::mainThreadPostRead, this, std::placeholders::_1);
	auto mainThreadWrite = [&](Connection* mainThreadConnection, ConnectionAsynchronousOperation* mainThreadAsyncOp)
	{
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WriteString(buffer_string, NULL, mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(static_cast<transfer_size_t>(5), bytesTransferred);

		bytesTransferred = 0;
		ASSERT_TRUE(mainThreadConnection->IsOperationCompleted(mainThreadAsyncOp));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS, mainThreadConnection->WaitForOperation(mainThreadAsyncOp, &bytesTransferred));
		ASSERT_EQ(CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED, mainThreadConnection->CancelOperation(mainThreadAsyncOp));
		ASSERT_EQ(static_cast<transfer_size_t>(5), bytesTransferred);
	};
	auto mainThreadPostWrite = std::bind(&Connection_Test::mainThreadPostWrite, this, std::placeholders::_1);
	auto mainThreadEnd = std::bind(&Connection_Test::mainThreadEnd, this, std::placeholders::_1);
	auto secondaryThreadStart = std::bind(&Connection_Test::secondaryThreadStart, this, std::placeholders::_1);
	auto secondaryThreadPostWrite = std::bind(&Connection_Test::secondaryThreadPostWrite, this, std::placeholders::_1, std::placeholders::_2);
	auto secondaryThreadPostRead = std::bind(&Connection_Test::secondaryThreadPostRead, this, std::placeholders::_1, std::placeholders::_2);
	auto secondaryThreadEnd = std::bind(&Connection_Test::secondaryThreadEnd, this, std::placeholders::_1, std::placeholders::_2);

	readWrite(secondaryThreadFunction, mainThreadStart, mainThreadPreRead, mainThreadRead, mainThreadPostRead, mainThreadWrite, mainThreadPostWrite, mainThreadEnd, secondaryThreadStart, secondaryThreadPostWrite, secondaryThreadPostRead, secondaryThreadEnd);
}

void IPC_Tests::Connection_Test::IsError()
{
	ASSERT_FALSE(getConnection()->IsError(CONNECTION_RETURN_SUCCESS));
	ASSERT_FALSE(getConnection()->IsError(CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED));
	ASSERT_TRUE(getConnection()->IsError(CONNECTION_RETURN_ERROR_NOT_OPENED));
	ASSERT_TRUE(getConnection()->IsError(CONNECTION_RETURN_ERROR_ALREADY_OPENED));
	ASSERT_TRUE(getConnection()->IsError(CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION));
	ASSERT_TRUE(getConnection()->IsError(CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION));
	ASSERT_TRUE(getConnection()->IsError(CONNECTION_RETURN_ERROR_INVALID_OPERATION));
	ASSERT_TRUE(getConnection()->IsError(CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION));
	ASSERT_TRUE(getConnection()->IsError(CONNECTION_RETURN_ERROR_COULD_NOT_CANCEL_OPERATION));
	ASSERT_TRUE(getConnection()->IsError(CONNECTION_RETURN_ERROR_OPERATION_ABORTED));
	ASSERT_TRUE(getConnection()->IsError(CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT));
}